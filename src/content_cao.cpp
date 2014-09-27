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

#include "content_cao.h"
#include "path.h"
#include "environment.h"
#include "settings.h"
#include "mesh.h"
#include <ICameraSceneNode.h>

static void setBillboardTextureMatrix(scene::IBillboardSceneNode *bill,
	float txs, float tys, int col, int row)
{
	video::SMaterial& material = bill->getMaterial(0);
	core::matrix4& matrix = material.getTextureMatrix(0);
	matrix.setTextureTranslate(txs*col, tys*row);
	matrix.setTextureScale(txs, tys);
}

core::map<u16, ClientActiveObject::Factory> ClientActiveObject::m_types;

/*
	ItemCAO
*/

#include "inventory.h"

// Prototype
ItemCAO proto_ItemCAO;

ItemCAO::ItemCAO():
	ClientActiveObject(0),
	m_selection_box(-BS/3.,0.0,-BS/3., BS/3.,BS*2./3.,BS/3.),
	m_node(NULL),
	m_position(v3f(0,10*BS,0)),
	m_camera_offset(v3s16(0,0,0))
{
	ClientActiveObject::registerType(getType(), create);
}

ItemCAO::~ItemCAO()
{
}

ClientActiveObject* ItemCAO::create()
{
	return new ItemCAO();
}

void ItemCAO::addToScene(scene::ISceneManager *smgr)
{
	if(m_node != NULL)
		return;

	video::IVideoDriver* driver = smgr->getVideoDriver();

	scene::SMesh *mesh = new scene::SMesh();
	scene::IMeshBuffer *buf = new scene::SMeshBuffer();
	video::SColor c(255,255,255,255);
	video::S3DVertex vertices[4] =
	{
		/*video::S3DVertex(-BS/2,-BS/4,0, 0,0,0, c, 0,1),
		video::S3DVertex(BS/2,-BS/4,0, 0,0,0, c, 1,1),
		video::S3DVertex(BS/2,BS/4,0, 0,0,0, c, 1,0),
		video::S3DVertex(-BS/2,BS/4,0, 0,0,0, c, 0,0),*/
		video::S3DVertex(BS/3.,0,0, 0,0,0, c, 0,1),
		video::S3DVertex(-BS/3.,0,0, 0,0,0, c, 1,1),
		video::S3DVertex(-BS/3.,0+BS*2./3.,0, 0,0,0, c, 1,0),
		video::S3DVertex(BS/3.,0+BS*2./3.,0, 0,0,0, c, 0,0),
	};
	u16 indices[] = {0,1,2,2,3,0};
	buf->append(vertices, 4, indices, 6);
	// Set material
	buf->getMaterial().setFlag(video::EMF_LIGHTING, false);
	buf->getMaterial().setFlag(video::EMF_BACK_FACE_CULLING, false);
	//buf->getMaterial().setTexture(0, NULL);
	// Initialize with the stick texture
	buf->getMaterial().setTexture
			(0, driver->getTexture(getTexturePath("stick.png").c_str()));
	buf->getMaterial().setFlag(video::EMF_BILINEAR_FILTER, false);
	buf->getMaterial().setFlag(video::EMF_FOG_ENABLE, true);
	buf->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	// Add to mesh
	mesh->addMeshBuffer(buf);
	buf->drop();
	m_node = smgr->addMeshSceneNode(mesh, NULL);
	mesh->drop();
	// Set it to use the materials of the meshbuffers directly.
	// This is needed for changing the texture in the future
	m_node->setReadOnlyMaterials(true);
	updateNodePos();

	/*
		Update image of node
	*/

	// Create an inventory item to see what is its image
	std::istringstream is(m_inventorystring, std::ios_base::binary);
	video::ITexture *texture = NULL;
	try{
		InventoryItem *item = NULL;
		item = InventoryItem::deSerialize(is);
		infostream<<__FUNCTION_NAME<<": m_inventorystring=\""
				<<m_inventorystring<<"\" -> item="<<item
				<<std::endl;
		if(item)
		{
			texture = item->getImage();
			delete item;
		}
	}
	catch(SerializationError &e)
	{
		infostream<<"WARNING: "<<__FUNCTION_NAME
				<<": error deSerializing inventorystring \""
				<<m_inventorystring<<"\""<<std::endl;
	}

	// Set meshbuffer texture
	buf->getMaterial().setTexture(0, texture);
}

