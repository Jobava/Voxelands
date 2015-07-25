/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mapnode.cpp
* voxelands - 3d voxel world sandbox game
* Copyright (C) Lisa 'darkrose' Milne 2014 <lisa@ltmnet.com>
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

#include "common_irrlicht.h"
#include "mapnode.h"
#ifndef SERVER
#include "tile.h"
#endif
#include "porting.h"
#include <string>
#include "mineral.h"
// For g_settings
#include "main.h"
#include "content_mapnode.h"
#include "nodemetadata.h"

ContentFeatures::~ContentFeatures()
{
	delete initial_metadata;
}

std::vector<NodeBox> transformNodeBox(MapNode &n,
		const std::vector<NodeBox> &nodebox)
{
	std::vector<NodeBox> boxes;
	int facedir = 0;
	if (
		content_features(n).param2_type == CPT_FACEDIR_SIMPLE
		|| content_features(n).param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		facedir = n.param2&0x0F;
	}else if (
		content_features(n).param_type == CPT_FACEDIR_SIMPLE
		|| content_features(n).param_type == CPT_FACEDIR_WALLMOUNT
	) {
		facedir = n.param1;
	}
	for(std::vector<NodeBox>::const_iterator i = nodebox.begin(); i != nodebox.end(); i++) {
		NodeBox box = *i;
		if (facedir == 1) {
			box.m_box.MinEdge.rotateXZBy(-90);
			box.m_box.MaxEdge.rotateXZBy(-90);
			box.m_box.repair();
		}else if (facedir == 2) {
			box.m_box.MinEdge.rotateXZBy(180);
			box.m_box.MaxEdge.rotateXZBy(180);
			box.m_box.repair();
		}else if (facedir == 3) {
			box.m_box.MinEdge.rotateXZBy(90);
			box.m_box.MaxEdge.rotateXZBy(90);
			box.m_box.repair();
		}else if (facedir == 4) {
			box.m_box.MinEdge.rotateXYBy(-90);
			box.m_box.MaxEdge.rotateXYBy(-90);
			box.m_box.repair();
		}else if (facedir == 5) {
			box.m_box.MinEdge.rotateXYBy(90);
			box.m_box.MaxEdge.rotateXYBy(90);
			box.m_box.repair();
		}
		boxes.push_back(box);
	}
	return boxes;
}

std::vector<NodeBox> ContentFeatures::getNodeBoxes(MapNode &n) const
{
	return transformNodeBox(n, nodeboxes);
}

std::vector<NodeBox> ContentFeatures::getWieldNodeBoxes() const
{
	if (wield_nodeboxes.size() > 0)
		return wield_nodeboxes;
	return nodeboxes;
}

#ifndef SERVER
void ContentFeatures::setTexture(u16 i, std::string name, u8 alpha)
{
	used_texturenames[name] = true;

	if(g_texturesource)
	{
		tiles[i].texture = g_texturesource->getTexture(name);
	}

	if(alpha != 255)
	{
		tiles[i].alpha = alpha;
		tiles[i].material_type = MATERIAL_ALPHA_VERTEX;
	}

	if(inventory_texture == NULL)
		setInventoryTexture(name);
}

void ContentFeatures::setMetaTexture(u16 i, std::string name, u8 alpha)
{
	used_texturenames[name] = true;

	if(g_texturesource) {
		meta_tiles[i].texture = g_texturesource->getTexture(name);
	}

	if (alpha != 255) {
		meta_tiles[i].alpha = alpha;
		meta_tiles[i].material_type = MATERIAL_ALPHA_VERTEX;
	}
}

void ContentFeatures::setInventoryTexture(std::string imgname)
{
	if(g_texturesource == NULL)
		return;

	imgname += "^[forcesingle";

	inventory_texture = g_texturesource->getTextureRaw(imgname);
}

void ContentFeatures::setInventoryTextureCube(std::string top,
		std::string left, std::string right)
{
	if(g_texturesource == NULL)
		return;

	str_replace_char(top, '^', '&');
	str_replace_char(left, '^', '&');
	str_replace_char(right, '^', '&');

	std::string imgname_full;
	imgname_full += "[inventorycube{";
	imgname_full += top;
	imgname_full += "{";
	imgname_full += left;
	imgname_full += "{";
	imgname_full += right;
	inventory_texture = g_texturesource->getTextureRaw(imgname_full);
}

