/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mesh.h
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

#ifndef MESH_HEADER
#define MESH_HEADER

#include "common_irrlicht.h"
#include <IAnimatedMesh.h>
#include <SAnimatedMesh.h>
#include <ICameraSceneNode.h>
#include <IMesh.h>
#include <IImage.h>
#include <IrrlichtDevice.h>
#include <IMeshSceneNode.h>
#include <SMesh.h>
#include <ISceneManager.h>
#include <IMeshBuffer.h>
#include <SMeshBuffer.h>
#include <string>
#include <vector>
#include "mapnode.h"

/*
	Create a new cube mesh.
	Vertices are at (+-scale.X/2, +-scale.Y/2, +-scale.Z/2).

	The resulting mesh has 6 materials (up, down, right, left, back, front)
	which must be defined by the caller.
*/
scene::IAnimatedMesh* createNodeBoxMesh(std::vector<NodeBox> nodeboxes, v3f scale);
scene::IAnimatedMesh* createCubeMesh(v3f scale);
#ifndef SERVER
scene::IAnimatedMesh* createModelMesh(scene::ISceneManager* smgr, std::string model, bool unique);
#endif

/*
	Create a new extruded mesh from a texture.
	Maximum bounding box is (+-scale.X/2, +-scale.Y/2, +-scale.Z).
	Thickness is in Z direction.

	The resulting mesh has 1 material which must be defined by the caller.
*/
scene::IAnimatedMesh* createExtrudedMesh(video::ITexture *texture,
		video::IVideoDriver *driver, v3f scale);

/*
	Multiplies each vertex coordinate by the specified scaling factors
	(componentwise vector multiplication).
*/
void scaleMesh(scene::IMesh *mesh, v3f scale);

/*
	Translate each vertex coordinate by the specified vector.
*/
void translateMesh(scene::IMesh *mesh, v3f vec);

/*
	Set a constant color for all vertices in the mesh
*/
void setMeshColor(scene::IMesh *mesh, const video::SColor &color);

/*
	Set the color of all vertices in the mesh.
	For each vertex, determine the largest absolute entry in
	the normal vector, and choose one of colorX, colorY or
	colorZ accordingly.
*/
void setMeshColorByNormalXYZ(scene::IMesh *mesh,
		const video::SColor &colorX,
		const video::SColor &colorY,
		const video::SColor &colorZ);

/*
	Render a mesh to a texture.
	Returns NULL if render-to-texture failed.
*/
video::ITexture *generateTextureFromMesh(scene::IMesh *mesh,
		IrrlichtDevice *device,
		core::dimension2d<u32> dim,
		std::string texture_name,
		v3f camera_position,
		v3f camera_lookat,
		core::CMatrix4<f32> camera_projection_matrix,
		video::SColorf ambient_light,
		v3f light_position,
		video::SColorf light_color,
		f32 light_radius);


/*
	A scene node that displays a 2D mesh extruded into the third dimension,
	to add an illusion of depth.

	Since this class was created to display the wielded tool of the local
	player, and only tools and items are rendered like this (but not solid
	content like stone and mud, which are shown as cubes), the option to
	draw a textured cube instead is provided.
 */
class ExtrudedSpriteSceneNode: public scene::ISceneNode
{
public:
	ExtrudedSpriteSceneNode(
		scene::ISceneNode* parent,
		scene::ISceneManager* mgr,
		s32 id = -1,
		const v3f& position = v3f(0,0,0),
		const v3f& rotation = v3f(0,0,0),
		const v3f& scale = v3f(1,1,1));
	~ExtrudedSpriteSceneNode();

	void setSprite(video::ITexture* texture);
	void setCube(const TileSpec tiles[6]);
	void setNodeBox(content_t c);
	void setArm(video::ITexture *texture = NULL);

	f32 getSpriteThickness() const { return m_thickness; }
	void setSpriteThickness(f32 thickness);

	void updateLight(u8 light);

	void removeSpriteFromCache(video::ITexture* texture);

	virtual const core::aabbox3d<f32>& getBoundingBox() const;
	virtual void OnRegisterSceneNode();
	virtual void render();

private:
	scene::IMeshSceneNode* m_meshnode;
	f32 m_thickness;
	scene::IMesh* m_cubemesh;
	bool m_is_cube;
	u8 m_light;

	// internal extrusion helper methods
	io::path getExtrudedName(video::ITexture* texture);
};

#endif
