/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* player.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2013-2014 <lisa@ltmnet.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>
*
* License updated from GPLv2 or later to GPLv3 or later by Lisa Milne
* for Voxelands.
************************************************************************/

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
#include "content_clothesitem.h"

/* character def:
gender:Yscale:XZscale:skintone:eyes:hairtone:hair:face:shirt-colour:pants-colour:shoe-type
the old default:
M:10:10:fair:blue:brown:medium:normal:green:blue:leather
*/

Player::Player():
	touching_ground(false),
	in_water(false),
	in_water_stable(false),
	is_climbing(false),
	swimming_up(false),
	in_bed(false),
	wake_timeout(0.0),
	craftresult_is_preview(true),
	hp(20),
	air(20),
	hunger(20),
	energy_effect(0),
	cold_effect(0),
	peer_id(PEER_ID_INEXISTENT),
	m_selected_item(0),
	m_pitch(0),
	m_yaw(0),
	m_speed(0,0,0),
	m_position(0,0,0),
	m_home(0,0,0),
	m_hashome(false),
	m_character(PLAYER_DEFAULT_CHARDEF),
	m_given_clothes(false)
{
	updateName("<not set>");
	for (u8 i=0; i<PLAYERFLAG_COUNT; i++) {
		m_hasflag[i] = false;
	}
	resetInventory();
}

Player::~Player()
{
}

void Player::wieldItem(u16 item)
{
	m_selected_item = item;
}

void Player::resetInventory(bool include_clothes)
{
	if (include_clothes) {
		inventory.clear();
	}else{
		InventoryList *l = inventory.getList("main");
		if (l)
			l->clearItems();
		l = inventory.getList("discard");
		if (l)
			l->clearItems();
		l = inventory.getList("craft");
		if (l)
			l->clearItems();
		l = inventory.getList("craftresult");
		if (l)
			l->clearItems();
	}
	checkInventory();
}

void Player::checkInventory()
{
	if (!inventory.getList("main"))
		inventory.addList("main", PLAYER_INVENTORY_SIZE);
	if (!inventory.getList("hat"))
		inventory.addList("hat",1);
	if (!inventory.getList("jacket"))
		inventory.addList("jacket",1);
	if (!inventory.getList("shirt"))
		inventory.addList("shirt",1);
	if (!inventory.getList("decorative"))
		inventory.addList("decorative",1);
	if (!inventory.getList("belt"))
		inventory.addList("belt",1);
	if (!inventory.getList("pants"))
		inventory.addList("pants",1);
	if (!inventory.getList("boots"))
		inventory.addList("boots",1);
	// this allows only the correct clothing type in a player's
	// relevant clothing slot
	{
		InventoryList *h = inventory.getList("hat");
		InventoryList *j = inventory.getList("jacket");
		InventoryList *s = inventory.getList("shirt");
		InventoryList *d = inventory.getList("decorative");
		InventoryList *t = inventory.getList("belt");
		InventoryList *p = inventory.getList("pants");
		InventoryList *b = inventory.getList("boots");
		h->setStackable(false);
		h->clearAllowed();
		j->setStackable(false);
		j->clearAllowed();
		s->setStackable(false);
		s->clearAllowed();
		d->setStackable(false);
		d->clearAllowed();
		t->setStackable(false);
		t->clearAllowed();
		p->setStackable(false);
		p->clearAllowed();
		b->setStackable(false);
		b->clearAllowed();
		for (
			std::map<content_t,struct ClothesItemFeatures>::iterator i = g_content_clothesitem_features.begin();
			i != g_content_clothesitem_features.end();
			i++
		) {
			ClothesItemFeatures c = i->second;
			switch (c.type) {
			case CT_HAT:
				h->addAllowed(c.content);
				break;
			case CT_JACKET:
				j->addAllowed(c.content);
				break;
			case CT_SHIRT:
				s->addAllowed(c.content);
				break;
			case CT_DECORATIVE:
			case CT_MEDALLION:
				d->addAllowed(c.content);
				break;
			case CT_BELT:
				t->addAllowed(c.content);
				break;
			case CT_PANTS:
				p->addAllowed(c.content);
				break;
			case CT_BOOTS:
				b->addAllowed(c.content);
				break;
			default:;
			}
		}
	}
	if (!inventory.getList("discard"))
		inventory.addList("discard",1);
	if (!inventory.getList("craft"))
		inventory.addList("craft", 9);
	if (!inventory.getList("craftresult"))
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
}

