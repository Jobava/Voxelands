/*
Part of Minetest-c55
Copyright (C) 2010-11 celeron55, Perttu Ahola <celeron55@gmail.com>
Copyright (C) 2011 Ciaran Gultnieks <ciaran@ciarang.com>

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include "servercommand.h"
#include "utility.h"
#include "settings.h"

#define PP(x) "("<<(x).X<<","<<(x).Y<<","<<(x).Z<<")"

void cmd_status(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	os<<ctx->server->getStatusString();
}

void cmd_me(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	std::wstring name = narrow_to_wide(ctx->player->getName());
	os << L"* " << name << L" " << ctx->paramstring;
	ctx->flags |= SEND_TO_OTHERS | SEND_NO_PREFIX;
}

void cmd_privs(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if(ctx->parms.size() == 1) {
		// Show our own real privs, without any adjustments
		// made for admin status
		os<<L"-!- " + narrow_to_wide(privsToString(
				ctx->server->getPlayerAuthPrivs(ctx->player->getName())));
		return;
	}

	if ((ctx->privs & PRIV_PRIVS) == 0) {
		os<<L"-!- You don't have permission to do that";
		return;
	}

	Player *tp = ctx->env->getPlayer(wide_to_narrow(ctx->parms[1]).c_str());
	if (tp == NULL) {
		os<<L"-!- No such player";
		return;
	}

	os<<L"-!- " + narrow_to_wide(privsToString(ctx->server->getPlayerAuthPrivs(tp->getName())));
}

void cmd_grantrevoke(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if(ctx->parms.size() != 3)
	{
		os<<L"-!- Missing parameter";
		return;
	}

	if((ctx->privs & PRIV_PRIVS) == 0)
	{
		os<<L"-!- You don't have permission to do that";
		return;
	}

	u64 newprivs = stringToPrivs(wide_to_narrow(ctx->parms[2]));
	if(newprivs == PRIV_INVALID)
	{
		os<<L"-!- Invalid privileges specified";
		return;
	}

	Player *tp = ctx->env->getPlayer(wide_to_narrow(ctx->parms[1]).c_str());
	if(tp == NULL)
	{
		os<<L"-!- No such player";
		return;
	}

	std::string playername = wide_to_narrow(ctx->parms[1]);
	u64 privs = ctx->server->getPlayerAuthPrivs(playername);

	if(ctx->parms[0] == L"grant"){
		privs |= newprivs;
		actionstream<<ctx->player->getName()<<" grants "
				<<wide_to_narrow(ctx->parms[2])<<" to "
				<<playername<<std::endl;

		std::wstring msg;
		msg += narrow_to_wide(ctx->player->getName());
		msg += L" granted you the privilege \"";
		msg += ctx->parms[2];
		msg += L"\"";
		ctx->server->notifyPlayer(playername.c_str(), msg);
	} else {
		privs &= ~newprivs;
		actionstream<<ctx->player->getName()<<" revokes "
				<<wide_to_narrow(ctx->parms[2])<<" from "
				<<playername<<std::endl;

		std::wstring msg;
		msg += narrow_to_wide(ctx->player->getName());
		msg += L" revoked from you the privilege \"";
		msg += ctx->parms[2];
		msg += L"\"";
		ctx->server->notifyPlayer(playername.c_str(), msg);
	}

	ctx->server->setPlayerAuthPrivs(playername, privs);

	os<<L"-!- Privileges change to ";
	os<<narrow_to_wide(privsToString(privs));
}

void cmd_time(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if(ctx->parms.size() != 2)
	{
		os<<L"-!- Missing parameter";
		return;
	}

	if((ctx->privs & PRIV_SETTIME) ==0)
	{
		os<<L"-!- You don't have permission to do that";
		return;
	}

	u32 time = stoi(wide_to_narrow(ctx->parms[1]));
	ctx->server->setTimeOfDay(time);
	os<<L"-!- time_of_day changed.";

	actionstream<<ctx->player->getName()<<" sets time "
			<<time<<std::endl;
}

void cmd_shutdown(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if((ctx->privs & PRIV_SERVER) ==0)
	{
		os<<L"-!- You don't have permission to do that";
		return;
	}

	actionstream<<ctx->player->getName()
			<<" shuts down server"<<std::endl;

	ctx->server->requestShutdown();

	os<<L"*** Server shutting down (operator request)";
	ctx->flags |= SEND_TO_OTHERS;
}

void cmd_setting(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if ((ctx->privs & PRIV_SERVER) ==0) {
		os<<L"-!- You don't have permission to do that";
		return;
	}

	/* no value is set, so instead show the current value */
	if (ctx->parms.size() == 2) {
		if (!g_settings->exists(wide_to_narrow(ctx->parms[1]))) {
			os << L"-!- '" << ctx->parms[1] << L"' is not set";
			return;
		}
		os << L"-!- '" << ctx->parms[1] << L"' has value '"
			<< narrow_to_wide(g_settings->get(wide_to_narrow(ctx->parms[1]))) << L"'";
		return;
	}

	/*std::string confline = wide_to_narrow(
			ctx->parms[1] + L" = " + ctx->params[2]);*/

	std::string confline = wide_to_narrow(ctx->paramstring);

	actionstream<<ctx->player->getName()
			<<" sets: "<<confline<<std::endl;

	g_settings->parseConfigLine(confline);

	ctx->server->saveConfig();

	os<< L"-!- Setting changed and configuration saved.";
}