void ContentFeatures::setInventoryTextureNodeBox(content_t c, std::string top,
		std::string left, std::string right)
{
	if(g_texturesource == NULL)
		return;

	char n[50];
	sprintf(n,"%d",(int)c);

	str_replace_char(top, '^', '&');
	str_replace_char(left, '^', '&');
	str_replace_char(right, '^', '&');

	std::string imgname_full;
	imgname_full += "[inventorynode{";
	imgname_full += n;
	imgname_full += "{";
	imgname_full += top;
	imgname_full += "{";
	imgname_full += left;
	imgname_full += "{";
	imgname_full += right;
	inventory_texture = g_texturesource->getTextureRaw(imgname_full);
}
#endif

struct ContentFeatures g_content_features[MAX_CONTENT+1];

ContentFeatures & content_features(content_t i)
{
	if (i > MAX_CONTENT)
		i = CONTENT_IGNORE;
	return g_content_features[i];
}
ContentFeatures & content_features(MapNode &n)
{
	return content_features(n.getContent());
}

/*
	See mapnode.h for description.
*/
#ifndef SERVER
#include "common_irrlicht.h"
#include "game.h"
#include "intl.h"
void init_mapnode(irr::IrrlichtDevice* device)
#else
void init_mapnode()
#endif
{
	bool repeat = false;
	if (g_texturesource == NULL) {
		dstream<<"INFO: Initial run of init_mapnode with "
				"g_texturesource=NULL. If this segfaults, "
				"there is a bug with something not checking for "
				"the NULL value."<<std::endl;
	}else{
		repeat = true;
		dstream<<"INFO: Full run of init_mapnode with "
				"g_texturesource!=NULL"<<std::endl;
	}

	/*// Read some settings
	bool new_style_water = g_settings.getBool("new_style_water");
	bool new_style_leaves = g_settings.getBool("new_style_leaves");*/

	/*
		Initialize content feature table
	*/

#ifndef SERVER
	/*
		Set initial material type to same in all tiles, so that the
		same material can be used in more stuff.
		This is set according to the leaves because they are the only
		differing material to which all materials can be changed to
		get this optimization.
	*/
	u8 initial_material_type = MATERIAL_ALPHA_SIMPLE;
	/*if(new_style_leaves)
		initial_material_type = MATERIAL_ALPHA_SIMPLE;
	else
		initial_material_type = MATERIAL_ALPHA_NONE;*/
	for(u16 i=0; i<MAX_CONTENT+1; i++)
	{
		ContentFeatures *f = &g_content_features[i];
		// Re-initialize
		f->reset();

		for(u16 j=0; j<6; j++)
			f->tiles[j].material_type = initial_material_type;
	}
#endif

	/*
		Initially set every block to be shown as an unknown block.
		Don't touch CONTENT_IGNORE or CONTENT_AIR.
	*/
	for (u16 i=0; i <= MAX_CONTENT; i++) {
		char buf[10];
		sprintf(buf,"0x%.4X",i);
		ContentFeatures *f = &g_content_features[i];
		f->description = narrow_to_wide(std::string(buf));
		if (i == CONTENT_IGNORE || i == CONTENT_AIR)
			continue;
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("unknown_block.png");
	}

	// Make CONTENT_IGNORE to not block the view when occlusion culling
	content_features(CONTENT_IGNORE).solidness = 0;
	content_features(CONTENT_IGNORE).suffocation_per_second = 0;

	/*
		Initialize mapnode content
	*/
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Base MapNodes"));
#endif
	content_mapnode_init(repeat);
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Circuit MapNodes"));
#endif
	content_mapnode_circuit(repeat);
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Plant MapNodes"));
#endif
	content_mapnode_plants(repeat);
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Farming MapNodes"));
#endif
	content_mapnode_farm(repeat);
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Decorative MapNodes"));
#endif
	content_mapnode_furniture(repeat);
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Interactive MapNodes"));
#endif
	content_mapnode_door(repeat);
#ifndef SERVER
	drawLoadingScreen(device,wgettext("Loading Special MapNodes"));
#endif
	content_mapnode_stair(repeat);
	content_mapnode_slab(repeat);
	content_mapnode_special(repeat);
}

