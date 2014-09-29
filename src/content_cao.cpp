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
#include "content_mob.h"
#include "path.h"
#include "environment.h"
#include "settings.h"
#include "mesh.h"
#include <ICameraSceneNode.h>

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
	MobCAO
*/

// Prototype
MobCAO proto_MobCAO;


MobCAO::MobCAO():
	ClientActiveObject(0),
	m_selection_box(-0.4*BS,-0.4*BS,-0.4*BS, 0.4*BS,0.8*BS,0.4*BS),
	m_node(NULL),
	m_position(v3f(0,10*BS,0)),
	m_camera_offset(v3s16(0,0,0)),
	m_yaw(0),
	m_last_light(0),
	m_player_hit_timer(0),
	m_damage_visual_timer(0),
	m_shooting(false),
	m_shooting_unset_timer(0),
	m_walking(false),
	m_walking_unset_timer(0),
	m_draw_type(MDT_NOTHING)
{
	ClientActiveObject::registerType(getType(), create);
}
MobCAO::~MobCAO()
{
}
ClientActiveObject* MobCAO::create()
{
	return new MobCAO();
}
void MobCAO::addToScene(scene::ISceneManager *smgr)
{
	if (m_node != NULL)
		return;

	video::IVideoDriver* driver = smgr->getVideoDriver();
	MobFeatures m = content_mob_features(m_content);
	if (m.model != "") {
		scene::IAnimatedMesh* mesh = createModelMesh(smgr,m.model.c_str(),true);
		if (!mesh)
			return;

		scene::IAnimatedMeshSceneNode* node;

		node = smgr->addAnimatedMeshSceneNode(mesh);

		if (node) {
			int s;
			int e;
			m.getAnimationFrames(MA_STAND,&s,&e);
			node->setFrameLoop(s,e);
			node->setScale(m.model_scale);
			setMeshColor(node->getMesh(), video::SColor(255,255,255,255));
			bool use_trilinear_filter = g_settings->getBool("trilinear_filter");
			bool use_bilinear_filter = g_settings->getBool("bilinear_filter");
			bool use_anisotropic_filter = g_settings->getBool("anisotropic_filter");

			// Set material flags and texture
			node->setMaterialTexture( 0, driver->getTexture(getTexturePath(m.texture).c_str()));
			video::SMaterial& material = node->getMaterial(0);
			material.setFlag(video::EMF_LIGHTING, false);
			material.setFlag(video::EMF_TRILINEAR_FILTER, use_trilinear_filter);
			material.setFlag(video::EMF_BILINEAR_FILTER, use_bilinear_filter);
			material.setFlag(video::EMF_ANISOTROPIC_FILTER, use_anisotropic_filter);
			material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

			node->setVisible(true);
			m_draw_type = MDT_MODEL;
		}
		m_node = (scene::IMeshSceneNode*)node;
#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
		mesh->drop();
#endif
		updateNodePos();
	}else if (m.nodeboxes.size() > 0) {
	}else if (m.texture != "") {
		bool use_trilinear_filter = g_settings->getBool("trilinear_filter");
		bool use_bilinear_filter = g_settings->getBool("bilinear_filter");
		bool use_anisotropic_filter = g_settings->getBool("anisotropic_filter");
		scene::IBillboardSceneNode *bill = smgr->addBillboardSceneNode(NULL, v2f(1, 1), v3f(0,0,0), -1);
		bill->setMaterialTexture(0, driver->getTexture(getTexturePath(m.texture).c_str()));
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialFlag(video::EMF_TRILINEAR_FILTER, use_trilinear_filter);
		bill->setMaterialFlag(video::EMF_BILINEAR_FILTER, use_bilinear_filter);
		bill->setMaterialFlag(video::EMF_ANISOTROPIC_FILTER, use_anisotropic_filter);
		bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		bill->setMaterialFlag(video::EMF_FOG_ENABLE, true);
		bill->setColor(video::SColor(255,0,0,0));
		bill->setVisible(true);
		bill->setSize(m.model_scale.X*BS,m.model_scale.Y*BS,m.model_scale.Z*BS);
		m_node = (scene::IMeshSceneNode*)bill;
		m_draw_type = MDT_SPRITE;
	}
}
void MobCAO::removeFromScene()
{
	if (m_node != NULL) {
		m_node->remove();
		m_node = NULL;
	}
}
void MobCAO::updateLight(u8 light_at_pos)
{
	MobFeatures m = content_mob_features(m_content);
	if (m.glow_light)
		light_at_pos = decode_light(m.glow_light);
	if (m_shooting && m.attack_glow_light)
		light_at_pos = decode_light(m.attack_glow_light);

	m_last_light = light_at_pos;

	//if (m_damage_visual_timer > 0)
		//return;

	u8 li = decode_light(light_at_pos);
	video::SColor color(255,li,li,li);

	if (m_node != NULL) {
		if (m_draw_type == MDT_MODEL) {
			setMeshVerticesColor(((scene::IAnimatedMeshSceneNode*)m_node)->getMesh(), color);
		}else if (m_draw_type == MDT_SPRITE) {
			((scene::IBillboardSceneNode*)m_node)->setColor(color);
		}
	}
}
v3s16 MobCAO::getLightPosition()
{
	return floatToInt(m_position, BS);
}
void MobCAO::updateNodePos()
{
	if (m_node == NULL)
		return;
	v3f offset = content_mob_features(m_content).model_offset+v3f(0,-0.5,0);
	offset.X *= (float)BS;
	offset.Y *= (float)BS;
	offset.Z *= (float)BS;
	offset += pos_translator.vect_show;
	m_node->setPosition(offset-intToFloat(m_camera_offset, BS));

	v3f rot = m_node->getRotation();
	if (m_draw_type == MDT_MODEL) {
		rot.Y = (90-m_yaw)+content_mob_features(m_content).model_rotation.Y;
	}else if (m_draw_type == MDT_SPRITE) {
		rot.Y = m_yaw+content_mob_features(m_content).model_rotation.Y;
	}
	m_node->setRotation(rot);
}
void MobCAO::step(float dtime, ClientEnvironment *env)
{
	if (!m_node)
		return;

	MobFeatures m = content_mob_features(m_content);

	pos_translator.translate(dtime);

	updateNodePos();

	/* Damage local player */
	if (m.attack_player_damage && m_player_hit_timer <= 0.0) {
		LocalPlayer *player = env->getLocalPlayer();
		assert(player);

		v3f playerpos = player->getPosition();

		if (
			fabs(m_position.X - playerpos.X) < m.attack_player_range.X*BS
			&& (
				fabs(m_position.Y - playerpos.Y) < m.attack_player_range.Y*BS
				|| (
					m_position.Y > playerpos.Y
					&& fabs(m_position.Y-(playerpos.Y+BS)) < m.attack_player_range.Y*BS
				)
			) && fabs(m_position.Z - playerpos.Z) < m.attack_player_range.Z*BS
		) {
			env->damageLocalPlayer(m.attack_player_damage);
			m_player_hit_timer = 3.0;
		}
	}

	/* Run timers */
	m_player_hit_timer -= dtime;

	if (m_damage_visual_timer >= 0) {
		m_damage_visual_timer -= dtime;
		if (m_damage_visual_timer <= 0) {
			infostream<<"id="<<m_id<<" damage visual ended"<<std::endl;
		}
	}

	m_walking_unset_timer += dtime;
	if (m_walking_unset_timer >= 1.0) {
		m_walking = false;
		setAnimation(MA_STAND);
	}

	m_shooting_unset_timer -= dtime;
	if (m_shooting_unset_timer <= 0.0 && m_shooting) {
		if (m.attack_glow_light) {
			u8 li = decode_light(m_last_light);
			video::SColor color(255,li,li,li);
			if (m_draw_type == MDT_MODEL) {
				setMeshVerticesColor(((scene::IAnimatedMeshSceneNode*)m_node)->getMesh(), color);
			}else if (m_draw_type == MDT_SPRITE) {
				((scene::IBillboardSceneNode*)m_node)->setColor(color);
			}
		}
		m_shooting = false;
		if (m_walking) {
			setAnimation(MA_MOVE);
		}else{
			setAnimation(MA_STAND);
		}
	}
}
void MobCAO::processMessage(const std::string &data)
{
	//infostream<<"MobCAO: Got message"<<std::endl;
	std::istringstream is(data, std::ios::binary);
	// command
	u8 cmd = readU8(is);

	// Move
	if (cmd == 0) {
		// pos
		m_position = readV3F1000(is);
		pos_translator.update(m_position);
		// yaw
		m_yaw = readF1000(is);

		if (!m_walking) {
			m_walking = true;
			m_walking_unset_timer = 0;
			setAnimation(MA_MOVE);
		}

		updateNodePos();
	}
	// Trigger shooting
	else if (cmd == 2) {
		// length
		m_shooting_unset_timer = readF1000(is);

		if (content_mob_features(m_content).attack_glow_light){
			u8 li = decode_light(content_mob_features(m_content).attack_glow_light);
			video::SColor color(255,li,li,li);
			if (m_node != NULL) {
				if (m_draw_type == MDT_MODEL) {
					setMeshVerticesColor(((scene::IAnimatedMeshSceneNode*)m_node)->getMesh(), color);
				}else if (m_draw_type == MDT_SPRITE) {
					((scene::IBillboardSceneNode*)m_node)->setColor(color);
				}
			}
		}

		if (!m_shooting) {
			m_shooting = true;
			setAnimation(MA_ATTACK);
		}
	}
}
void MobCAO::initialize(const std::string &data)
{
	{
		std::istringstream is(data, std::ios::binary);
		// version
		u8 version = readU8(is);
		// check version
		if (version != 0)
			return;
		// pos
		m_position = readV3F1000(is);
		pos_translator.init(m_position);
		// content
		m_content = readU16(is);
		// yaw
		m_yaw = readF1000(is);
		// speed
		v3f speed = readV3F1000(is);
		// age
		float age = readF1000(is);
		// hp
		age = readU8(is);
		// shooting
		version = readU8(is);
		m_shooting = !!version;
	}

	updateNodePos();
}
bool MobCAO::directReportPunch(const std::string &toolname, v3f dir)
{
	if (content_mob_features(m_content).punch_action == MPA_IGNORE)
		return false;
	video::SColor color(255,255,0,0);

	if (m_node != NULL) {
		if (m_draw_type == MDT_MODEL) {
			setMeshVerticesColor(((scene::IAnimatedMeshSceneNode*)m_node)->getMesh(), color);
		}else if (m_draw_type == MDT_SPRITE) {
			((scene::IBillboardSceneNode*)m_node)->setColor(color);
		}
	}

	m_damage_visual_timer = 0.05;

	m_position += dir * BS;
	pos_translator.sharpen();
	pos_translator.update(m_position);
	updateNodePos();

	return false;
}
void MobCAO::setAnimation(MobAnimation anim)
{
	if (m_draw_type != MDT_MODEL)
		return;
	int s;
	int e;
	content_mob_features(m_content).getAnimationFrames(anim,&s,&e);
	((scene::IAnimatedMeshSceneNode*)m_node)->setFrameLoop(s,e);
}