void Player::serialize(std::ostream &os)
{
	// Utilize a Settings object for storing values
	Settings args;
	args.setS32("version", 1);
	args.set("name", m_name);
	args.setFloat("pitch", m_pitch);
	args.setFloat("yaw", m_yaw);
	args.setV3F("position", m_position);
	args.setBool("craftresult_is_preview", craftresult_is_preview);
	args.setS32("hp", hp);
	args.setS32("air", air);
	args.setS32("hunger", hunger);
	if (m_hashome)
		args.setV3F("home",m_home);
	const char* flags[8] = {"white","blue","green","orange","purple","red","yellow","black"};
	for (u16 i=0; i<8; i++) {
		std::string n("flag_");
		n += flags[i];
		if (!m_hasflag[i])
			continue;
		args.setV3F(n,m_flag[i]);
	}
	if (m_given_clothes)
		args.set("clothes_given","true");
	args.setFloat("wake_timeout",wake_timeout);

	args.writeLines(os);

	os<<"PlayerArgsEnd\n";

	inventory.serialize(os);
}

void Player::deSerialize(std::istream &is)
{
	Settings args;

	for (;;) {
		if (is.eof())
			return;
		std::string line;
		std::getline(is, line);
		std::string trimmedline = trim(line);
		if (trimmedline == "PlayerArgsEnd")
			break;
		args.parseConfigLine(line);
	}

	//args.getS32("version");
	std::string name = args.get("name");
	updateName(name.c_str());
	m_pitch = args.getFloat("pitch");
	m_yaw = args.getFloat("yaw");
	m_position = args.getV3F("position");
	if (args.exists("craftresult_is_preview")) {
		craftresult_is_preview = args.getBool("craftresult_is_preview");
	}else{
		craftresult_is_preview = true;
	}
	if (args.exists("hp")) {
		hp = args.getS32("hp");
	}else{
		hp = 20;
	}
	if (args.exists("air")) {
		air = args.getS32("air");
	}else{
		air = 20;
	}
	if (args.exists("hunger")) {
		hunger = args.getS32("hunger");
	}else{
		hunger = 20;
	}
	if (args.exists("home")) {
		m_home = args.getV3F("home");
		m_hashome = true;
	}else{
		m_hashome = false;
	}
	const char* flags[8] = {"white","blue","green","orange","purple","red","yellow","black"};
	for (u16 i=0; i<8; i++) {
		std::string n("flag_");
		n += flags[i];
		m_hasflag[i] = args.exists(n);
		if (!m_hasflag[i])
			continue;
		m_flag[i] = args.getV3F(n);
		if (!m_hashome) {
			m_home = m_flag[i];
			m_hashome = true;
		}
	}
	if (args.exists("clothes_given")) {
		m_given_clothes = args.getBool("clothes_given");
	}else{
		m_given_clothes = false;
	}

	if (args.exists("wake_timeout")) {
		wake_timeout = args.getFloat("wake_timeout");
	}else{
		wake_timeout = 0.0;
	}

	inventory.deSerialize(is);
	checkInventory();
}

bool Player::getHome(s8 i, v3f &h)
{
	if (i == PLAYERFLAG_HOME) {
		if (!m_hashome)
			return false;
		h = m_home;
		return true;
	}else if (i > PLAYERFLAG_HOME && i < PLAYERFLAG_COUNT) {
		if (!m_hasflag[i])
			return false;
		h = m_flag[i];
		return true;
	}
	return false;
}

void Player::setHome(s8 i, v3f h)
{
	if (i > PLAYERFLAG_HOME && i < PLAYERFLAG_COUNT) {
		m_flag[i] = h;
		m_hasflag[i] = true;
	}
	m_home = h;
	m_hashome = true;
}

