/*
Minetest-c55
Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "player.h"
#include "map.h"
#include "connection.h"
#include "constants.h"
#include "utility.h"
#ifndef SERVER
#include "common_irrlicht.h"
#include "mesh.h"
#endif
#include "settings.h"
#include "path.h"

Player::Player():
	touching_ground(false),
	in_water(false),
	in_water_stable(false),
	is_climbing(false),
	swimming_up(false),
	inventory_backup(NULL),
	craftresult_is_preview(true),
	hp(20),
	peer_id(PEER_ID_INEXISTENT),
	m_selected_item(0),
	m_pitch(0),
	m_yaw(0),
	m_speed(0,0,0),
	m_position(0,0,0),
	m_home(0,0,0),
	m_hashome(false)
{
	updateName("<not set>");
	resetInventory();
}

Player::~Player()
{
	delete inventory_backup;
}

void Player::wieldItem(u16 item)
{
	m_selected_item = item;
}

void Player::resetInventory()
{
	inventory.clear();
	inventory.addList("main", PLAYER_INVENTORY_SIZE);
	inventory.addList("craft", 9);
	inventory.addList("craftresult", 1);
}

// Y direction is ignored
void Player::accelerate(v3f target_speed, f32 max_increase)
{
	v3f d_wanted = target_speed - m_speed;
	d_wanted.Y = 0;
	f32 dl_wanted = d_wanted.getLength();
	f32 dl = dl_wanted;
	if(dl > max_increase)
		dl = max_increase;

	v3f d = d_wanted.normalize() * dl;

	m_speed.X += d.X;
	m_speed.Z += d.Z;
	//m_speed += d;

#if 0 // old code
	if(m_speed.X < target_speed.X - max_increase)
		m_speed.X += max_increase;
	else if(m_speed.X > target_speed.X + max_increase)
		m_speed.X -= max_increase;
	else if(m_speed.X < target_speed.X)
		m_speed.X = target_speed.X;
	else if(m_speed.X > target_speed.X)
		m_speed.X = target_speed.X;

	if(m_speed.Z < target_speed.Z - max_increase)
		m_speed.Z += max_increase;
	else if(m_speed.Z > target_speed.Z + max_increase)
		m_speed.Z -= max_increase;
	else if(m_speed.Z < target_speed.Z)
		m_speed.Z = target_speed.Z;
	else if(m_speed.Z > target_speed.Z)
		m_speed.Z = target_speed.Z;
#endif
}

void Player::serialize(std::ostream &os)
{
	// Utilize a Settings object for storing values
	Settings args;
	args.setS32("version", 1);
	args.set("name", m_name);
	//args.set("password", m_password);
	args.setFloat("pitch", m_pitch);
	args.setFloat("yaw", m_yaw);
	args.setV3F("position", m_position);
	args.setBool("craftresult_is_preview", craftresult_is_preview);
	args.setS32("hp", hp);
	if (m_hashome)
		args.setV3F("home",m_home);

	args.writeLines(os);

	os<<"PlayerArgsEnd\n";

	// If actual inventory is backed up due to creative mode, save it
	// instead of the dummy creative mode inventory
	if(inventory_backup)
		inventory_backup->serialize(os);
	else
		inventory.serialize(os);
}

void Player::deSerialize(std::istream &is)
{
	Settings args;

	for(;;)
	{
		if(is.eof())
			return;
		std::string line;
		std::getline(is, line);
		std::string trimmedline = trim(line);
		if(trimmedline == "PlayerArgsEnd")
			break;
		args.parseConfigLine(line);
	}

	//args.getS32("version");
	std::string name = args.get("name");
	updateName(name.c_str());
	/*std::string password = "";
	if(args.exists("password"))
		password = args.get("password");
	updatePassword(password.c_str());*/
	m_pitch = args.getFloat("pitch");
	m_yaw = args.getFloat("yaw");
	m_position = args.getV3F("position");
	try{
		craftresult_is_preview = args.getBool("craftresult_is_preview");
	}catch(SettingNotFoundException &e){
		craftresult_is_preview = true;
	}
	try{
		hp = args.getS32("hp");
	}catch(SettingNotFoundException &e){
		hp = 20;
	}
	try{
		m_home = args.getV3F("home");
		m_hashome = true;
	}catch(SettingNotFoundException &e){}
	/*try{
		std::string sprivs = args.get("privs");
		if(sprivs == "all")
		{
			privs = PRIV_ALL;
		}
		else
		{
			std::istringstream ss(sprivs);
			ss>>privs;
		}
	}catch(SettingNotFoundException &e){
		privs = PRIV_DEFAULT;
	}*/

	inventory.deSerialize(is);
}