void ItemCAO::removeFromScene()
{
	if(m_node == NULL)
		return;

	m_node->remove();
	m_node = NULL;
}

void ItemCAO::updateLight(u8 light_at_pos)
{
	if(m_node == NULL)
		return;

	u8 li = decode_light(light_at_pos);
	video::SColor color(255,li,li,li);
	setMeshVerticesColor(m_node->getMesh(), color);
}

v3s16 ItemCAO::getLightPosition()
{
	return floatToInt(m_position, BS);
}

void ItemCAO::updateNodePos()
{
	if(m_node == NULL)
		return;

	m_node->setPosition(m_position-intToFloat(m_camera_offset, BS));
}

void ItemCAO::step(float dtime, ClientEnvironment *env)
{
	if(m_node)
	{
		/*v3f rot = m_node->getRotation();
		rot.Y += dtime * 120;
		m_node->setRotation(rot);*/
		LocalPlayer *player = env->getLocalPlayer();
		assert(player);
		v3f rot = m_node->getRotation();
		rot.Y = 180.0 - (player->getYaw());
		m_node->setRotation(rot);
	}
}

void ItemCAO::processMessage(const std::string &data)
{
	infostream<<"ItemCAO: Got message"<<std::endl;
	std::istringstream is(data, std::ios::binary);
	// command
	u8 cmd = readU8(is);
	if(cmd == 0)
	{
		// pos
		m_position = readV3F1000(is);
		updateNodePos();
	}
}

void ItemCAO::initialize(const std::string &data)
{
	infostream<<"ItemCAO: Got init data"<<std::endl;

	{
		std::istringstream is(data, std::ios::binary);
		// version
		u8 version = readU8(is);
		// check version
		if(version != 0)
			return;
		// pos
		m_position = readV3F1000(is);
		// inventorystring
		m_inventorystring = deSerializeString(is);
	}

	updateNodePos();
}

/*
	RatCAO
*/

#include "inventory.h"

// Prototype
RatCAO proto_RatCAO;

RatCAO::RatCAO():
	ClientActiveObject(0),
	m_selection_box(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.),
	m_node(NULL),
	m_position(v3f(0,10*BS,0)),
	m_camera_offset(v3s16(0,0,0)),
	m_yaw(0)
{
	ClientActiveObject::registerType(getType(), create);
}

RatCAO::~RatCAO()
{
}

ClientActiveObject* RatCAO::create()
{
	return new RatCAO();
}

void RatCAO::addToScene(scene::ISceneManager *smgr)
{
	if(m_node != NULL)
		return;

	video::IVideoDriver* driver = smgr->getVideoDriver();
	scene::IAnimatedMesh* mesh = createModelMesh(smgr,"rat.x",true);
	if (!mesh)
		return;

	m_node = smgr->addAnimatedMeshSceneNode(mesh);

	if (m_node) {
		m_node->setFrameLoop(0,79);
		m_node->setScale(v3f(1.0,1.0,1.0));
		setMeshColor(m_node->getMesh(), video::SColor(255,255,255,255));

		bool use_trilinear_filter = g_settings->getBool("trilinear_filter");
		bool use_bilinear_filter = g_settings->getBool("bilinear_filter");
		bool use_anisotropic_filter = g_settings->getBool("anisotropic_filter");

		// Set material flags and texture
		m_node->setMaterialTexture( 0, driver->getTexture(getTexturePath("rat_mob.png").c_str()));
		video::SMaterial& material = m_node->getMaterial(0);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_TRILINEAR_FILTER, use_trilinear_filter);
		material.setFlag(video::EMF_BILINEAR_FILTER, use_bilinear_filter);
		material.setFlag(video::EMF_ANISOTROPIC_FILTER, use_anisotropic_filter);
		material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

		m_node->setPosition(v3f(0,0,0));
	}