void cmd_teleport(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if((ctx->privs & PRIV_TELEPORT) ==0)
	{
		os<<L"-!- You don't have permission to do that";
		return;
	}

	if(ctx->parms.size() != 2)
	{
		os<<L"-!- Missing parameter";
		return;
	}

	std::vector<std::wstring> coords = str_split(ctx->parms[1], L',');
	if(coords.size() != 3)
	{
		os<<L"-!- You can only specify coordinates currently";
		return;
	}

	v3f dest(stoi(coords[0])*10, stoi(coords[1])*10, stoi(coords[2])*10);

	actionstream<<ctx->player->getName()<<" teleports from "
			<<PP(ctx->player->getPosition()/BS)<<" to "
			<<PP(dest/BS)<<std::endl;

	ctx->player->setPosition(dest);
	ctx->server->SendMovePlayer(ctx->player);

	os<< L"-!- Teleported.";
}

void cmd_banunban(std::wostringstream &os, ServerCommandContext *ctx)
{
	if ((ctx->privs & PRIV_BAN) == 0) {
		os<<L"-!- You don't have permission to do that";
		return;
	}

	if (ctx->parms.size() < 2) {
		std::string desc = ctx->server->getBanDescription("");
		os<<L"-!- Ban list: "<<narrow_to_wide(desc);
		return;
	}
	if (ctx->parms[0] == L"ban") {
		Player *player = ctx->env->getPlayer(wide_to_narrow(ctx->parms[1]).c_str());

		if (player == NULL) {
			os<<L"-!- No such player";
			return;
		}
		std::wstring msg;
		msg += narrow_to_wide(ctx->player->getName());
		msg += L" has banned you from this server.";
		ctx->server->notifyPlayer(player->getName(), msg);

		try{
			Address address = ctx->server->getPeerAddress(player->peer_id);
			std::string ip_string = address.serializeString();
			ctx->server->setIpBanned(ip_string, player->getName());
			os<<L"-!- Banned "<<narrow_to_wide(ip_string)<<L"|"
					<<narrow_to_wide(player->getName());

			actionstream<<ctx->player->getName()<<" bans "
					<<player->getName()<<" / "<<ip_string<<std::endl;
		} catch(con::PeerNotFoundException){
			std::string ip_string = ((ServerRemotePlayer*)player)->getAddress();
			if (ip_string != "") {
				ctx->server->setIpBanned(ip_string, player->getName());
				os<<L"-!- Banned "<<narrow_to_wide(ip_string)<<L"|"
						<<narrow_to_wide(player->getName());

				actionstream<<ctx->player->getName()<<" bans "
						<<player->getName()<<" / "<<ip_string<<std::endl;
			}else{
				dstream<<__FUNCTION_NAME<<": peer was not found"<<std::endl;
			}
		}
	}else{
		std::string ip_or_name = wide_to_narrow(ctx->parms[1]);
		std::string desc = ctx->server->getBanDescription(ip_or_name);
		ctx->server->unsetIpBanned(ip_or_name);
		os<<L"-!- Unbanned "<<narrow_to_wide(desc);

		actionstream<<ctx->player->getName()<<" unbans "
				<<ip_or_name<<std::endl;
	}
}

void cmd_clearobjects(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if ((ctx->privs & PRIV_SERVER) ==0) {
		os<<L"-!- You don't have permission to do that";
		return;
	}

	actionstream<<ctx->player->getName()
			<<" clears all objects"<<std::endl;

	{
		std::wstring msg;
		msg += L"Clearing all objects. This may take long.";
		msg += L" You may experience a timeout. (by ";
		msg += narrow_to_wide(ctx->player->getName());
		msg += L")";
		ctx->server->notifyPlayers(msg);
	}

	ctx->env->clearAllObjects();

	actionstream<<"object clearing done"<<std::endl;

	os<<L"*** cleared all objects";
	ctx->flags |= SEND_TO_OTHERS;
}

void cmd_setpassword(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	if ((ctx->privs & PRIV_SERVER) ==0) {
		os<<L"-!- You don't have permission to do that";
		return;
	}

	if (ctx->parms.size() != 3) {
		os<<L"-!- /setpassword <PLAYERNAME> <PASSWORD>";
		return;
	}

	Player *player = ctx->env->getPlayer(wide_to_narrow(ctx->parms[1]).c_str());
	if (player == NULL) {
		os<<L"-!- No such player";
		return;
	}

	std::string name = wide_to_narrow(ctx->parms[1]);
	std::string pass = translatePassword(name,ctx->parms[2]);
	ctx->server->setPlayerPassword(name.c_str(), pass.c_str());

	os<<L"-!- Password set for player '";
	os<<ctx->parms[1];
	os<<L"'";
}