void Player::unsetHome(s8 i)
{
	bool reset = false;
	if (i == PLAYERFLAG_HOME) {
		reset = true;
	}else if (i > PLAYERFLAG_HOME && i < PLAYERFLAG_COUNT) {
		if (m_hashome && m_hasflag[i] && m_home == m_flag[i])
			reset = true;
		m_hasflag[i] = false;
	}
	if (!reset)
		return;

	m_hashome = false;

	for (u8 i=0; i<PLAYERFLAG_COUNT; i++) {
		if (!m_hasflag[i])
			continue;
		m_hashome = true;
		m_home = m_flag[i];
	}
}

v3f Player::getScale()
{
	if (m_character == "")
		m_character = std::string(PLAYER_DEFAULT_CHARDEF);
	Strfnd f(m_character);

	std::string gender = f.next(":");
	std::string y = f.next(":"); // y scale
	std::string xz = f.next(":"); // xz scale

	f32 y_f = mystof(y)/10.0;
	if (y_f < 0.8)
		y_f = 0.8;
	if (y_f > 1.1)
		y_f = 1.1;
	f32 xz_f = mystof(xz)/10.0;
	if (xz_f < 0.8)
		xz_f = 0.8;
	if (xz_f > 1.1)
		xz_f = 1.1;

	return v3f(xz_f,y_f,xz_f);
}

//gender:Yscale:XZscale:skintone:eyes:hairtone:hair:face:shirt-colour:pants-colour:shoe-type
//M:10:10:fair:blue:brown:medium:human:green:blue:leather
void Player::getSkin(std::vector<std::string> &parts)
{
	if (m_character == "")
		m_character = std::string(PLAYER_DEFAULT_CHARDEF);
	Strfnd f(m_character);

	std::string gender = f.next(":");
	f.next(":"); // y scale
	f.next(":"); // xz scale
	std::string skintone = f.next(":");
	std::string eyes = f.next(":");
	std::string hairtone = f.next(":");
	std::string hair = f.next(":");
	std::string face = f.next(":");

	if (gender != "M" && gender != "F")
		gender = "M";
	if (getPath("skin",std::string("skintone_")+skintone+"_"+gender+".png",true) == "")
		skintone = "fair";
	if (getPath("skin",std::string("eyes_")+eyes+".png",true) == "")
		eyes = "blue";
	if (hairtone == "" || hair == "" || getPath("skin",std::string("hair_")+hair+"_"+hairtone+"_"+gender+".png",true) == "") {
		hairtone = "brown";
		hair = "medium";
	}
	if (getPath("skin",std::string("face_")+face+gender+".png",true) == "")
		face = "human";

	//parts.push_back(std::string("skins")+DIR_DELIM+"skintone_"+skintone+"_"+gender+".png");

	parts.push_back(std::string("skins")+DIR_DELIM+"skintone_"+skintone+".png");
	parts.push_back(std::string("skins")+DIR_DELIM+"gender_"+gender+".png");
	parts.push_back(std::string("skins")+DIR_DELIM+"face_"+face+"_"+skintone+"_"+gender+".png");
	parts.push_back(std::string("skins")+DIR_DELIM+"eyes_"+eyes+".png");
	parts.push_back(std::string("skins")+DIR_DELIM+"hair_"+hair+"_"+hairtone+"_"+gender+".png");
}

/*
	ServerRemotePlayer
*/