#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
	mesh->drop();
#endif
	updateNodePos();
}

void RatCAO::removeFromScene()
{
	if (m_node == NULL)
		return;

	m_node->remove();
	m_node = NULL;
}

void RatCAO::updateLight(u8 light_at_pos)
{
	if(m_node == NULL)
		return;

	u8 li = decode_light(light_at_pos);
	video::SColor color(255,li,li,li);
	setMeshVerticesColor(m_node->getMesh(), color);
}

v3s16 RatCAO::getLightPosition()
{
	return floatToInt(m_position+v3f(0,BS*0.5,0), BS);
}

void RatCAO::updateNodePos()
{
	if(m_node == NULL)
		return;

	//m_node->setPosition(m_position);
	m_node->setPosition(pos_translator.vect_show-intToFloat(m_camera_offset, BS));

	v3f rot = m_node->getRotation();
	rot.Y = 90.0 - m_yaw;
	m_node->setRotation(rot);
}

void RatCAO::step(float dtime, ClientEnvironment *env)
{
	pos_translator.translate(dtime);
	updateNodePos();
}

void RatCAO::processMessage(const std::string &data)
{
	//infostream<<"RatCAO: Got message"<<std::endl;
	std::istringstream is(data, std::ios::binary);
	// command
	u8 cmd = readU8(is);
	if(cmd == 0)
	{
		// pos
		m_position = readV3F1000(is);
		pos_translator.update(m_position);
		// yaw
		m_yaw = readF1000(is);
		updateNodePos();
	}
}

void RatCAO::initialize(const std::string &data)
{
	//infostream<<"RatCAO: Got init data"<<std::endl;

	{
		std::istringstream is(data, std::ios::binary);
		// version
		u8 version = readU8(is);
		// check version
		if(version != 0)
			return;
		// pos
		m_position = readV3F1000(is);
		pos_translator.init(m_position);
	}

	updateNodePos();
}

/*
	Oerkki1CAO
*/

#include "inventory.h"

// Prototype
Oerkki1CAO proto_Oerkki1CAO;

Oerkki1CAO::Oerkki1CAO():
	ClientActiveObject(0),
	m_selection_box(-BS/3.,0.0,-BS/3., BS/3.,BS*2.,BS/3.),
	m_node(NULL),
	m_position(v3f(0,10*BS,0)),
	m_camera_offset(v3s16(0,0,0)),
	m_yaw(0),
	m_damage_visual_timer(0),
	m_damage_texture_enabled(false)
{
	ClientActiveObject::registerType(getType(), create);
}

Oerkki1CAO::~Oerkki1CAO()
{
}

ClientActiveObject* Oerkki1CAO::create()
{
	return new Oerkki1CAO();
}

void Oerkki1CAO::addToScene(scene::ISceneManager *smgr)
{
	if(m_node != NULL)
		return;

	video::IVideoDriver* driver = smgr->getVideoDriver();
	scene::IAnimatedMesh* mesh = createModelMesh(smgr,"oerkki.x",true);
	if (!mesh)
		return;

	m_node = smgr->addAnimatedMeshSceneNode(mesh);

	if (m_node) {
		m_node->setFrameLoop(24,36);
		m_node->setScale(v3f(5.0,5.0,5.0));
		setMeshColor(m_node->getMesh(), video::SColor(255,255,255,255));
		bool use_trilinear_filter = g_settings->getBool("trilinear_filter");
		bool use_bilinear_filter = g_settings->getBool("bilinear_filter");
		bool use_anisotropic_filter = g_settings->getBool("anisotropic_filter");

		// Set material flags and texture
		m_node->setMaterialTexture( 0, driver->getTexture(getTexturePath("oerkki.png").c_str()));
		video::SMaterial& material = m_node->getMaterial(0);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_TRILINEAR_FILTER, use_trilinear_filter);
		material.setFlag(video::EMF_BILINEAR_FILTER, use_bilinear_filter);
		material.setFlag(video::EMF_ANISOTROPIC_FILTER, use_anisotropic_filter);
		material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

		m_node->setPosition(v3f(0,0,0));
	}