bool Player::getHome(v3f &h)
{
	if (!m_hashome)
		return false;
	h = m_home;
	return true;
}

void Player::setHome(v3f h)
{
	m_home = h;
	m_hashome = true;
}

/*
	RemotePlayer
*/

#ifndef SERVER

RemotePlayer::RemotePlayer(
		scene::ISceneNode* parent,
		IrrlichtDevice *device,
		s32 id):
	scene::ISceneNode(parent, (device==NULL)?NULL:device->getSceneManager(), id),
	m_node(NULL),
	m_text(NULL),
	m_anim_id(PLAYERANIM_STAND)
{
	m_box = core::aabbox3d<f32>(-BS/2,0,-BS/2,BS/2,BS*2,BS/2);

	if(parent != NULL && device != NULL)
	{
		// ISceneNode stores a member called SceneManager
		scene::ISceneManager* mgr = SceneManager;
		video::IVideoDriver* driver = mgr->getVideoDriver();
		gui::IGUIEnvironment* gui = device->getGUIEnvironment();

		// Add a text node for showing the name
		wchar_t wname[1] = {0};
		m_text = mgr->addTextSceneNode(gui->getBuiltInFont(),
				wname, video::SColor(255,255,255,255), this);
		if (!m_text)
			return;
		m_text->setPosition(v3f(0, (f32)BS*2.1, 0));

		// get a unique mesh so that the player model has it's own lighting
		scene::IAnimatedMesh* mesh = createModelMesh(mgr,"character.b3d",true);
		if (!mesh)
			return;

		m_node = mgr->addAnimatedMeshSceneNode(mesh,this);

		if (m_node) {
			m_node->setFrameLoop(0,79);
			m_node->setScale(v3f(1.0,1.0,1.0));
			setMeshColor(m_node->getMesh(), video::SColor(255,255,255,255));

			// Set material flags and texture
			m_node->setMaterialTexture( 0, driver->getTexture(getTexturePath("character.png").c_str()));
			video::SMaterial& material = m_node->getMaterial(0);
			material.setFlag(video::EMF_LIGHTING, false);
			material.setFlag(video::EMF_BILINEAR_FILTER, false);
			material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

			m_node->setPosition(v3f(0,(f32)BS,0));
		}
#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
		mesh->drop();
#endif
	}
}

RemotePlayer::~RemotePlayer()
{
	if(SceneManager != NULL) {
		ISceneNode::remove();
	}
}

void RemotePlayer::updateName(const char *name)
{
	Player::updateName(name);
	if (m_text != NULL) {
		wchar_t wname[PLAYERNAME_SIZE];
		mbstowcs(wname, m_name, strlen(m_name)+1);
		m_text->setText(wname);
	}
	if (m_node != NULL && !isLocal()) {
		std::string tex = std::string("player_") + m_name + ".png";
		std::string ptex = getPath("player",tex,true);
		printf("'%s' '%s'\n",tex.c_str(),ptex.c_str());
		if (ptex == "")
			return;
		// Set material flags and texture
		scene::ISceneManager* mgr = SceneManager;
		video::IVideoDriver* driver = mgr->getVideoDriver();
		m_node->setMaterialTexture( 0, driver->getTexture(ptex.c_str()));
		video::SMaterial& material = m_node->getMaterial(0);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_BILINEAR_FILTER, false);
	}
}