//gender:Yscale:XZscale:skintone:eyes:hairtone:hair:face:shirt-colour:pants-colour:shoe-type
//M:10:10:fair:blue:brown:medium:human:green:blue:leather
void ServerRemotePlayer::setCharDef(std::string d)
{
	m_character = d;
	if (m_given_clothes)
		return;
	if (m_character == "")
		m_character = std::string(PLAYER_DEFAULT_CHARDEF);
	Strfnd f(m_character);

	f.next(":"); // gender
	f.next(":"); // y scale
	f.next(":"); // xz scale
	f.next(":"); // skintone
	f.next(":"); // eyes
	f.next(":"); // hairtone
	f.next(":"); // hair
	f.next(":"); // face
	std::string shirt = f.next(":");
	std::string pants = f.next(":");
	std::string shoes = f.next(":");

	{
		InventoryList *l = inventory.getList("shirt");
		if (!l)
			return;
		InventoryItem *item = NULL;
		if (shirt == "white") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT,0,0);
		}else if (shirt == "blue") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_BLUE,0,0);
		}else if (shirt == "orange") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_ORANGE,0,0);
		}else if (shirt == "purple") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_PURPLE,0,0);
		}else if (shirt == "red") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_RED,0,0);
		}else if (shirt == "yellow") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_YELLOW,0,0);
		}else if (shirt == "black") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_BLACK,0,0);
		}
		if (item == NULL)
			item = new ClothesItem(CONTENT_CLOTHESITEM_COTTON_TSHIRT_GREEN,0,0);
		l->addItem(0,item);
	}
	{
		InventoryList *l = inventory.getList("pants");
		if (!l)
			return;
		InventoryItem *item = NULL;
		if (pants == "white") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS,0,0);
		}else if (pants == "green") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_GREEN,0,0);
		}else if (pants == "orange") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_ORANGE,0,0);
		}else if (pants == "purple") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_PURPLE,0,0);
		}else if (pants == "red") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_RED,0,0);
		}else if (pants == "yellow") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_YELLOW,0,0);
		}else if (pants == "black") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_BLACK,0,0);
		}
		if (item == NULL)
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_PANTS_BLUE,0,0);
		l->addItem(0,item);
	}
	{
		InventoryList *l = inventory.getList("boots");
		if (!l)
			return;
		InventoryItem *item = NULL;
		if (shirt == "fur") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_FUR_SHOES,0,0);
		}else if (shirt == "canvas") {
			item = new ClothesItem(CONTENT_CLOTHESITEM_CANVAS_SHOES,0,0);
		}
		if (item == NULL)
			item = new ClothesItem(CONTENT_CLOTHESITEM_LEATHER_SHOES,0,0);
		l->addItem(0,item);
	}

	m_given_clothes = true;
}

/*
	RemotePlayer
*/

#ifndef SERVER

#include "sound.h"

RemotePlayer::RemotePlayer(
		scene::ISceneNode* parent,
		IrrlichtDevice *device,
		s32 id):
	scene::ISceneNode(parent, (device==NULL)?NULL:device->getSceneManager(), id),
	m_node(NULL),
	m_text(NULL),
	m_wield(NULL),
	m_anim_id(PLAYERANIM_STAND),
	m_pointed(CONTENT_IGNORE),
	m_next_foot(0)
{
	m_box = core::aabbox3d<f32>(-BS/2,0,-BS/2,BS/2,BS*2,BS/2);

	if (parent != NULL && device != NULL) {
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
			m_node->setScale(Player::getScale());
			setMeshColor(m_node->getMesh(), video::SColor(255,255,255,255));

			// Set material flags and texture
			m_node->setMaterialTexture(0, driver->getTexture(getTexturePath("character.png").c_str()));
			video::SMaterial& material = m_node->getMaterial(0);
			material.setFlag(video::EMF_LIGHTING, false);
			material.setFlag(video::EMF_BILINEAR_FILTER, false);
			material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

			m_node->setPosition(v3f(0,(f32)BS,0));
			scene::IBoneSceneNode *arm = m_node->getJointNode("Arm_Low_Right");
			if (arm)
				m_wield = new ExtrudedSpriteSceneNode(arm,mgr,-1,v3f(0,5,1),v3f(-45,-90,45),v3f(4,4,4));
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
	if (name != NULL) {
		Player::updateName(name);
		if (m_text != NULL) {
			wchar_t wname[PLAYERNAME_SIZE];
			mbstowcs(wname, m_name, strlen(m_name)+1);
			m_text->setText(wname);
		}
	}
	if (m_node != NULL && !isLocal()) {
		// Set material flags and texture
		m_node->setMaterialTexture( 0, getTexture());
		video::SMaterial& material = m_node->getMaterial(0);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_BILINEAR_FILTER, false);
	}
}