#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
	mesh->drop();
#endif
	updateNodePos();
}

void Oerkki1CAO::removeFromScene()
{
	if(m_node == NULL)
		return;

	m_node->remove();
	m_node = NULL;
}

void Oerkki1CAO::updateLight(u8 light_at_pos)
{
	if(m_node == NULL)
		return;

	if(light_at_pos <= 2)
	{
		m_node->setVisible(false);
		return;
	}

	m_node->setVisible(true);

	u8 li = decode_light(light_at_pos);
	video::SColor color(255,li,li,li);
	setMeshVerticesColor(m_node->getMesh(), color);
}

v3s16 Oerkki1CAO::getLightPosition()
{
	return floatToInt(m_position+v3f(0,BS*1.5,0), BS);
}

void Oerkki1CAO::updateNodePos()
{
	if(m_node == NULL)
		return;

	m_node->setPosition(pos_translator.vect_show-intToFloat(m_camera_offset, BS));

	v3f rot = m_node->getRotation();
	rot.Y = 90-m_yaw;
	m_node->setRotation(rot);
}

void Oerkki1CAO::step(float dtime, ClientEnvironment *env)
{
	pos_translator.translate(dtime);
	updateNodePos();

	LocalPlayer *player = env->getLocalPlayer();
	assert(player);

	v3f playerpos = player->getPosition();
	v2f playerpos_2d(playerpos.X,playerpos.Z);
	v2f objectpos_2d(m_position.X,m_position.Z);

	if(fabs(m_position.Y - playerpos.Y) < 1.5*BS &&
			objectpos_2d.getDistanceFrom(playerpos_2d) < 1.5*BS)
	{
		if(m_attack_interval.step(dtime, 0.5))
		{
			env->damageLocalPlayer(2);
		}
	}

	if(m_damage_visual_timer > 0)
	{
		if(!m_damage_texture_enabled)
		{
			// Enable damage texture
			if(m_node)
				m_node->setFrameLoop(37,49);
			m_damage_texture_enabled = true;
		}
		m_damage_visual_timer -= dtime;
	}
	else
	{
		if(m_damage_texture_enabled)
		{
			// Disable damage texture
			if(m_node)
				m_node->setFrameLoop(24,36);
			m_damage_texture_enabled = false;
		}
	}
}

void Oerkki1CAO::processMessage(const std::string &data)
{
	//infostream<<"Oerkki1CAO: Got message"<<std::endl;
	std::istringstream is(data, std::ios::binary);
	// command
	u8 cmd = readU8(is);
	if(cmd == 0)
	{
		// pos
		m_position = readV3F1000(is);
		pos_translator.update(m_position);
		// yaw
		m_yaw = readF1000(is);
		updateNodePos();
	}
	else if(cmd == 1)
	{
		//u16 damage = readU8(is);
		m_damage_visual_timer = 1.0;
	}
}

void Oerkki1CAO::initialize(const std::string &data)
{
	//infostream<<"Oerkki1CAO: Got init data"<<std::endl;

	{
		std::istringstream is(data, std::ios::binary);
		// version
		u8 version = readU8(is);
		// check version
		if(version != 0)
			return;
		// pos
		m_position = readV3F1000(is);
		pos_translator.init(m_position);
	}

	updateNodePos();
}

bool Oerkki1CAO::directReportPunch(const std::string &toolname, v3f dir)
{
	m_damage_visual_timer = 1.0;

	m_position += dir * BS;
	pos_translator.sharpen();
	pos_translator.update(m_position);
	updateNodePos();

	return false;
}

/*
	FireflyCAO
*/

// Prototype
FireflyCAO proto_FireflyCAO;

FireflyCAO::FireflyCAO():
	ClientActiveObject(0),
	m_selection_box(-BS/3.,0.0,-BS/3., BS/3.,BS/2.,BS/3.),
	m_node(NULL),
	m_position(v3f(0,10*BS,0)),
	m_camera_offset(v3s16(0,0,0)),
	m_yaw(0)
{
	ClientActiveObject::registerType(getType(), create);
}