void RemotePlayer::move(f32 dtime, Map &map, f32 pos_max_d)
{
	m_pos_animation_time_counter += dtime;
	m_pos_animation_counter += dtime;
	v3f movevector = m_position - m_oldpos;
	f32 moveratio;
	if(m_pos_animation_time < 0.001)
		moveratio = 1.0;
	else
		moveratio = m_pos_animation_counter / m_pos_animation_time;
	if(moveratio > 1.5)
		moveratio = 1.5;
	m_showpos = m_oldpos + movevector * moveratio;

	if (
		(
			movevector.X < 0.001
			&& movevector.X > -0.001
		) || (
			movevector.Z < 0.001
			&& movevector.Z > -0.001
		)
	) {
		if (m_anim_id == PLAYERANIM_DIG) {
			if (m_node->getEndFrame() != 198)
				m_node->setFrameLoop(189,198);
		}else if (m_node->getEndFrame() != 79) {
			m_node->setFrameLoop(0,79);
		}
	}else{
		if (m_anim_id == PLAYERANIM_DIG) {
			if (m_node->getEndFrame() != 219)
				m_node->setFrameLoop(200,219);
		}else if (m_node->getEndFrame() != 187) {
			m_node->setFrameLoop(168,187);
		}
	}

	ISceneNode::setPosition(m_showpos-m_camera_offset);
}

#endif

#ifndef SERVER
/*
	LocalPlayer
*/

LocalPlayer::LocalPlayer():
	m_sneak_node(32767,32767,32767),
	m_sneak_node_exists(false)
{
	// Initialize hp to 0, so that no hearts will be shown if server
	// doesn't support health points
	hp = 0;
}

LocalPlayer::~LocalPlayer()
{
}