void RemotePlayer::wieldItem(u16 item)
{
	m_selected_item = item;
	if (!m_wield)
		return;
	const InventoryItem *i = getWieldItem();
	if (!i) {
		m_wield->setVisible(false);
		return;
	}
	bool haveWield = false;

	// Try to make a MaterialItem cube.
	if (std::string(i->getName()) == "MaterialItem") {
		// A block-type material
		MaterialItem* mat_item = (MaterialItem*)i;
		content_t content = mat_item->getMaterial();
		if (content_features(content).solidness || content_features(content).visual_solidness) {
			m_wield->setCube(content_features(content).tiles);
			haveWield = true;
		}else if (
			(
				content_features(content).draw_type == CDT_NODEBOX
				|| content_features(content).draw_type == CDT_NODEBOX_META
				|| content_features(content).draw_type == CDT_FENCELIKE
				|| content_features(content).draw_type == CDT_WALLLIKE
			)
			&& content_features(content).wield_nodebox == true
		) {
			m_wield->setNodeBox(content);
			haveWield = true;
		}
		m_wield->setScale(v3f(3,3,3));
	}else{
		m_wield->setScale(v3f(4.5,4.5,4.5));
	}

	// If that failed, make an extruded sprite.
	if (!haveWield) {
		m_wield->setSprite(i->getImageRaw());
	}

	m_wield->setVisible(true);
}

void RemotePlayer::move(f32 dtime, Map &map, f32 pos_max_d)
{
	m_pos_animation_time_counter += dtime;
	m_pos_animation_counter += dtime;
	v3f movevector = m_position - m_oldpos;
	f32 moveratio;
	if (m_pos_animation_time < 0.001) {
		moveratio = 1.0;
	}else{
		moveratio = m_pos_animation_counter / m_pos_animation_time;
	}
	if (moveratio > 1.5)
		moveratio = 1.5;
	m_showpos = m_oldpos + movevector * moveratio;

	int frame = m_node->getFrameNr();
	/* roughly sort of when a step sound should probably be heard, maybe */
	if (frame == 218 || frame == 186 || frame == 209 || frame == 177) {
		sound_playStep(&map,m_showpos,m_next_foot);
		m_next_foot = !m_next_foot;
	}
	/* roughly sort of when a dig sound should probably be heard, maybe */
	if (frame == 214 || frame == 205 || frame == 193) {
		sound_playDig(m_pointed,m_showpos);
	}

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
		if (m_anim_id == PLAYERANIM_DIG) { // walk/dig
			if (m_node->getEndFrame() != 219)
				m_node->setFrameLoop(200,219);
		}else if (m_node->getEndFrame() != 187) { // walk
			m_node->setFrameLoop(168,187);
		}
	}

	ISceneNode::setPosition(m_showpos-m_camera_offset);
}

video::ITexture* RemotePlayer::getTexture()
{
	std::string clothes[7];
	const char* list[7] = {"hat","shirt","jacket","decorative","belt","pants","boots"};
	std::vector<std::string> parts;
	getSkin(parts);
	for (int j=0; j<7; j++) {
		InventoryList *l = inventory.getList(list[j]);
		if (l == NULL)
			continue;
		InventoryItem *i = l->getItem(0);
		if (i == NULL)
			continue;
		clothes[j] = content_clothesitem_features(i->getContent()).overlay_texture;
	}

	std::string tex = "";

	tex += parts[0];         // skin
	tex += "^" + parts[1];   // underwear
	tex += "^" + parts[2];   // face
	tex += "^" + parts[3];   // eyes
	tex += "^" + clothes[5]; // pants
	tex += "^" + clothes[1]; // shirt
	tex += "^" + clothes[4]; // belt
	tex += "^" + clothes[3]; // deco
	tex += "^" + parts[4];   // hair
	tex += "^" + clothes[2]; // jacket
	tex += "^" + clothes[6]; // boots
	tex += "^" + clothes[0]; // hat

	return g_texturesource->getTextureRaw(tex);
}

#endif

#ifndef SERVER
/*
	LocalPlayer
*/