FireflyCAO::~FireflyCAO()
{
}

ClientActiveObject* FireflyCAO::create()
{
	return new FireflyCAO();
}

void FireflyCAO::addToScene(scene::ISceneManager *smgr)
{
	if(m_node != NULL)
		return;

	video::IVideoDriver* driver = smgr->getVideoDriver();

	scene::SMesh *mesh = new scene::SMesh();
	scene::IMeshBuffer *buf = new scene::SMeshBuffer();
	video::SColor c(255,255,255,255);
	video::S3DVertex vertices[4] =
	{
		video::S3DVertex(0,0,0, 0,0,0, c, 0,1),
		video::S3DVertex(BS/2,0,0, 0,0,0, c, 1,1),
		video::S3DVertex(BS/2,BS/2,0, 0,0,0, c, 1,0),
		video::S3DVertex(0,BS/2,0, 0,0,0, c, 0,0),
	};
	u16 indices[] = {0,1,2,2,3,0};
	buf->append(vertices, 4, indices, 6);
	// Set material
	buf->getMaterial().setFlag(video::EMF_LIGHTING, false);
	buf->getMaterial().setFlag(video::EMF_BACK_FACE_CULLING, false);
	//buf->getMaterial().setTexture(0, NULL);
	buf->getMaterial().setTexture
			(0, driver->getTexture(getTexturePath("firefly.png").c_str()));
	buf->getMaterial().setFlag(video::EMF_BILINEAR_FILTER, false);
	buf->getMaterial().setFlag(video::EMF_FOG_ENABLE, true);
	buf->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL;
	// Add to mesh
	mesh->addMeshBuffer(buf);
	buf->drop();
	m_node = smgr->addMeshSceneNode(mesh, NULL);
	mesh->drop();
	// Set it to use the materials of the meshbuffers directly.
	// This is needed for changing the texture in the future
	m_node->setReadOnlyMaterials(true);
	updateNodePos();
}

void FireflyCAO::removeFromScene()
{
	if(m_node == NULL)
		return;

	m_node->remove();
	m_node = NULL;
}

void FireflyCAO::updateLight(u8 light_at_pos)
{
	if(m_node == NULL)
		return;

	u8 li = 255;
	video::SColor color(255,li,li,li);
	setMeshVerticesColor(m_node->getMesh(), color);
}

v3s16 FireflyCAO::getLightPosition()
{
	return floatToInt(m_position+v3f(0,BS*0.5,0), BS);
}

void FireflyCAO::updateNodePos()
{
	if(m_node == NULL)
		return;

	m_node->setPosition(pos_translator.vect_show-intToFloat(m_camera_offset, BS));

	v3f rot = m_node->getRotation();
	rot.Y = 180.0 - m_yaw;
	m_node->setRotation(rot);
}

void FireflyCAO::step(float dtime, ClientEnvironment *env)
{
	pos_translator.translate(dtime);
	updateNodePos();
}

void FireflyCAO::processMessage(const std::string &data)
{
	//infostream<<"FireflyCAO: Got message"<<std::endl;
	std::istringstream is(data, std::ios::binary);
	// command
	u8 cmd = readU8(is);
	if(cmd == 0)
	{
		// pos
		m_position = readV3F1000(is);
		pos_translator.update(m_position);
		// yaw
		m_yaw = readF1000(is);
		updateNodePos();
	}
}

void FireflyCAO::initialize(const std::string &data)
{
	//infostream<<"FireflyCAO: Got init data"<<std::endl;

	{
		std::istringstream is(data, std::ios::binary);
		// version
		u8 version = readU8(is);
		// check version
		if(version != 0)
			return;
		// pos
		m_position = readV3F1000(is);
		pos_translator.init(m_position);
	}

	updateNodePos();
}

/*
	MobV2CAO
*/

// Prototype
MobV2CAO proto_MobV2CAO;