void LocalPlayer::move(f32 dtime, Map &map, f32 pos_max_d,
                core::list<CollisionInfo> *collision_info)
{
	v3f position = getPosition();
	v3f oldpos = position;
	v3s16 oldpos_i = floatToInt(oldpos, BS);

	v3f old_speed = m_speed;

	/*std::cout<<"oldpos_i=("<<oldpos_i.X<<","<<oldpos_i.Y<<","
			<<oldpos_i.Z<<")"<<std::endl;*/

	/*
		Calculate new position
	*/

	// Skip collision detection if a special movement mode is used
	bool free_move = g_settings->getBool("free_move");
	if (free_move) {
		position += m_speed * dtime;
		setPosition(position);
		return;
	}

	/*
		Collision detection
	*/

	// Player position in nodes
	v3s16 pos_i = floatToInt(position, BS);

	/*
		Check if player is in water (the oscillating value)
	*/
	try{
		// If in water, the threshold of coming out is at higher y
		if (in_water) {
			v3s16 pp = floatToInt(position + v3f(0,BS*0.1,0), BS);
			in_water = content_liquid(map.getNode(pp).getContent());
		// If not in water, the threshold of going in is at lower y
		}else{
			v3s16 pp = floatToInt(position + v3f(0,BS*0.5,0), BS);
			in_water = content_liquid(map.getNode(pp).getContent());
		}
	}catch(InvalidPositionException &e) {
		in_water = false;
	}

	/*
		Check if player is in water (the stable value)
	*/
	try{
		v3s16 pp = floatToInt(position + v3f(0,0,0), BS);
		in_water_stable = content_liquid(map.getNode(pp).getContent());
	}catch(InvalidPositionException &e) {
		in_water_stable = false;
	}

	/*
	        Check if player is climbing
	*/

	try{
	        v3s16 pp = floatToInt(position + v3f(0,0.5*BS,0), BS);
		v3s16 pp2 = floatToInt(position + v3f(0,-0.2*BS,0), BS);
		is_climbing = ((content_features(map.getNode(pp).getContent()).climbable ||
				content_features(map.getNode(pp2).getContent()).climbable) && !free_move);
	}catch(InvalidPositionException &e) {
	        is_climbing = false;
	}

	/*
		Collision uncertainty radius
		Make it a bit larger than the maximum distance of movement
	*/
	//f32 d = pos_max_d * 1.1;
	// A fairly large value in here makes moving smoother
	f32 d = 0.15*BS;

	// This should always apply, otherwise there are glitches
	assert(d > pos_max_d);

	float player_radius = BS*0.35;
	float player_height = BS*1.7;

	// Maximum distance over border for sneaking
	f32 sneak_max = BS*0.4;

	if (control.sneak && m_sneak_node_exists) {
		f32 maxd = 0.5*BS + sneak_max;
		v3f lwn_f = intToFloat(m_sneak_node, BS);
		position.X = rangelim(position.X, lwn_f.X-maxd, lwn_f.X+maxd);
		position.Z = rangelim(position.Z, lwn_f.Z-maxd, lwn_f.Z+maxd);

		f32 min_y = lwn_f.Y + 0.5*BS;
		if (position.Y < min_y) {
			position.Y = min_y;

			if(m_speed.Y < 0)
				m_speed.Y = 0;
		}
	}

	/*
		Calculate player collision box (new and old)
	*/

	core::aabbox3d<f32> playerbox(
		-player_radius,
		0.0,
		-player_radius,
		player_radius,
		player_height,
		player_radius
	);

	float player_stepheight = touching_ground ? (BS*0.6) : (BS*0.2);

	v3f accel_f = v3f(0,0,0);

	collisionMoveResult result = collisionMoveSimple(&map,
		pos_max_d, playerbox,
		player_stepheight, dtime,
		position,
		m_speed,
		accel_f);

	/*
		If the player's feet touch the topside of any node, this is
		set to true.

		Player is allowed to jump when this is true.
	*/
	touching_ground = result.touching_ground;

	/*
		Check the nodes under the player to see from which node the
		player is sneaking from, if any.  If the node from under
		the player has been removed, the player falls.
	*/
	v3s16 current_node = floatToInt(position - v3f(0,BS/2,0), BS);
	if (m_sneak_node_exists && map.getNodeNoEx(m_old_node_below).getContent() == CONTENT_AIR && m_old_node_below_type != CONTENT_AIR) {
		// Old node appears to have been removed; that is,
		// it wasn't air before but now it is
		m_refresh_sneak_node = false;
		m_sneak_node_exists = false;
	}else if (map.getNodeNoEx(current_node).getContent() != CONTENT_AIR) {
		// We are on something, so make sure to recalculate the sneak
		// node.
		m_refresh_sneak_node = true;
	}

	if (m_refresh_sneak_node) {
		v3s16 pos_i_bottom = floatToInt(position - v3f(0,BS/2,0), BS);
		v2f player_p2df(position.X, position.Z);
		f32 min_distance_f = 100000.0*BS;

		v3s16 new_sneak_node = m_sneak_node;
		for (s16 x=-1; x<=1; x++)
		for (s16 z=-1; z<=1; z++)
		{
			v3s16 p = pos_i_bottom + v3s16(x,0,z);
			v3f pf = intToFloat(p, BS);
			v2f node_p2df(pf.X, pf.Z);
			f32 distance_f = player_p2df.getDistanceFrom(node_p2df);
			f32 max_axis_distance_f = MYMAX(
			fabs(player_p2df.X-node_p2df.X),
			fabs(player_p2df.Y-node_p2df.Y));

			if (distance_f > min_distance_f || max_axis_distance_f > 0.5*BS + sneak_max + 0.1*BS)
				continue;

			try{
				// The node to be sneaked on has to be walkable
				if (content_features(map.getNode(p).getContent()).walkable == false)
					continue;
				// And the node above it has to be nonwalkable
				if (content_features(map.getNode(p+v3s16(0,1,0)).getContent()).walkable == true)
					continue;
			}catch(InvalidPositionException &e) {
				continue;
			}

			min_distance_f = distance_f;
			new_sneak_node = p;
		}

		bool sneak_node_found = (min_distance_f < 100000.0*BS*0.9);

		m_sneak_node = new_sneak_node;
		m_sneak_node_exists = sneak_node_found;

		/*
			If sneaking, the player's collision box can be in air, so
			this has to be set explicitly
		*/
		if (sneak_node_found && control.sneak)
			touching_ground = true;
	}

	/*
		Set new position
	*/
	setPosition(position);

	/*
		Report collisions
	*/
	if(collision_info)
	{
		// Report fall collision
		if(old_speed.Y < m_speed.Y - 0.1 && !result.standing_on_unloaded)
		{
			CollisionInfo info;
			info.t = COLLISION_FALL;
			info.speed = m_speed.Y - old_speed.Y;
			collision_info->push_back(info);
		}
	}
}