LocalPlayer::LocalPlayer():
	energy_effectf(0.0),
	cold_effectf(0.0),
	m_sneak_node(32767,32767,32767),
	m_sneak_node_exists(false),
	m_can_use_energy(true),
	m_ignore_energy(false),
	m_low_energy_effect(0)
{
	m_energy = 10.0;
	m_character = g_settings->get("character_definition");
}

LocalPlayer::~LocalPlayer()
{
}

void LocalPlayer::move(f32 dtime, Map &map, f32 pos_max_d,
		core::list<CollisionInfo> *collision_info)
{
	v3f position = getPosition();

	v3f old_speed = m_speed;

	/*
		Calculate new position
	*/

	// Skip collision detection if a special movement mode is used
	if (control.free) {
		position += m_speed * dtime;
		setPosition(position);
		return;
	}

	/*
		Check if player is in water (the oscillating value)
	*/
	try{
		// If in water, the threshold of coming out is at higher y
		if (in_water) {
			v3s16 pp = floatToInt(position + v3f(0,BS*0.1,0), BS);
			in_water = content_features(map.getNode(pp).getContent()).liquid_type != LIQUID_NONE;
		// If not in water, the threshold of going in is at lower y
		}else{
			v3s16 pp = floatToInt(position + v3f(0,BS*0.5,0), BS);
			in_water = content_features(map.getNode(pp).getContent()).liquid_type != LIQUID_NONE;
		}
	}catch(InvalidPositionException &e) {
		in_water = false;
	}

	/*
		Check if player is in water (the stable value)
	*/
	try{
		v3s16 pp = floatToInt(position + v3f(0,0,0), BS);
		in_water_stable = content_features(map.getNode(pp).getContent()).liquid_type != LIQUID_NONE;
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
				content_features(map.getNode(pp2).getContent()).climbable) && !control.free);
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

	if (m_ignore_energy) {
		m_can_use_energy = true;
		m_energy = 20.0;
	}

	// Random constants
	f32 walk_acceleration = 4.0 * BS;
	f32 walkspeed_max = 4.0 * BS;

	setPitch(control.pitch);
	setYaw(control.yaw);

	v3f move_direction = v3f(0,0,1);
	move_direction.rotateXZBy(getYaw());

	v3f speed = v3f(0,0,0);
	if (control.fast && !m_can_use_energy)
		control.fast = false;

	if (control.free || is_climbing) {
		v3f speed = getSpeed();
		speed.Y = 0;
		setSpeed(speed);
	}

	if (control.forward)
		speed += move_direction;
	if (control.backward)
		speed -= move_direction;
	if (control.left)
		speed += move_direction.crossProduct(v3f(0,1,0));
	if (control.right)
		speed += move_direction.crossProduct(v3f(0,-1,0));
	if (control.jump && !control.free && m_can_use_energy) {
		if (touching_ground) {
			v3f speed = getSpeed();
			/*
				NOTE: The d value in move() affects jump height by
				raising the height at which the jump speed is kept
				at its starting value
			*/
			if (speed.Y < 6*BS && !energy_effectf)
				m_energy -= 0.6;
			speed.Y = 6.5*BS;
			setSpeed(speed);
		}else if (in_water) {
			// Use the oscillating value for getting out of water
			// (so that the player doesn't fly on the surface)
			v3f speed = getSpeed();
			speed.Y = 0.8*BS;
			setSpeed(speed);
			swimming_up = true;
			if (!energy_effectf)
				m_energy -= dtime*0.8;
		}else if (in_water_stable && !energy_effectf) {
			m_energy -= dtime*0.8;
		}
	}
	if (control.up) {
		if (control.free) {
			v3f speed = getSpeed();
			if (control.fast) {
				speed.Y = 20*BS;
			}else{
				speed.Y = walkspeed_max;
			}
			setSpeed(speed);
		}else if (in_water) {
			// Use the oscillating value for getting out of water
			// (so that the player doesn't fly on the surface)
			if (m_can_use_energy) {
				v3f speed = getSpeed();
				speed.Y = 1.5*BS;
				setSpeed(speed);
				swimming_up = true;
				if (!energy_effectf)
					m_energy -= dtime;
			}
		}else if (is_climbing) {
	                v3f speed = getSpeed();
			speed.Y = 3*BS;
			setSpeed(speed);
		}else if (in_water_stable && m_can_use_energy && !energy_effectf) {
			m_energy -= dtime;
		}
	}
	if (control.down) {
		if (control.free) {
			// In free movement mode, aux1 descends
			v3f speed = getSpeed();
			if (control.fast) {
				speed.Y = -20*BS;
			}else{
				speed.Y = -walkspeed_max;
			}
			setSpeed(speed);
		}else if (is_climbing) {
		        v3f speed = getSpeed();
			speed.Y = -3*BS;
			setSpeed(speed);
		}else if (in_water) {
		        v3f speed = getSpeed();
			speed.Y = -5*BS;
			setSpeed(speed);
		}
	}

	// The speed of the player (Y is ignored)
	if (control.fast && m_can_use_energy) {
		if (!energy_effectf && (speed.X || speed.Y || speed.Z))
			m_energy -= dtime;
		if (control.free) {
			speed = speed.normalize() * walkspeed_max * 6.0;
		}else{
			speed = speed.normalize() * walkspeed_max * 2.0;
		}
	}else{
		if (energy_effectf) {
			if (m_energy < hp)
				m_energy += dtime*5.0;
		}else if (control.digging) {
			m_energy -= dtime*0.2;
		}else if (m_energy < hp) {
			if (speed.X || speed.Y || speed.Z) {
				m_energy += dtime*((float)hunger/30.0);
			}else{
				m_energy += dtime*((float)hunger/15.0);
			}
		}
		if (control.sneak) {
			speed = speed.normalize() * walkspeed_max / 3.0;
		}else{
			speed = speed.normalize() * walkspeed_max;
		}
	}
	if (m_energy > hp) {
		m_energy = hp;
	}else if (m_energy < -0.1) {
		m_can_use_energy = false;
		m_energy = -0.1;
		if (g_sound && m_low_energy_effect == 0) {
			if (m_character == "")
				m_character = std::string(PLAYER_DEFAULT_CHARDEF);
			Strfnd f(m_character);
			std::string gender = f.next(":");
			std::string snd("low-energy-");
			snd += gender;

			m_low_energy_effect = g_sound->playSound(snd,true);
		}
	}else if (m_energy > 1.8) {
		m_can_use_energy = true;
		if (g_sound && m_low_energy_effect) {
			g_sound->stopSound(m_low_energy_effect);
			m_low_energy_effect = 0;
		}
	}

	if (energy_effectf > 0.0)
		energy_effectf -= dtime;
	if (energy_effectf < 0.0)
		energy_effectf = 0.0;
	if (cold_effectf > 0.0)
		cold_effectf -= dtime;
	if (cold_effectf < 0.0)
		cold_effectf = 0.0;

	f32 inc = walk_acceleration * BS * dtime;

	// Faster acceleration if fast and free movement
	if (control.free && control.fast)
		inc = walk_acceleration * BS * dtime * 10;

	// Accelerate to target speed with maximum increment
	accelerate(speed, inc);
}

video::ITexture* LocalPlayer::getTexture()
{
	std::string clothes[7];
	const char* list[7] = {"hat","shirt","jacket","decorative","belt","pants","boots"};
	std::vector<std::string> parts;
	getSkin(parts);
	for (int j=0; j<7; j++) {
		InventoryList *l = inventory.getList(list[j]);
		if (l == NULL)
			continue;
		InventoryItem *i = l->getItem(0);
		if (i == NULL)
			continue;
		clothes[j] = content_clothesitem_features(i->getContent()).overlay_texture;
	}

	std::string tex = "";

	tex += parts[0];         // skin
	tex += "^" + parts[1];   // face
	tex += "^" + parts[2];   // eyes
	tex += "^" + clothes[5]; // pants
	tex += "^" + clothes[1]; // shirt
	tex += "^" + clothes[4]; // belt
	tex += "^" + clothes[3]; // deco
	tex += "^" + parts[3];   // hair
	tex += "^" + clothes[2]; // jacket
	tex += "^" + clothes[6]; // boots
	tex += "^" + clothes[0]; // hat

	return g_texturesource->getTextureRaw(tex);
}
#endif