MobV2CAO::MobV2CAO():
	ClientActiveObject(0),
	m_selection_box(-0.4*BS,-0.4*BS,-0.4*BS, 0.4*BS,0.8*BS,0.4*BS),
	m_node(NULL),
	m_position(v3f(0,10*BS,0)),
	m_camera_offset(v3s16(0,0,0)),
	m_yaw(0),
	m_walking(false),
	m_walking_unset_timer(0),
	m_walk_timer(0),
	m_walk_frame(0),
	m_damage_visual_timer(0),
	m_last_light(0),
	m_shooting(0),
	m_shooting_unset_timer(0),
	m_sprite_size(BS,BS),
	m_sprite_y(0),
	m_bright_shooting(false),
	m_lock_full_brightness(false),
	m_player_hit_timer(0)
{
	ClientActiveObject::registerType(getType(), create);

	m_properties = new Settings;
}

MobV2CAO::~MobV2CAO()
{
	delete m_properties;
}

ClientActiveObject* MobV2CAO::create()
{
	return new MobV2CAO();
}

void MobV2CAO::addToScene(scene::ISceneManager *smgr)
{
	if(m_node != NULL)
		return;

	/*infostream<<"MobV2CAO::addToScene using texture_name="<<
			m_texture_name<<std::endl;*/
	std::string texture_string = "[makealpha2:128,0,0;128,128,0:";
	texture_string += m_texture_name;

	scene::IBillboardSceneNode *bill = smgr->addBillboardSceneNode(
		NULL, v2f(1, 1), v3f(0,0,0), -1);
	bill->setMaterialTexture(0, g_texturesource->getTextureRaw(texture_string));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_BILINEAR_FILTER, false);
	bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
	bill->setMaterialFlag(video::EMF_FOG_ENABLE, true);
	bill->setColor(video::SColor(255,0,0,0));
	bill->setVisible(false); /* Set visible when brightness is known */
	bill->setSize(m_sprite_size);
	if(m_sprite_type == "humanoid_1"){
		const float txp = 1./192;
		const float txs = txp*32;
		const float typ = 1./240;
		const float tys = typ*48;
		setBillboardTextureMatrix(bill, txs, tys, 0, 0);
	} else if(m_sprite_type == "simple"){
		const float txs = 1.0;
		const float tys = 1.0 / m_simple_anim_frames;
		setBillboardTextureMatrix(bill, txs, tys, 0, 0);
	} else {
		infostream<<"MobV2CAO: Unknown sprite type \""<<m_sprite_type<<"\""
				<<std::endl;
	}

	m_node = bill;

	updateNodePos();
}

void MobV2CAO::removeFromScene()
{
	if(m_node == NULL)
		return;

	m_node->drop();
	m_node->remove();
	m_node = NULL;
}

void MobV2CAO::updateLight(u8 light_at_pos)
{
	if(m_lock_full_brightness)
		light_at_pos = 15;

	m_last_light = light_at_pos;

	if(m_node == NULL)
		return;

	if(m_damage_visual_timer > 0)
		return;

	if(m_shooting && m_bright_shooting)
		return;

	/*if(light_at_pos <= 2){
		m_node->setVisible(false);
		return;
	}*/

	m_node->setVisible(true);

	u8 li = decode_light(light_at_pos);
	video::SColor color(255,li,li,li);
	m_node->setColor(color);
}

v3s16 MobV2CAO::getLightPosition()
{
	return floatToInt(m_position+v3f(0,0,0), BS);
}

void MobV2CAO::updateNodePos()
{
	if(m_node == NULL)
		return;

	m_node->setPosition(pos_translator.vect_show-intToFloat(m_camera_offset, BS) + v3f(0,m_sprite_y,0));
}

