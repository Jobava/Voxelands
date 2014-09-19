/*
Minetest-c55
Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>

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

std::vector<aabb3f> transformNodeBox(MapNode &n,
		const std::vector<aabb3f> &nodebox)
{
	std::vector<aabb3f> boxes;
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
	for(std::vector<aabb3f>::const_iterator
		i = nodebox.begin();
		i != nodebox.end(); i++)
	{
		aabb3f box = *i;
		if (facedir == 1) {
			box.MinEdge.rotateXZBy(-90);
			box.MaxEdge.rotateXZBy(-90);
			box.repair();
		}else if (facedir == 2) {
			box.MinEdge.rotateXZBy(180);
			box.MaxEdge.rotateXZBy(180);
			box.repair();
		}else if (facedir == 3) {
			box.MinEdge.rotateXZBy(90);
			box.MaxEdge.rotateXZBy(90);
			box.repair();
		}else if (facedir == 4) {
			box.MinEdge.rotateXYBy(-90);
			box.MaxEdge.rotateXYBy(-90);
			box.repair();
		}else if (facedir == 5) {
			box.MinEdge.rotateXYBy(90);
			box.MaxEdge.rotateXYBy(90);
			box.repair();
		}
		boxes.push_back(box);
	}
	return boxes;
}

std::vector<aabb3f> ContentFeatures::getNodeBoxes(MapNode &n) const
{
        return transformNodeBox(n, nodeboxes);
}

std::vector<aabb3f> ContentFeatures::getWieldNodeBoxes() const
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
void init_mapnode(video::IVideoDriver* driver)
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
	for(u16 i=0; i<MAX_CONTENT+1; i++)
	{
		if(i == CONTENT_IGNORE || i == CONTENT_AIR)
			continue;
		ContentFeatures *f = &g_content_features[i];
		f->setAllTextures("unknown_block.png");
		f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	}

	// Make CONTENT_IGNORE to not block the view when occlusion culling
	content_features(CONTENT_IGNORE).solidness = 0;

	/*
		Initialize mapnode content
	*/
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Base MapNodes");
#endif
	content_mapnode_init(repeat);
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Circuit MapNodes");
#endif
	content_mapnode_circuit(repeat);
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Plant MapNodes");
#endif
	content_mapnode_plants(repeat);
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Farming MapNodes");
#endif
	content_mapnode_farm(repeat);
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Decorative MapNodes");
#endif
	content_mapnode_furniture(repeat);
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Interactive MapNodes");
#endif
	content_mapnode_door(repeat);
#ifndef SERVER
	drawLoadingScreen(driver,"Loading Special MapNodes");
#endif
	content_mapnode_stair(repeat);
	content_mapnode_slab(repeat);
	content_mapnode_special(repeat);
}