void cmd_help(std::wostringstream &os,
	ServerCommandContext *ctx)
{
	u64 privs = ctx->privs;

	if (ctx->parms.size() > 1) {
		if ((privs&PRIV_SERVER) == PRIV_SERVER) {
			if (ctx->parms[1] == L"shutdown") {
				os<<L"-!- /#shutdown - shutdown the server";
				return;
			}else if (ctx->parms[1] == L"clearobjects") {
				os<<L"-!- /clearobjects - remove active objects from the world, may take a long time";
				return;
			}else if (ctx->parms[1] == L"setting") {
				os<<L"-!- /#setting <SETTING> [= VALUE] - change or view a config setting";
				return;
			}else if (ctx->parms[1] == L"setpassword") {
				os<<L"-!- /setpassword <PLAYERNAME> <PASSWORD> - change or set a player's password";
				return;
			}
		}
		if ((privs&PRIV_SETTIME) == PRIV_SETTIME && ctx->parms[1] == L"time") {
			os<<L"-!- /time <0-23999> - set the game time";
			return;
		}
		if ((privs&PRIV_TELEPORT) == PRIV_TELEPORT && ctx->parms[1] == L"teleport") {
			os<<L"-!- /teleport <X>,<Y>,<Z> - teleport to coordinates <X>,<Y>,<Z>";
			return;
		}
		if ((privs&PRIV_PRIVS) == PRIV_PRIVS) {
			if (ctx->parms[1] == L"grant") {
				os<<L"-!- /grant <PLAYERNAME> <PRIVILEGE> - grant a new privilege to a player";
				return;
			}else if (ctx->parms[1] == L"revoke") {
				os<<L"-!- /revoke <PLAYERNAME> <PRIVILEGE> - remove privilege from a player";
				return;
			}
		}
		if ((privs&PRIV_BAN) == PRIV_BAN) {
			if (ctx->parms[1] == L"ban") {
				os<<L"-!- /ban <PLAYERNAME OR IP ADDRESS> - ban a player from the server";
				return;
			}else if (ctx->parms[1] == L"unban") {
				os<<L"-!- /unban <PLAYERNAME OR IP ADDRESS> - remove a player's ban from the server";
				return;
			}
		}
		if (ctx->parms[1] == L"privs") {
			if ((privs&PRIV_PRIVS) == PRIV_PRIVS) {
				os<<L"-!- /privs [PLAYERNAME] - view the server privileges of a player";
				return;
			}
			os<<L"-!- /privs - view your server privileges";
			return;
		}
		if (ctx->parms[1] == L"status") {
			os<<L"-!- /status - view the server's status and welcome message";
			return;
		}
		if (ctx->parms[1] == L"help") {
			os<<L"-!- /help [COMMAND] - get help for a command or a list of commands";
			return;
		}
	}

	os<<L"-!- Available commands: ";
	os<<L"help status privs";
	if ((privs&PRIV_SERVER) == PRIV_SERVER)
		os<<L" shutdown setting clearobjects setpassword";
	if ((privs&PRIV_SETTIME) == PRIV_SETTIME)
		os<<L" time";
	if ((privs&PRIV_TELEPORT) == PRIV_TELEPORT)
		os<<L" teleport";
	if ((privs&PRIV_PRIVS) == PRIV_PRIVS)
		os<<L" grant revoke";
	if ((privs&PRIV_BAN) == PRIV_BAN)
		os<<L" ban unban";
}


std::wstring processServerCommand(ServerCommandContext *ctx)
{

	std::wostringstream os(std::ios_base::binary);
	ctx->flags = SEND_TO_SENDER;	// Default, unless we change it.

	if (ctx->parms.size() == 0 || ctx->parms[0] == L"help") {
		cmd_help(os,ctx);
	}else if (ctx->parms[0] == L"status") {
		cmd_status(os, ctx);
	}else if(ctx->parms[0] == L"privs") {
		cmd_privs(os, ctx);
	}else if(ctx->parms[0] == L"grant" || ctx->parms[0] == L"revoke") {
		cmd_grantrevoke(os, ctx);
	}else if(ctx->parms[0] == L"time") {
		cmd_time(os, ctx);
	}else if(ctx->parms[0] == L"shutdown") {
		cmd_shutdown(os, ctx);
	}else if(ctx->parms[0] == L"setting") {
		cmd_setting(os, ctx);
	}else if(ctx->parms[0] == L"teleport") {
		cmd_teleport(os, ctx);
	}else if(ctx->parms[0] == L"ban" || ctx->parms[0] == L"unban") {
		cmd_banunban(os, ctx);
	}else if(ctx->parms[0] == L"me") {
		cmd_me(os, ctx);
	}else if(ctx->parms[0] == L"setpassword") {
		cmd_setpassword(os, ctx);
	}else if(ctx->parms[0] == L"clearobjects") {
		cmd_clearobjects(os, ctx);
	}else{
		os<<L"-!- Invalid command '" + ctx->parms[0] + L"', try /help";
	}

	return os.str();
}