void MobV2CAO::step(float dtime, ClientEnvironment *env)
{
	scene::IBillboardSceneNode *bill = m_node;
	if(!bill)
		return;

	pos_translator.translate(dtime);

	if(m_sprite_type == "humanoid_1"){
		scene::ICameraSceneNode* camera = m_node->getSceneManager()->getActiveCamera();
		if(!camera)
			return;
		v3f cam_to_mob = m_node->getAbsolutePosition() - camera->getAbsolutePosition();
		cam_to_mob.normalize();
		int col = 0;
		if(cam_to_mob.Y > 0.75)
			col = 5;
		else if(cam_to_mob.Y < -0.75)
			col = 4;
		else{
			float mob_dir = atan2(cam_to_mob.Z, cam_to_mob.X) / PI * 180.;
			float dir = mob_dir - m_yaw;
			dir = wrapDegrees_180(dir);
			//infostream<<"id="<<m_id<<" dir="<<dir<<std::endl;
			if(fabs(wrapDegrees_180(dir - 0)) <= 45.1)
				col = 2;
			else if(fabs(wrapDegrees_180(dir - 90)) <= 45.1)
				col = 3;
			else if(fabs(wrapDegrees_180(dir - 180)) <= 45.1)
				col = 0;
			else if(fabs(wrapDegrees_180(dir + 90)) <= 45.1)
				col = 1;
			else
				col = 4;
		}

		int row = 0;
		if(m_shooting){
			row = 3;
		} else if(m_walking){
			m_walk_timer += dtime;
			if(m_walk_timer >= 0.5){
				m_walk_frame = (m_walk_frame + 1) % 2;
				m_walk_timer = 0;
			}
			if(m_walk_frame == 0)
				row = 1;
			else
				row = 2;
		}

		const float txp = 1./192;
		const float txs = txp*32;
		const float typ = 1./240;
		const float tys = typ*48;
		setBillboardTextureMatrix(bill, txs, tys, col, row);
	} else if(m_sprite_type == "simple"){
		m_walk_timer += dtime;
		if(m_walk_timer >= m_simple_anim_frametime){
			m_walk_frame = (m_walk_frame + 1) % m_simple_anim_frames;
			m_walk_timer = 0;
		}
		int col = 0;
		int row = m_walk_frame;
		const float txs = 1.0;
		const float tys = 1.0 / m_simple_anim_frames;
		setBillboardTextureMatrix(bill, txs, tys, col, row);
	} else {
		infostream<<"MobV2CAO::step(): Unknown sprite type \""
				<<m_sprite_type<<"\""<<std::endl;
	}

	updateNodePos();

	/* Damage local player */
	if(m_player_hit_damage && m_player_hit_timer <= 0.0){
		LocalPlayer *player = env->getLocalPlayer();
		assert(player);

		v3f playerpos = player->getPosition();
		v2f playerpos_2d(playerpos.X,playerpos.Z);
		v2f objectpos_2d(m_position.X,m_position.Z);

		if(fabs(m_position.Y - playerpos.Y) < m_player_hit_distance*BS &&
		objectpos_2d.getDistanceFrom(playerpos_2d) < m_player_hit_distance*BS)
		{
			env->damageLocalPlayer(m_player_hit_damage);
			m_player_hit_timer = m_player_hit_interval;
		}
	}

	/* Run timers */

	m_player_hit_timer -= dtime;

	if(m_damage_visual_timer >= 0){
		m_damage_visual_timer -= dtime;
		if(m_damage_visual_timer <= 0){
			infostream<<"id="<<m_id<<" damage visual ended"<<std::endl;
		}
	}

	m_walking_unset_timer += dtime;
	if(m_walking_unset_timer >= 1.0){
		m_walking = false;
	}

	m_shooting_unset_timer -= dtime;
	if(m_shooting_unset_timer <= 0.0){
		if(m_bright_shooting){
			u8 li = decode_light(m_last_light);
			video::SColor color(255,li,li,li);
			bill->setColor(color);
			m_bright_shooting = false;
		}
		m_shooting = false;
	}

}