void LocalPlayer::move(f32 dtime, Map &map, f32 pos_max_d)
{
	move(dtime, map, pos_max_d, NULL);
}

void LocalPlayer::applyControl(float dtime)
{
	// Clear stuff
	swimming_up = false;

	// Random constants
	f32 walk_acceleration = 4.0 * BS;
	f32 walkspeed_max = 4.0 * BS;

	setPitch(control.pitch);
	setYaw(control.yaw);

	v3f move_direction = v3f(0,0,1);
	move_direction.rotateXZBy(getYaw());

	v3f speed = v3f(0,0,0);

	bool free_move = g_settings->getBool("free_move");
	bool fast_move = g_settings->getBool("fast_move");
	bool continuous_forward = g_settings->getBool("continuous_forward");

	if(free_move || is_climbing)
	{
		v3f speed = getSpeed();
		speed.Y = 0;
		setSpeed(speed);
	}

	// Whether superspeed mode is used or not
	bool superspeed = false;

	// If free movement and fast movement, always move fast
	if(free_move && fast_move)
		superspeed = true;

	// Auxiliary button 1 (E)
	if(control.aux1)
	{
		if(free_move)
		{
			// In free movement mode, aux1 descends
			v3f speed = getSpeed();
			if(fast_move)
				speed.Y = -20*BS;
			else
				speed.Y = -walkspeed_max;
			setSpeed(speed);
		}
		else if(is_climbing)
		{
		        v3f speed = getSpeed();
			speed.Y = -3*BS;
			setSpeed(speed);
		}
		else
		{
			// If not free movement but fast is allowed, aux1 is
			// "Turbo button"
			if(fast_move)
				superspeed = true;
		}
	}

	if(continuous_forward)
		speed += move_direction;

	if(control.up)
	{
		if(continuous_forward)
			superspeed = true;
		else
			speed += move_direction;
	}
	if(control.down)
	{
		speed -= move_direction;
	}
	if(control.left)
	{
		speed += move_direction.crossProduct(v3f(0,1,0));
	}
	if(control.right)
	{
		speed += move_direction.crossProduct(v3f(0,-1,0));
	}
	if(control.jump)
	{
		if(free_move)
		{
			v3f speed = getSpeed();
			if(fast_move)
				speed.Y = 20*BS;
			else
				speed.Y = walkspeed_max;
			setSpeed(speed);
		}
		else if(touching_ground)
		{
			v3f speed = getSpeed();
			/*
				NOTE: The d value in move() affects jump height by
				raising the height at which the jump speed is kept
				at its starting value
			*/
			speed.Y = 6.5*BS;
			setSpeed(speed);
		}
		// Use the oscillating value for getting out of water
		// (so that the player doesn't fly on the surface)
		else if(in_water)
		{
			v3f speed = getSpeed();
			speed.Y = 1.5*BS;
			setSpeed(speed);
			swimming_up = true;
		}
		else if(is_climbing)
		{
	                v3f speed = getSpeed();
			speed.Y = 3*BS;
			setSpeed(speed);
		}
	}

	// The speed of the player (Y is ignored)
	if(superspeed)
		speed = speed.normalize() * walkspeed_max * 5.0;
	else if(control.sneak)
		speed = speed.normalize() * walkspeed_max / 3.0;
	else
		speed = speed.normalize() * walkspeed_max;

	f32 inc = walk_acceleration * BS * dtime;

	// Faster acceleration if fast and free movement
	if(free_move && fast_move)
		inc = walk_acceleration * BS * dtime * 10;

	// Accelerate to target speed with maximum increment
	accelerate(speed, inc);
}
#endif