v3s16 facedir_rotate(u8 facedir, v3s16 dir)
{
	/*
		Face 2 (normally Z-) direction:
		facedir=0: Z-
		facedir=1: X-
		facedir=2: Z+
		facedir=3: X+
	*/
	switch (facedir) {
	case 0: // Same
		return v3s16(dir.X, dir.Y, dir.Z);
		break;
	case 1: // Face is taken from rotXZccv(-90)
		return v3s16(-dir.Z, dir.Y, dir.X);
		break;
	case 2: // Face is taken from rotXZccv(180)
		return v3s16(-dir.X, dir.Y, -dir.Z);
		break;
	case 3: // Face is taken from rotXZccv(90)
		return v3s16(dir.Z, dir.Y, -dir.X);
		break;
	default:;
	}

	return dir;
}

v3s16 MapNode::getRotation(v3s16 dir)
{
	if (
		content_features(*this).param2_type == CPT_FACEDIR_SIMPLE
		|| content_features(*this).param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param2&0x0F, dir);
	}else if (
		content_features(*this).param_type == CPT_FACEDIR_SIMPLE
		|| content_features(*this).param_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param1, dir);
	}
	return dir;
}

s16 MapNode::getRotationAngle()
{
	int facedir = 0;
	ContentFeatures &f = content_features(*this);
	if (
		f.param2_type == CPT_FACEDIR_SIMPLE
		|| f.param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		facedir = param2&0x0F;
	}else if (
		f.param_type == CPT_FACEDIR_SIMPLE
		|| f.param_type == CPT_FACEDIR_WALLMOUNT
	) {
		facedir = param1;
	}
	switch (facedir) {
	case 1:
		return -90;
		break;
	case 2:
		return 180;
		break;
	case 3:
		return 90;
		break;
	default:;
	}
	return 0;
}

v3s16 MapNode::getEffectedRotation()
{
	u8 facedir = 0;
	ContentFeatures *f = &content_features(getContent());
	if (f->onact_also_affects == v3s16(0,0,0))
		return v3s16(0,0,0);

	if (
		f->param2_type == CPT_FACEDIR_SIMPLE
		|| f->param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		facedir = (param2&0x0F);
	}else if (
		f->param_type == CPT_FACEDIR_SIMPLE
		|| f->param_type == CPT_FACEDIR_WALLMOUNT
	) {
		facedir = param1;
	}

	switch (facedir) {
	case 0: // Same
		return v3s16(-f->onact_also_affects.X, f->onact_also_affects.Y, -f->onact_also_affects.Z);
		break;
	case 1: // Face is taken from rotXZccv(-90)
		return v3s16(-f->onact_also_affects.Z, f->onact_also_affects.Y, f->onact_also_affects.X);
		break;
	case 2: // Face is taken from rotXZccv(180)
		return v3s16(f->onact_also_affects.X, f->onact_also_affects.Y, f->onact_also_affects.Z);
		break;
	case 3: // Face is taken from rotXZccv(90)
		return v3s16(f->onact_also_affects.Z, f->onact_also_affects.Y, -f->onact_also_affects.X);
		break;
	default:;
	}

	return f->onact_also_affects;
}