void MobV2CAO::processMessage(const std::string &data)
{
	//infostream<<"MobV2CAO: Got message"<<std::endl;
	std::istringstream is(data, std::ios::binary);
	// command
	u8 cmd = readU8(is);

	// Move
	if(cmd == 0)
	{
		// pos
		m_position = readV3F1000(is);
		pos_translator.update(m_position);
		// yaw
		m_yaw = readF1000(is);

		m_walking = true;
		m_walking_unset_timer = 0;

		updateNodePos();
	}
	// Damage
	else if(cmd == 1)
	{
		//u16 damage = readU16(is);

		/*u8 li = decode_light(m_last_light);
		if(li >= 100)
			li = 30;
		else
			li = 255;*/

		/*video::SColor color(255,255,0,0);
		m_node->setColor(color);

		m_damage_visual_timer = 0.2;*/
	}
	// Trigger shooting
	else if(cmd == 2)
	{
		// length
		m_shooting_unset_timer = readF1000(is);
		// bright?
		m_bright_shooting = readU8(is);
		if(m_bright_shooting){
			u8 li = 255;
			video::SColor color(255,li,li,li);
			m_node->setColor(color);
		}

		m_shooting = true;
	}
}

void MobV2CAO::initialize(const std::string &data)
{
	//infostream<<"MobV2CAO: Got init data"<<std::endl;

	{
		std::istringstream is(data, std::ios::binary);
		// version
		u8 version = readU8(is);
		// check version
		if(version != 0){
			infostream<<__FUNCTION_NAME<<": Invalid version"<<std::endl;
			return;
		}

		std::ostringstream tmp_os(std::ios::binary);
		decompressZlib(is, tmp_os);
		std::istringstream tmp_is(tmp_os.str(), std::ios::binary);
		m_properties->parseConfigLines(tmp_is, "MobArgsEnd");

		infostream<<"MobV2CAO::initialize(): got properties:"<<std::endl;
		m_properties->writeLines(infostream);

		m_properties->setDefault("looks", "dummy_default");
		m_properties->setDefault("yaw", "0");
		m_properties->setDefault("pos", "(0,0,0)");
		m_properties->setDefault("player_hit_damage", "0");
		m_properties->setDefault("player_hit_distance", "1.5");
		m_properties->setDefault("player_hit_interval", "1.5");

		setLooks(m_properties->get("looks"));
		m_yaw = m_properties->getFloat("yaw");
		m_position = m_properties->getV3F("pos");
		m_player_hit_damage = m_properties->getS32("player_hit_damage");
		m_player_hit_distance = m_properties->getFloat("player_hit_distance");
		m_player_hit_interval = m_properties->getFloat("player_hit_interval");

		pos_translator.init(m_position);
	}

	updateNodePos();
}

bool MobV2CAO::directReportPunch(const std::string &toolname, v3f dir)
{
	video::SColor color(255,255,0,0);
	m_node->setColor(color);

	m_damage_visual_timer = 0.05;

	m_position += dir * BS;
	pos_translator.sharpen();
	pos_translator.update(m_position);
	updateNodePos();

	return false;
}

void MobV2CAO::setLooks(const std::string &looks)
{
	v2f selection_size = v2f(0.4, 0.4) * BS;
	float selection_y = 0 * BS;

	if(looks == "dungeon_master"){
		m_texture_name = "dungeon_master.png";
		m_sprite_type = "humanoid_1";
		m_sprite_size = v2f(2, 3) * BS;
		m_sprite_y = 0.85 * BS;
		selection_size = v2f(0.4, 2.6) * BS;
		selection_y = -0.4 * BS;
	}
	else if(looks == "fireball"){
		m_texture_name = "fireball.png";
		m_sprite_type = "simple";
		m_sprite_size = v2f(1, 1) * BS;
		m_simple_anim_frames = 3;
		m_simple_anim_frametime = 0.1;
		m_lock_full_brightness = true;
	}
	else{
		m_texture_name = "stone.png";
		m_sprite_type = "simple";
		m_sprite_size = v2f(1, 1) * BS;
		m_simple_anim_frames = 3;
		m_simple_anim_frametime = 0.333;
		selection_size = v2f(0.4, 0.4) * BS;
		selection_y = 0 * BS;
	}

	m_selection_box = core::aabbox3d<f32>(
			-selection_size.X, selection_y, -selection_size.X,
			selection_size.X, selection_y+selection_size.Y,
			selection_size.X);
}

