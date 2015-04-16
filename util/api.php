<?php

/*

CREATE DATABASE voxelands ;

USE voxelands ;

CREATE TABLE `servers` (
 `id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
 `name` VARCHAR (255) NOT NULL,
 `addr` VARCHAR (255) NOT NULL,
 `port` INT(11) UNSIGNED,
 `mode` VARCHAR (20) NOT NULL,
 `motd` VARCHAR (255) NOT NULL,
 `players` INT(11) UNSIGNED NOT NULL DEFAULT '0',
 `public` VARCHAR (10) NOT NULL,
 `version` VARCHAR (50) NOT NULL,
 `features` VARCHAR (255) NOT NULL,
 `lastreply` INT(11) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin ;

CREATE TABLE `players` (
 `id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
 `name` VARCHAR(255) NOT NULL,
 `hash` VARCHAR(255) NOT NULL,
 `cookie` VARCHAR(255) NOT NULL,
 `server` INT(11) UNSIGNED NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COLLATE=utf8_bin ;

CREATE USER 'apiuser'@'localhost' IDENTIFIED BY 'apipass';
GRANT SELECT,INSERT,UPDATE ON `servers` TO 'apiuser'@'localhost';
GRANT SELECT,INSERT,UPDATE ON `players` TO 'apiuser'@'localhost';

 */

$db = false;

function db_connect()
{
	global $db;
	if ($db !== false)
		return;

	$db = mysqli_connect("localhost","apiuser","apipass","voxelands");
	if (mysqli_connect_errno())
		$db = false;
}

function db_close()
{
	global $db;
	if ($db === false)
		return;
	$db->close();
	$db = false;
}

function db_insert($server_name, $server_addr, $server_port)
{
	global $db;
	db_connect();
	$name = db_escape($server_name);
	$addr = db_escape($server_addr);
	$port = db_escape($server_port);
	$time = time();
	$data = $db->query("SELECT `id` FROM `servers` WHERE `addr` = '$addr' AND `port` = '$port'");
	if ($data && $data->num_rows > 0) {
		$id = $data->fetch_array();
		$id = $id['id'];
		$data->close();
		$data = $db->query("UPDATE `servers` SET `lastreply` = '$time', `name` = '$name' WHERE `id` = '$id'");
	}else{
		if ($data)
			$data->close();
		$db->query("INSERT INTO `servers` (`name`,`addr`,`port`,`lastreply`) VALUES ('$name','$addr','$port','$time')");
	}
}

function db_query_server($server_name=false,$server_mode=false,$version=false,$public=false)
{
	global $db;
	db_connect();

	$time = time()-1200;
	$q = "SELECT * FROM `servers` WHERE `lastreply` > $time";

	if ($server_name !== false) {
		$name = db_escape($server_name);
		$q .= " AND `name` LIKE '%$name%'";
	}
	if ($server_mode !== false) {
		$mode = db_escape($server_mode);
		$q .= " AND `mode` = '$mode'";
	}
	if ($version !== false) {
		$v = db_escape($version);
		$q .= " AND `mode` LIKE '%$v%'";
	}
	if ($public == 'public' || $public == 'private') {
		$q .= " AND `public` = '$public'";
	}

	$data = $db->query($q);
	if (!$data) {
		return array();
	}elseif ($data->num_rows < 1) {
		$data->close();
		return array();
	}

	$result = array();
	while ($row = $data->fetch_array(MYSQLI_ASSOC)) {
		$result[] = $row;
	}
	$data->close();
	return $result;
}

function db_query_player($player_name=false,$count=false,$start=false)
{
	global $db;
	db_connect();

	$q = "SELECT * FROM `players`";
	if ($player_name !== false) {
		$p = db_escape($player_name);
		$q .= " WHERE `name` LIKE '%$p%'";
	}
	if ($start !== false && $count !== false) {
		$start = intval($start);
		$count = intval($count);
		$q .= " LIMIT $start , $count";
	}elseif ($count !== false) {
		$count = intval($count);
		$q .= " LIMIT $count";
	}

	$data = $db->query($q);
	if (!$data) {
		return array();
	}elseif ($data->num_rows < 1) {
		$data->close();
		return array();
	}

	$result = array();
	while ($row = $data->fetch_array(MYSQLI_ASSOC)) {
		$result[] = $row;
	}
	$data->close();
	return $result;
}

function db_escape($str)
{
	global $db;
	db_connect();
	return $db->real_escape_string($str);
}