#ifndef SERVER
TileSpec MapNode::getTileFrom(v3s16 dir, TileSpec raw_spec[6], bool rotate)
{
	TileSpec spec;
	s32 dir_i = 0;
	ContentFeatures &f = content_features(*this);

	if (
		f.param2_type == CPT_FACEDIR_SIMPLE
		|| f.param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param2&0x0F, dir);
	}else if (
		f.param_type == CPT_FACEDIR_SIMPLE
		|| f.param_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param1, dir);
	}
	if (dir == v3s16(0,-1,0)) {
		dir_i = 1;
	}else if(dir == v3s16(1,0,0)) {
		dir_i = 2;
	}else if(dir == v3s16(-1,0,0)) {
		dir_i = 3;
	}else if(dir == v3s16(0,0,1)) {
		dir_i = 4;
	}else if(dir == v3s16(0,0,-1)) {
		dir_i = 5;
	}

	spec = raw_spec[dir_i];

	/*
		If it contains some mineral, change texture id
	*/
	if (f.param_type == CPT_MINERAL && g_texturesource) {
		u8 mineral = getMineral();
		std::string mineral_texture_name = mineral_features(mineral).texture;
		if (mineral_texture_name != "") {
			u32 orig_id = spec.texture.id;
			std::string texture_name = g_texturesource->getTextureName(orig_id);
			//texture_name += "^blit:";
			texture_name += "^";
			texture_name += mineral_texture_name;
			u32 new_id = g_texturesource->getTextureId(texture_name);
			spec.texture = g_texturesource->getTexture(new_id);
		}
	}
	if (rotate && f.rotate_tile_with_nodebox) {
		u32 orig_id = spec.texture.id;
		std::string texture_name = g_texturesource->getTextureName(orig_id);
		texture_name += getTileRotationString(dir);
		u32 new_id = g_texturesource->getTextureId(texture_name);
		spec.texture = g_texturesource->getTexture(new_id);
	}

	return spec;
}
std::string MapNode::getTileRotationString(v3s16 dir)
{
	s32 dir_i = 0;
	ContentFeatures &f = content_features(*this);
	if (!f.rotate_tile_with_nodebox)
		return "";

	if (
		f.param2_type == CPT_FACEDIR_SIMPLE
		|| f.param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param2&0x0F, dir);
	}else if (
		f.param_type == CPT_FACEDIR_SIMPLE
		|| f.param_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param1, dir);
	}
	if (dir == v3s16(0,-1,0)) {
		dir_i = 1;
	}else if(dir == v3s16(1,0,0)) {
		dir_i = 2;
	}else if(dir == v3s16(-1,0,0)) {
		dir_i = 3;
	}else if(dir == v3s16(0,0,1)) {
		dir_i = 4;
	}else if(dir == v3s16(0,0,-1)) {
		dir_i = 5;
	}
	u8 facedir = 0;
	if (f.param_type == CPT_FACEDIR_SIMPLE) {
		facedir = param1;
	}else if (f.param2_type == CPT_FACEDIR_SIMPLE) {
		facedir = (param2&0x0F);
	}
	if (dir_i == 0) {
		if (facedir == 1) { // -90
			return "^[transformR270";
		}else if (facedir == 2) { // 180
			return "^[transformR180";
		}else if (facedir == 3) { // 90
			return "^[transformR90";
		}
	}else if (dir_i == 1) {
		if (facedir == 1) { // -90
			return "^[transformR90";
		}else if (facedir == 2) { // 180
			return "^[transformR180";
		}else if (facedir == 3) { // 90
			return "^[transformR270";
		}
	}

	return "";
}
#endif
FaceText MapNode::getFaceText(v3s16 dir)
{
	s32 dir_i = 0;
	ContentFeatures &f = content_features(*this);
	if (
		f.param2_type == CPT_FACEDIR_SIMPLE
		|| f.param2_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param2&0x0F, dir);
	}else if (
		f.param_type == CPT_FACEDIR_SIMPLE
		|| f.param_type == CPT_FACEDIR_WALLMOUNT
	) {
		dir = facedir_rotate(param1, dir);
	}
	if (dir == v3s16(0,-1,0)) {
		dir_i = 1;
	}else if(dir == v3s16(1,0,0)) {
		dir_i = 2;
	}else if(dir == v3s16(-1,0,0)) {
		dir_i = 3;
	}else if(dir == v3s16(0,0,1)) {
		dir_i = 4;
	}else if(dir == v3s16(0,0,-1)) {
		dir_i = 5;
	}

	return f.facetexts[dir_i];
}

u8 MapNode::getMineral()
{
	if(content_features(*this).param_type == CPT_MINERAL)
	{
		return param1 & 0x0f;
	}

	return MINERAL_NONE;
}