/*
	Nodes make a face if contents differ and solidness differs.
	Return value:
		0: No face
		1: Face uses m1's content
		2: Face uses m2's content
	equivalent: Whether the blocks share the same face (eg. water and glass)
*/
u8 face_contents(content_t m1, content_t m2, bool *equivalent)
{
	*equivalent = false;

	if (m1 == CONTENT_IGNORE || m2 == CONTENT_IGNORE)
		return 0;

	ContentFeatures *f1 = &content_features(m1);
	ContentFeatures *f2 = &content_features(m2);

	bool contents_differ = (m1 != m2);

	// Contents don't differ for different forms of same liquid
	if (f1->liquid_type != LIQUID_NONE && f2->liquid_type != LIQUID_NONE && f1->liquid_alternative_flowing == f2->liquid_alternative_flowing)
		contents_differ = false;

	u8 c1 = f1->solidness;
	u8 c2 = f2->solidness;

	bool solidness_differs = (c1 != c2);
	bool makes_face = contents_differ && solidness_differs;

	if (makes_face == false)
		return 0;

	if (c1 == 0)
		c1 = f1->visual_solidness;
	if (c2 == 0)
		c2 = f2->visual_solidness;

	if (c1 == c2) {
		*equivalent = true;
		// If same solidness, liquid takes precense
		if (f1->liquid_type != LIQUID_NONE)
			return 1;
		if (f2->liquid_type != LIQUID_NONE)
			return 2;
	}

	if (c1 > c2)
		return 1;

	return 2;
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
	v3s16 newdir;
	if(facedir==0) // Same
		newdir = v3s16(dir.X, dir.Y, dir.Z);
	else if(facedir == 1) // Face is taken from rotXZccv(-90)
		newdir = v3s16(-dir.Z, dir.Y, dir.X);
	else if(facedir == 2) // Face is taken from rotXZccv(180)
		newdir = v3s16(-dir.X, dir.Y, -dir.Z);
	else if(facedir == 3) // Face is taken from rotXZccv(90)
		newdir = v3s16(dir.Z, dir.Y, -dir.X);
	else
		newdir = dir;
	return newdir;
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

#ifndef SERVER
TileSpec MapNode::getTileFrom(v3s16 dir, TileSpec raw_spec[6])
{
	TileSpec spec;
	s32 dir_i = 0;

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

	if(dir == v3s16(0,-1,0))
		dir_i = 1;
	else if(dir == v3s16(1,0,0))
		dir_i = 2;
	else if(dir == v3s16(-1,0,0))
		dir_i = 3;
	else if(dir == v3s16(0,0,1))
		dir_i = 4;
	else if(dir == v3s16(0,0,-1))
		dir_i = 5;

	spec = raw_spec[dir_i];

	/*
		If it contains some mineral, change texture id
	*/
	if(content_features(*this).param_type == CPT_MINERAL && g_texturesource)
	{
		u8 mineral = getMineral();
		std::string mineral_texture_name = mineral_features(mineral).texture;
		if(mineral_texture_name != "")
		{
			u32 orig_id = spec.texture.id;
			std::string texture_name = g_texturesource->getTextureName(orig_id);
			//texture_name += "^blit:";
			texture_name += "^";
			texture_name += mineral_texture_name;
			u32 new_id = g_texturesource->getTextureId(texture_name);
			spec.texture = g_texturesource->getTexture(new_id);
		}
	}
	if (content_features(*this).rotate_tile_with_nodebox) {
		u8 facedir = 0;
		if (content_features(*this).param_type == CPT_FACEDIR_SIMPLE) {
			facedir = param1;
		}else if (content_features(*this).param2_type == CPT_FACEDIR_SIMPLE) {
			facedir = (param2&0x0F);
		}
		if (dir_i == 0) {
			if (facedir == 1) { // -90
				std::string name = g_texturesource->getTextureName(spec.texture.id);
				name += "^[transformR270";
				spec.texture = g_texturesource->getTexture(name);
			}else if (facedir == 2) { // 180
				std::string name = g_texturesource->getTextureName(spec.texture.id);
				name += "^[transformR180";
				spec.texture = g_texturesource->getTexture(name);
			}else if (facedir == 3) { // 90
				std::string name = g_texturesource->getTextureName(spec.texture.id);
				name += "^[transformR90";
				spec.texture = g_texturesource->getTexture(name);
			}
		}else if (dir_i == 1) {
			if (facedir == 1) { // -90
				std::string name = g_texturesource->getTextureName(spec.texture.id);
				name += "^[transformR90";
				spec.texture = g_texturesource->getTexture(name);
			}else if (facedir == 2) { // 180
				std::string name = g_texturesource->getTextureName(spec.texture.id);
				name += "^[transformR180";
				spec.texture = g_texturesource->getTexture(name);
			}else if (facedir == 3) { // 90
				std::string name = g_texturesource->getTextureName(spec.texture.id);
				name += "^[transformR270";
				spec.texture = g_texturesource->getTexture(name);
			}
		}
	}

	return spec;
}
#endif

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
	if(!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapNode format not supported");

	if(version == 0)
		return 1;
	else if(version <= 9)
		return 2;
	else
		return 3;
}
void MapNode::serialize(u8 *dest, u8 version)
{
	if(!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapNode format not supported");

	// Translate to wanted version
	MapNode n_foreign = mapnode_translate_from_internal(*this, version);

	u8 actual_param0 = n_foreign.param0;

	// Convert special values from new version to old
	if(version <= 18)
	{
		// In these versions, CONTENT_IGNORE and CONTENT_AIR
		// are 255 and 254
		if(actual_param0 == CONTENT_IGNORE)
			actual_param0 = 255;
		else if(actual_param0 == CONTENT_AIR)
			actual_param0 = 254;
	}

	if(version == 0)
	{
		dest[0] = actual_param0;
	}
	else if(version <= 9)
	{
		dest[0] = actual_param0;
		dest[1] = n_foreign.param1;
	}
	else
	{
		dest[0] = actual_param0;
		dest[1] = n_foreign.param1;
		dest[2] = n_foreign.param2;
	}
}
void MapNode::deSerialize(u8 *source, u8 version)
{
	if(!ser_ver_supported(version))
		throw VersionMismatchException("ERROR: MapNode format not supported");

	if(version == 0)
	{
		param0 = source[0];
	}
	else if(version == 1)
	{
		param0 = source[0];
		// This version doesn't support saved lighting
		if(light_propagates() || light_source() > 0)
			param1 = 0;
		else
			param1 = source[1];
	}
	else if(version <= 9)
	{
		param0 = source[0];
		param1 = source[1];
	}
	else
	{
		param0 = source[0];
		param1 = source[1];
		param2 = source[2];
	}

	// Convert special values from old version to new
	if(version <= 18)
	{
		// In these versions, CONTENT_IGNORE and CONTENT_AIR
		// are 255 and 254
		if(param0 == 255)
			param0 = CONTENT_IGNORE;
		else if(param0 == 254)
			param0 = CONTENT_AIR;
	}
	// version 19 is fucked up with sometimes the old values and sometimes not
	if(version == 19)
	{
		if(param0 == 255)
			param0 = CONTENT_IGNORE;
		else if(param0 == 254)
			param0 = CONTENT_AIR;
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