function check_server($host, $port)
{
	$socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
	$timeout = array("sec" => 1, "usec" => 0);
	socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, $timeout);
	$buf = "\x4f\x45\x74\x03\x00\x00\x00\x03\xff\xdc\x01";
	socket_sendto($socket, $buf, strlen($buf), 0, $host, $port);
	$buf = socket_read($socket, 1000);
	if ($buf == "")
		return false;

	/* we got a reply, read the peer id then send a disconnect */
	$peer_id = substr($buf, 9, 2);
	$buf = "\x4f\x45\x74\x03".$peer_id."\x00\x00\x03";
	socket_sendto($socket, $buf, strlen($buf), 0, $host, $port);
	socket_close($socket);

	return true;
}

function error_send($msg)
{
	header("Content-Type: text/plain\r\n");
	echo $msg;
}

function txt_send($txt)
{
	header("Content-Type: text/plain\r\n");
	echo $txt;
}

function html_send($html)
{
	readfile($_SERVER['DOCUMENT_ROOT']."/header.html");
	echo $html;
	readfile($_SERVER['DOCUMENT_ROOT']."/footer.html");
}

function server_announce()
{
	$server_addr = isset($_POST['server_address']) ? urldecode($_POST['server_address']) : $_SERVER['REMOTE_ADDR'];
	$server_name = isset($_POST['server_name']) ? urldecode($_POST['server_name']) : $server_addr;
	$server_port = isset($_POST['server_port']) ? urldecode($_POST['server_port']) : '30000';

	if (!check_server($server_addr,$server_port))
		return error_send("Server Not Found");

	db_connect();
	db_insert($server_name,$server_addr,$server_port);
	db_close();

	txt_send($server_addr);
}

function server_list()
{
	$a = db_query_server();
	$count = count($a);
	$format = 'txt';
	if (isset($_GET['format'])) {
		$format = $_GET['format'];
	}else if (isset($_POST['format'])) {
		$format = $_POST['format'];
	}
	if ($format == 'html') {
		$out = '';
		foreach ($a as $server) {
			if ($server['mode'] == '')
				$server['mode'] = 'adventure';
			$out .= <<<EOT

	<div class="panel">
		<h2 class="centre">$server[name]</h2>
		<p class="centre">$server[motd]</p>
		<p><a href="http://$server[addr]:$server[port]/">Web Interface</a></p>
		<p>Mode: $server[mode]</p>
		<p>Address: $server[addr]:$server[port]</p>
	</div>

EOT;
		}
		html_send($out);
	}elseif ($format == 'htmltable') {
		$out = <<<EOT

	<table>
EOT;
		foreach ($a as $server) {
			if ($server['mode'] == '')
				$server['mode'] = 'adventure';
			$out .= <<<EOT

		<tr>
			<td>$server[name]</td>
			<td>$server[motd]</td>
			<td><a href="http://$server[addr]:$server[port]/">Web Interface</a></td>
			<td>Mode: $server[mode]</td>
			<td>Address: $server[addr]:$server[port]</td>
		</tr>

EOT;
		}
		$out .= <<<EOT

	</table>

EOT;
		txt_send($out);
	}elseif ($format == 'xml') {
		$out = <<<EOT
<?xml version="1.0" encoding="UTF-8" ?>
<serverlist count="$count">

EOT;
		foreach ($a as $server) {
			if ($server['mode'] == '')
				$server['mode'] = 'adventure';
			$out .= <<<EOT
	<server>
		<name>$server[name]</name>
		<mode>$server[mode]</mode>
		<address>$server[addr]</address>
		<port>$server[port]</port>
	</server>

EOT;
		}
		$out .= "\n</serverlist>\n";
		txt_send($out);
	}elseif ($format == 'json') {
		$out = "{\n";
		$i = 0;
		foreach ($a as $server) {
			if ($server['mode'] == '')
				$server['mode'] = 'adventure';
			$name = addslashes($server['name']);
			$mode = addslashes($server['mode']);
			$addr = addslashes($server['addr']);
			$port = addslashes($server['port']);
			$out .= <<<EOT

	$i : {
		name : '$name',
		mode : '$mode',
		address : '$addr',
		port : '$port'
	},

EOT;
			$i++;
		}
		$out .= "}\n";
		txt_send($out);
	}else{
		$out = "servers: $count\n\n";
		foreach ($a as $server) {
			if ($server['mode'] == '')
				$server['mode'] = 'adventure';
			$out .= <<<EOT
$server[name]
$server[mode]
$server[addr]:$server[port]


EOT;
		}
		txt_send($out);
	}
}

function player_find()
{
	txt_send("hello world");
}

function home()
{
	html_send("hello world");
}

$u = explode("/",$_SERVER['REQUEST_URI']);
if (count($u) < 2)
	$u = array("","home");

if ($u[1] == "announce") {
	server_announce();
}elseif ($u[1] == "list") {
	server_list();
}elseif ($u[1] == "player") {
	player_find();
}else{
	home();
}

db_close();

?>