u32 MapNode::serializedLength(u8 version)
{
	if (!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapNode format not supported");

	if (version <= 20)
		return 3;

	return 4;
}
void MapNode::serialize(u8 *dest, u8 version)
{
	if (!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapNode format not supported");

	// Translate to wanted version
	MapNode n_foreign = mapnode_translate_from_internal(*this, version);

	if (version <= 20) {
		u8 p0 = 0;
		dest[1] = n_foreign.param1;
		dest[2] = n_foreign.param2;
		if (content < 0x80) {
			p0 = content;
		}else{
			p0 = content>>4;
			dest[2] &= ~(0xF0);
			dest[2] |= (content&0x0F)<<4;
		}
		dest[0] = p0;
	}else{
		dest[0] = (n_foreign.content&0xFF00)>>8;
		dest[1] = (n_foreign.content&0xFF);
		dest[2] = n_foreign.param1;
		dest[3] = n_foreign.param2;
	}
}
void MapNode::deSerialize(u8 *source, u8 version)
{
	if (!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapNode format not supported");

	if (version <= 20) {
		if (source[0] < 0x80) {
			content = source[0];
			param2 = source[2];
		}else{
			content = (source[0]<<4) + (source[2]>>4);
			param2 = (source[2]&0x0F);
		}
		param1 = source[1];
	}else{
		content = (source[0]<<8) | source[1];
		param1 = source[2];
		param2 = source[3];
	}

	// Translate to our known version
	*this = mapnode_translate_to_internal(*this, version);
}

/*
	Gets lighting value at face of node

	Parameters must consist of air and !air.
	Order doesn't matter.

	If either of the nodes doesn't exist, light is 0.

	parameters:
		daynight_ratio: 0...1000
		n: getNodeParent(p)
		n2: getNodeParent(p + face_dir)
		face_dir: axis oriented unit vector from p to p2

	returns encoded light value.
*/
u8 getFaceLight(u32 daynight_ratio, MapNode n, MapNode n2,
		v3s16 face_dir)
{
	try{
		u8 light;
		u8 l1 = n.getLightBlend(daynight_ratio);
		u8 l2 = n2.getLightBlend(daynight_ratio);
		if(l1 > l2)
			light = l1;
		else
			light = l2;

		// Make some nice difference to different sides

		// This makes light come from a corner
		/*if(face_dir.X == 1 || face_dir.Z == 1 || face_dir.Y == -1)
			light = diminish_light(diminish_light(light));
		else if(face_dir.X == -1 || face_dir.Z == -1)
			light = diminish_light(light);*/

		// All neighboring faces have different shade (like in minecraft)
		if(face_dir.X == 1 || face_dir.X == -1 || face_dir.Y == -1)
			light = diminish_light(diminish_light(light));
		else if(face_dir.Z == 1 || face_dir.Z == -1)
			light = diminish_light(light);

		return light;
	}
	catch(InvalidPositionException &e)
	{
		return 0;
	}
}

u8 face_light(MapNode n, MapNode n2, v3s16 face_dir)
{
	u8 ld = n.getLight(LIGHTBANK_DAY);
	u8 ld2 = n2.getLight(LIGHTBANK_DAY);
	u8 ln = n.getLight(LIGHTBANK_NIGHT);
	u8 ln2 = n2.getLight(LIGHTBANK_NIGHT);

	if (n2.getContent() == CONTENT_IGNORE) {
		if (ld > 0) {
			ld2 = ld;
		}else{
			ld2 = LIGHT_MAX;
		}
		ln2 = ln;
	}

	if (ld2 > ld)
		ld = ld2;
	if (ln2 > ln)
		ln = ln2;

	if (face_dir.X == 1 || face_dir.X == -1 || face_dir.Y == -1) {
		ld = MYMAX(0,ld-2);
		ln = MYMAX(0,ln-2);
	}else if(face_dir.Z == 1 || face_dir.Z == -1) {
		ld = MYMAX(0,ld-1);
		ln = MYMAX(0,ln-1);
	}

	return (ln<<4)|ld;
}


