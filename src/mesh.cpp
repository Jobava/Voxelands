/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* mesh.cpp
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

#include "mesh.h"
#include "log.h"
#include "utility.h"
#include <cassert>
#include <iostream>
#include <vector>
#include <IAnimatedMesh.h>
#include <SAnimatedMesh.h>
#include <ICameraSceneNode.h>
#include "constants.h"
#include "path.h"
#include <IMeshLoader.h>
#include <IMeshCache.h>
#include "main.h"
#include "settings.h"

// In Irrlicht 1.8 the signature of ITexture::lock was changed from
// (bool, u32) to (E_TEXTURE_LOCK_MODE, u32).
#if IRRLICHT_VERSION_MAJOR == 1 && IRRLICHT_VERSION_MINOR <= 7
#define MY_ETLM_READ_ONLY true
#else
#define MY_ETLM_READ_ONLY video::ETLM_READ_ONLY
#endif

scene::IAnimatedMesh* createNodeBoxMesh(std::vector<NodeBox> nodeboxes, v3f scale)
{
	video::SColor c(255,255,255,255);

	scene::SMesh *mesh = new scene::SMesh();
	for (std::vector<NodeBox>::const_iterator n = nodeboxes.begin(); n != nodeboxes.end(); n++) {
		NodeBox box = *n;
		v3f min = box.m_box.MinEdge;
		v3f max = box.m_box.MaxEdge;
		// Compute texture coords
		f32 tx1 = (box.m_box.MinEdge.X/BS)+0.5;
		f32 ty1 = (box.m_box.MinEdge.Y/BS)+0.5;
		f32 tz1 = (box.m_box.MinEdge.Z/BS)+0.5;
		f32 tx2 = (box.m_box.MaxEdge.X/BS)+0.5;
		f32 ty2 = (box.m_box.MaxEdge.Y/BS)+0.5;
		f32 tz2 = (box.m_box.MaxEdge.Z/BS)+0.5;
		f32 txc[24] = {
			// up
			tx1, 1-tz2, tx2, 1-tz1,
			// down
			tx1, tz1, tx2, tz2,
			// right
			tz1, 1-ty2, tz2, 1-ty1,
			// left
			1-tz2, 1-ty2, 1-tz1, 1-ty1,
			// back
			1-tx2, 1-ty2, 1-tx1, 1-ty1,
			// front
			tx1, 1-ty2, tx2, 1-ty1,
		};
		video::S3DVertex vertices[24] = {
			// up
			video::S3DVertex(min.X/BS,max.Y/BS,max.Z/BS, 0,1,0, c, txc[0],txc[1]),
			video::S3DVertex(max.X/BS,max.Y/BS,max.Z/BS, 0,1,0, c, txc[2],txc[1]),
			video::S3DVertex(max.X/BS,max.Y/BS,min.Z/BS, 0,1,0, c, txc[2],txc[3]),
			video::S3DVertex(min.X/BS,max.Y/BS,min.Z/BS, 0,1,0, c, txc[0],txc[3]),
			// down
			video::S3DVertex(min.X/BS,min.Y/BS,min.Z/BS, 0,-1,0, c, txc[4],txc[5]),
			video::S3DVertex(max.X/BS,min.Y/BS,min.Z/BS, 0,-1,0, c, txc[6],txc[5]),
			video::S3DVertex(max.X/BS,min.Y/BS,max.Z/BS, 0,-1,0, c, txc[6],txc[7]),
			video::S3DVertex(min.X/BS,min.Y/BS,max.Z/BS, 0,-1,0, c, txc[4],txc[7]),
			// right
			video::S3DVertex(max.X/BS,max.Y/BS,min.Z/BS, 1,0,0, c, txc[ 8],txc[9]),
			video::S3DVertex(max.X/BS,max.Y/BS,max.Z/BS, 1,0,0, c, txc[10],txc[9]),
			video::S3DVertex(max.X/BS,min.Y/BS,max.Z/BS, 1,0,0, c, txc[10],txc[11]),
			video::S3DVertex(max.X/BS,min.Y/BS,min.Z/BS, 1,0,0, c, txc[ 8],txc[11]),
			// left
			video::S3DVertex(min.X/BS,max.Y/BS,max.Z/BS, -1,0,0, c, txc[12],txc[13]),
			video::S3DVertex(min.X/BS,max.Y/BS,min.Z/BS, -1,0,0, c, txc[14],txc[13]),
			video::S3DVertex(min.X/BS,min.Y/BS,min.Z/BS, -1,0,0, c, txc[14],txc[15]),
			video::S3DVertex(min.X/BS,min.Y/BS,max.Z/BS, -1,0,0, c, txc[12],txc[15]),
			// back
			video::S3DVertex(max.X/BS,max.Y/BS,max.Z/BS, 0,0,1, c, txc[16],txc[17]),
			video::S3DVertex(min.X/BS,max.Y/BS,max.Z/BS, 0,0,1, c, txc[18],txc[17]),
			video::S3DVertex(min.X/BS,min.Y/BS,max.Z/BS, 0,0,1, c, txc[18],txc[19]),
			video::S3DVertex(max.X/BS,min.Y/BS,max.Z/BS, 0,0,1, c, txc[16],txc[19]),
			// front
			video::S3DVertex(min.X/BS,max.Y/BS,min.Z/BS, 0,0,-1, c, txc[20],txc[21]),
			video::S3DVertex(max.X/BS,max.Y/BS,min.Z/BS, 0,0,-1, c, txc[22],txc[21]),
			video::S3DVertex(max.X/BS,min.Y/BS,min.Z/BS, 0,0,-1, c, txc[22],txc[23]),
			video::S3DVertex(min.X/BS,min.Y/BS,min.Z/BS, 0,0,-1, c, txc[20],txc[23]),
		};

		if (box.m_angle != v3s16(0,0,0)) {
			for (s32 j=0; j<24; j++) {
				if (box.m_angle.Y)
					vertices[j].Pos.rotateXZBy(box.m_angle.Y);
				if (box.m_angle.X)
					vertices[j].Pos.rotateYZBy(box.m_angle.X);
				if (box.m_angle.Z)
					vertices[j].Pos.rotateXYBy(box.m_angle.Z);
			}
		}

		u16 indices[6] = {0,1,2,2,3,0};
		for (u32 i=0; i<6; ++i) {
			scene::IMeshBuffer *buf = new scene::SMeshBuffer();
			buf->append(vertices + 4 * i, 4, indices, 6);
			// Set default material
			buf->getMaterial().setFlag(video::EMF_LIGHTING, false);
			buf->getMaterial().setFlag(video::EMF_BILINEAR_FILTER, false);
			buf->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
			// Add mesh buffer to mesh
			mesh->addMeshBuffer(buf);
			buf->drop();
		}
	}

	scene::SAnimatedMesh *anim_mesh = new scene::SAnimatedMesh(mesh);
	mesh->drop();
	scaleMesh(anim_mesh, scale);  // also recalculates bounding box
	return anim_mesh;
}

scene::IAnimatedMesh* createCubeMesh(v3f scale)
{
	std::vector<NodeBox> nodeboxes;
	nodeboxes.clear();
	nodeboxes.push_back(NodeBox(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));

	return createNodeBoxMesh(nodeboxes,scale);
}

#ifndef SERVER
scene::IAnimatedMesh* createModelMesh(scene::ISceneManager* smgr, std::string model, bool unique)
{
	std::string model_path = getModelPath(model);
	scene::IAnimatedMesh* mesh = smgr->getMesh(model_path.c_str());
	if (mesh && !unique)
		return mesh;
#if (IRRLICHT_VERSION_MAJOR >= 1 && IRRLICHT_VERSION_MINOR >= 8) || IRRLICHT_VERSION_MAJOR >= 2
	// irrlicht 1.8+ we just manually load the mesh
	scene::IMeshLoader *loader;
	u32 lc = smgr->getMeshLoaderCount();
	io::IReadFile* file = smgr->getFileSystem()->createAndOpenFile(model_path.c_str());
	if (!file)
		return 0;
	for (u32 i=0; i<lc; i++) {
		loader = smgr->getMeshLoader(i);
		file->seek(0);
		if (loader->isALoadableFileExtension(model.c_str())) {
			mesh = loader->createMesh(file);
			break;
		}
	}
	file->drop();
#else
	// irrlicht 1.7 doesn't have a meshloader interface, so rename
	// the previous mesh from this file to force scenemanager to
	// load a fresh mesh
	scene::IMeshCache *mc = smgr->getMeshCache();
	std::string mp;
	int i=0;
	do{
		mp = model_path + "-" + itos(i++);
	} while (mc->getMeshByName(mp.c_str()));

	mc->renameMesh(mesh,mp.c_str());

	mesh = smgr->getMesh(model_path.c_str());
#endif
	return mesh;
}
#endif

static scene::IAnimatedMesh* extrudeARGB(u32 twidth, u32 theight, u8 *data)
{
	const s32 argb_wstep = 4 * twidth;
	const s32 alpha_threshold = 64;

	scene::IMeshBuffer *buf = new scene::SMeshBuffer();
	video::SColor c(255,255,255,255);

	// Front and back
	{
		video::S3DVertex vertices[8] = {
			video::S3DVertex(-0.5,-0.5,-0.5, 0,0,-1, c, 0,1),
			video::S3DVertex(-0.5,+0.5,-0.5, 0,0,-1, c, 0,0),
			video::S3DVertex(+0.5,+0.5,-0.5, 0,0,-1, c, 1,0),
			video::S3DVertex(+0.5,-0.5,-0.5, 0,0,-1, c, 1,1),
			video::S3DVertex(+0.5,-0.5,+0.5, 0,0,+1, c, 1,1),
			video::S3DVertex(+0.5,+0.5,+0.5, 0,0,+1, c, 1,0),
			video::S3DVertex(-0.5,+0.5,+0.5, 0,0,+1, c, 0,0),
			video::S3DVertex(-0.5,-0.5,+0.5, 0,0,+1, c, 0,1),
		};
		u16 indices[12] = {0,1,2,2,3,0,4,5,6,6,7,4};
		buf->append(vertices, 8, indices, 12);
	}

	// "Interior"
	// (add faces where a solid pixel is next to a transparent one)
	u8 *solidity = new u8[(twidth+2) * (theight+2)];
	u32 wstep = twidth + 2;
	for (u32 y = 0; y < theight + 2; ++y) {
		u8 *scanline = solidity + y * wstep;
		if (y == 0 || y == theight + 1) {
			for (u32 x = 0; x < twidth + 2; ++x) {
				scanline[x] = 0;
			}
		}else{
			scanline[0] = 0;
			u8 *argb_scanline = data + (y - 1) * argb_wstep;
			for (u32 x = 0; x < twidth; ++x) {
				scanline[x+1] = (argb_scanline[x*4+3] >= alpha_threshold);
			}
			scanline[twidth + 1] = 0;
		}
	}

	// without this, there would be occasional "holes" in the mesh
	f32 eps = 0.01;

	for (u32 y = 0; y <= theight; ++y) {
		u8 *scanline = solidity + y * wstep + 1;
		for (u32 x = 0; x <= twidth; ++x) {
			if (scanline[x] && !scanline[x + wstep]) {
				u32 xx = x + 1;
				while (scanline[xx] && !scanline[xx + wstep]) {
					++xx;
				}
				f32 vx1 = (x - eps) / (f32) twidth - 0.5;
				f32 vx2 = (xx + eps) / (f32) twidth - 0.5;
				f32 vy = 0.5 - (y - eps) / (f32) theight;
				f32 tx1 = x / (f32) twidth;
				f32 tx2 = xx / (f32) twidth;
				f32 ty = (y - 0.5) / (f32) theight;
				video::S3DVertex vertices[8] = {
					video::S3DVertex(vx1,vy,-0.5, 0,-1,0, c, tx1,ty),
					video::S3DVertex(vx2,vy,-0.5, 0,-1,0, c, tx2,ty),
					video::S3DVertex(vx2,vy,+0.5, 0,-1,0, c, tx2,ty),
					video::S3DVertex(vx1,vy,+0.5, 0,-1,0, c, tx1,ty),
				};
				u16 indices[6] = {0,1,2,2,3,0};
				buf->append(vertices, 4, indices, 6);
				x = xx - 1;
			}
			if (!scanline[x] && scanline[x + wstep]) {
				u32 xx = x + 1;
				while (!scanline[xx] && scanline[xx + wstep]) {
					++xx;
				}
				f32 vx1 = (x - eps) / (f32) twidth - 0.5;
				f32 vx2 = (xx + eps) / (f32) twidth - 0.5;
				f32 vy = 0.5 - (y + eps) / (f32) theight;
				f32 tx1 = x / (f32) twidth;
				f32 tx2 = xx / (f32) twidth;
				f32 ty = (y + 0.5) / (f32) theight;
				video::S3DVertex vertices[8] = {
					video::S3DVertex(vx1,vy,-0.5, 0,1,0, c, tx1,ty),
					video::S3DVertex(vx1,vy,+0.5, 0,1,0, c, tx1,ty),
					video::S3DVertex(vx2,vy,+0.5, 0,1,0, c, tx2,ty),
					video::S3DVertex(vx2,vy,-0.5, 0,1,0, c, tx2,ty),
				};
				u16 indices[6] = {0,1,2,2,3,0};
				buf->append(vertices, 4, indices, 6);
				x = xx - 1;
			}
		}
	}

	for (u32 x = 0; x <= twidth; ++x) {
		u8 *scancol = solidity + x + wstep;
		for (u32 y = 0; y <= theight; ++y) {
			if (scancol[y * wstep] && !scancol[y * wstep + 1]) {
				u32 yy = y + 1;
				while (scancol[yy * wstep] && !scancol[yy * wstep + 1]) {
					++yy;
				}
				f32 vx = (x - eps) / (f32) twidth - 0.5;
				f32 vy1 = 0.5 - (y - eps) / (f32) theight;
				f32 vy2 = 0.5 - (yy + eps) / (f32) theight;
				f32 tx = (x - 0.5) / (f32) twidth;
				f32 ty1 = y / (f32) theight;
				f32 ty2 = yy / (f32) theight;
				video::S3DVertex vertices[8] = {
					video::S3DVertex(vx,vy1,-0.5, 1,0,0, c, tx,ty1),
					video::S3DVertex(vx,vy1,+0.5, 1,0,0, c, tx,ty1),
					video::S3DVertex(vx,vy2,+0.5, 1,0,0, c, tx,ty2),
					video::S3DVertex(vx,vy2,-0.5, 1,0,0, c, tx,ty2),
				};
				u16 indices[6] = {0,1,2,2,3,0};
				buf->append(vertices, 4, indices, 6);
				y = yy - 1;
			}
			if (!scancol[y * wstep] && scancol[y * wstep + 1]) {
				u32 yy = y + 1;
				while (!scancol[yy * wstep] && scancol[yy * wstep + 1]) {
					++yy;
				}
				f32 vx = (x + eps) / (f32) twidth - 0.5;
				f32 vy1 = 0.5 - (y - eps) / (f32) theight;
				f32 vy2 = 0.5 - (yy + eps) / (f32) theight;
				f32 tx = (x + 0.5) / (f32) twidth;
				f32 ty1 = y / (f32) theight;
				f32 ty2 = yy / (f32) theight;
				video::S3DVertex vertices[8] = {
					video::S3DVertex(vx,vy1,-0.5, -1,0,0, c, tx,ty1),
					video::S3DVertex(vx,vy2,-0.5, -1,0,0, c, tx,ty2),
					video::S3DVertex(vx,vy2,+0.5, -1,0,0, c, tx,ty2),
					video::S3DVertex(vx,vy1,+0.5, -1,0,0, c, tx,ty1),
				};
				u16 indices[6] = {0,1,2,2,3,0};
				buf->append(vertices, 4, indices, 6);
				y = yy - 1;
			}
		}
	}

	delete[] solidity;

	// Add to mesh
	scene::SMesh *mesh = new scene::SMesh();
	mesh->addMeshBuffer(buf);
	buf->drop();
	scene::SAnimatedMesh *anim_mesh = new scene::SAnimatedMesh(mesh);
	mesh->drop();
	return anim_mesh;
}

scene::IAnimatedMesh* createExtrudedMesh(video::ITexture *texture,
		video::IVideoDriver *driver, v3f scale)
{
	scene::IAnimatedMesh *mesh = NULL;
	core::dimension2d<u32> size = texture->getSize();
	video::ECOLOR_FORMAT format = texture->getColorFormat();
	if (format == video::ECF_A8R8G8B8) {
		// Texture is in the correct color format, we can pass it
		// to extrudeARGB right away.
		void *data = texture->lock(MY_ETLM_READ_ONLY);
		if (data == NULL)
			return NULL;
		mesh = extrudeARGB(size.Width, size.Height, (u8*) data);
		texture->unlock();
	}else{
		video::IImage *img1 = driver->createImageFromData(format, size, texture->lock(MY_ETLM_READ_ONLY));
		if (img1 == NULL)
			return NULL;

		// img1 is in the texture's color format, convert to 8-bit ARGB
		video::IImage *img2 = driver->createImage(video::ECF_A8R8G8B8, size);
		if (img2 != NULL) {
			img1->copyTo(img2);

			mesh = extrudeARGB(size.Width, size.Height, (u8*) img2->lock());
			img2->unlock();
			img2->drop();
		}
		img1->drop();
	}

	// Set default material
	if (mesh && mesh->getMeshBufferCount() > 0) {
		scene::IMeshBuffer *buf = mesh->getMeshBuffer(0);
		if (buf) {
			buf->getMaterial().setTexture(0, texture);
			buf->getMaterial().setFlag(video::EMF_LIGHTING, false);
			buf->getMaterial().setFlag(video::EMF_BILINEAR_FILTER, false);
			buf->getMaterial().MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
		}
	}

	scaleMesh(mesh, scale);  // also recalculates bounding box
	return mesh;
}

void scaleMesh(scene::IMesh *mesh, v3f scale)
{
	if (mesh == NULL)
		return;

	core::aabbox3d<f32> bbox;
	bbox.reset(0,0,0);

	u16 mc = mesh->getMeshBufferCount();
	for (u16 j=0; j<mc; j++) {
		scene::IMeshBuffer *buf = mesh->getMeshBuffer(j);
		if (!buf)
			continue;
		u16 vc = buf->getVertexCount();
		if (!vc)
			continue;
		video::S3DVertex *vertices = (video::S3DVertex*)buf->getVertices();
		if (!vertices)
			continue;
		for (u16 i=0; i<vc; i++) {
			vertices[i].Pos *= scale;
		}
		buf->recalculateBoundingBox();

		// calculate total bounding box
		if (j == 0) {
			bbox = buf->getBoundingBox();
		}else{
			bbox.addInternalBox(buf->getBoundingBox());
		}
	}
	mesh->setBoundingBox(bbox);
}

void translateMesh(scene::IMesh *mesh, v3f vec)
{
	if (mesh == NULL)
		return;

	core::aabbox3d<f32> bbox;
	bbox.reset(0,0,0);

	u16 mc = mesh->getMeshBufferCount();
	for (u16 j=0; j<mc; j++) {
		scene::IMeshBuffer *buf = mesh->getMeshBuffer(j);
		if (!buf)
			continue;
		u16 vc = buf->getVertexCount();
		if (!vc)
			continue;
		video::S3DVertex *vertices = (video::S3DVertex*)buf->getVertices();
		if (!vertices)
			continue;
		for (u16 i=0; i<vc; i++) {
			vertices[i].Pos += vec;
		}
		buf->recalculateBoundingBox();

		// calculate total bounding box
		if (j == 0) {
			bbox = buf->getBoundingBox();
		}else{
			bbox.addInternalBox(buf->getBoundingBox());
		}
	}
	mesh->setBoundingBox(bbox);
}

void setMeshColor(scene::IMesh *mesh, const video::SColor &color)
{
	if (mesh == NULL)
		return;

	u16 mc = mesh->getMeshBufferCount();
	for (u16 j=0; j<mc; j++) {
		scene::IMeshBuffer *buf = mesh->getMeshBuffer(j);
		if (!buf)
			continue;
		u16 vc = buf->getVertexCount();
		if (!vc)
			continue;
		video::S3DVertex *vertices = (video::S3DVertex*)buf->getVertices();
		if (!vertices)
			continue;
		for (u16 i=0; i<vc; i++) {
			vertices[i].Color = color;
		}
	}
}

void setMeshColorByNormalXYZ(scene::IMesh *mesh,
		const video::SColor &colorX,
		const video::SColor &colorY,
		const video::SColor &colorZ)
{
	if (mesh == NULL)
		return;

	u16 mc = mesh->getMeshBufferCount();
	for (u16 j=0; j<mc; j++) {
		scene::IMeshBuffer *buf = mesh->getMeshBuffer(j);
		if (!buf)
			continue;
		u16 vc = buf->getVertexCount();
		if (!vc)
			continue;
		video::S3DVertex *vertices = (video::S3DVertex*)buf->getVertices();
		if (!vertices)
			continue;
		for (u16 i=0; i<vc; i++) {
			f32 x = fabs(vertices[i].Normal.X);
			f32 y = fabs(vertices[i].Normal.Y);
			f32 z = fabs(vertices[i].Normal.Z);
			if (x >= y && x >= z) {
				vertices[i].Color = colorX;
			}else if (y >= z) {
				vertices[i].Color = colorY;
			}else{
				vertices[i].Color = colorZ;
			}
		}
	}
}

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
		f32 light_radius)
{
	video::IVideoDriver *driver = device->getVideoDriver();
	if(driver->queryFeature(video::EVDF_RENDER_TO_TARGET) == false)
	{
		static bool warned = false;
		if(!warned)
		{
			errorstream<<"generateTextureFromMesh(): EVDF_RENDER_TO_TARGET"
					" not supported."<<std::endl;
			warned = true;
		}
		return NULL;
	}

	// Create render target texture
	video::ITexture *rtt = driver->addRenderTargetTexture(
			dim, texture_name.c_str(), video::ECF_A8R8G8B8);
	if(rtt == NULL)
	{
		errorstream<<"generateTextureFromMesh(): addRenderTargetTexture"
				" returned NULL."<<std::endl;
		return NULL;
	}

	// Set render target
	driver->setRenderTarget(rtt, false, true, video::SColor(0,0,0,0));

	// Get a scene manager
	scene::ISceneManager *smgr_main = device->getSceneManager();
	assert(smgr_main);
	scene::ISceneManager *smgr = smgr_main->createNewSceneManager();
	assert(smgr);

	scene::IMeshSceneNode* meshnode = smgr->addMeshSceneNode(mesh, NULL, -1, v3f(0,0,0), v3f(0,0,0), v3f(1,1,1), true);
	meshnode->setMaterialFlag(video::EMF_LIGHTING, true);
	meshnode->setMaterialFlag(video::EMF_ANTI_ALIASING, true);
	meshnode->setMaterialFlag(video::EMF_BILINEAR_FILTER, true);

	scene::ICameraSceneNode* camera = smgr->addCameraSceneNode(0,
			camera_position, camera_lookat);
	// second parameter of setProjectionMatrix (isOrthogonal) is ignored
	camera->setProjectionMatrix(camera_projection_matrix, false);

	smgr->setAmbientLight(ambient_light);
	smgr->addLightSceneNode(0, light_position, light_color, light_radius);

	// Render scene
	driver->beginScene(true, true, video::SColor(0,0,0,0));
	smgr->drawAll();
	driver->endScene();

	// NOTE: The scene nodes should not be dropped, otherwise
	//       smgr->drop() segfaults
	/*cube->drop();
	camera->drop();
	light->drop();*/
	// Drop scene manager
	smgr->drop();

	// Unset render target
	driver->setRenderTarget(0, false, true, 0);

	return rtt;
}

ExtrudedSpriteSceneNode::ExtrudedSpriteSceneNode(
	scene::ISceneNode* parent,
	scene::ISceneManager* mgr,
	s32 id,
	const v3f& position,
	const v3f& rotation,
	const v3f& scale
):
	ISceneNode(parent, mgr, id, position, rotation, scale)
{
	m_meshnode = mgr->addMeshSceneNode(NULL, this, -1, v3f(0,0,0), v3f(0,0,0), v3f(1,1,1), true);
	m_thickness = 0.1;
	m_cubemesh = NULL;
	m_is_cube = false;
	m_light = LIGHT_MAX;
}

ExtrudedSpriteSceneNode::~ExtrudedSpriteSceneNode()
{
	removeChild(m_meshnode);
	if (m_cubemesh)
		m_cubemesh->drop();
}

void ExtrudedSpriteSceneNode::setSprite(video::ITexture* texture)
{
	const v3f sprite_scale(1.0,1.0, 1.0); // width, height, thickness

	if (texture == NULL) {
		m_meshnode->setVisible(false);
		return;
	}

	io::path name = getExtrudedName(texture);
	scene::IMeshCache* cache = SceneManager->getMeshCache();
	scene::IAnimatedMesh* mesh = cache->getMeshByName(name);
	if (mesh != NULL)
	{
		// Extruded texture has been found in cache.
		m_meshnode->setMesh(mesh);
	}
	else
	{
		// Texture was not yet extruded, do it now and save in cache
		mesh = createExtrudedMesh(texture, SceneManager->getVideoDriver(), sprite_scale);
		if (mesh == NULL)
		{
			dstream << "Warning: failed to extrude sprite" << std::endl;
			m_meshnode->setVisible(false);
			return;
		}
		cache->addMesh(name, mesh);
		m_meshnode->setMesh(mesh);
		mesh->drop();
	}

	m_meshnode->setScale(v3f(1, 1, m_thickness));
	m_meshnode->getMaterial(0).setTexture(0, texture);
	m_meshnode->getMaterial(0).setFlag(video::EMF_LIGHTING, false);
	m_meshnode->getMaterial(0).setFlag(video::EMF_BILINEAR_FILTER, false);
	m_meshnode->getMaterial(0).MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
	m_meshnode->setVisible(true);
	m_is_cube = false;
	updateLight(m_light);
}

void ExtrudedSpriteSceneNode::setCube(const TileSpec tiles[6])
{
	const v3f cube_scale(1.0, 1.0, 1.0);
	if (m_cubemesh)
		m_cubemesh->drop();

	m_cubemesh = createCubeMesh(cube_scale);

	m_meshnode->setMesh(m_cubemesh);
	m_meshnode->setScale(v3f(1));
	for (int i = 0; i < 6; ++i)
	{
		// Get the tile texture and atlas transformation
		video::ITexture* atlas = tiles[i].texture.atlas;
		v2f pos = tiles[i].texture.pos;
		v2f size = tiles[i].texture.size;

		// Set material flags and texture
		video::SMaterial& material = m_meshnode->getMaterial(i);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_BILINEAR_FILTER, false);
		tiles[i].applyMaterialOptions(material);
		material.setTexture(0, atlas);
		material.getTextureMatrix(0).setTextureTranslate(pos.X, pos.Y);
		material.getTextureMatrix(0).setTextureScale(size.X, size.Y);
	}
	m_meshnode->setVisible(true);
	m_is_cube = true;
	updateLight(m_light);
}

void ExtrudedSpriteSceneNode::setNodeBox(content_t c)
{
	const v3f cube_scale(1.0, 1.0, 1.0);
	if (m_cubemesh)
		m_cubemesh->drop();

	std::vector<NodeBox> boxes = content_features(c).getWieldNodeBoxes();
	m_cubemesh = createNodeBoxMesh(boxes,cube_scale);

	for (u16 i=0; i < boxes.size(); i++) {
		for (int t=0; t<6; t++) {
			video::ITexture* atlas = content_features(c).tiles[t].texture.atlas;
			v2f pos = content_features(c).tiles[t].texture.pos;
			v2f size = content_features(c).tiles[t].texture.size;
			video::SMaterial& material = m_cubemesh->getMeshBuffer((i*6)+t)->getMaterial();
			material.setFlag(video::EMF_LIGHTING, false);
			material.setFlag(video::EMF_BILINEAR_FILTER, false);
			content_features(c).tiles[i].applyMaterialOptions(material);
			material.setTexture(0, atlas);
			material.getTextureMatrix(0).setTextureTranslate(pos.X, pos.Y);
			material.getTextureMatrix(0).setTextureScale(size.X, size.Y);
		}
	}

	m_meshnode->setMesh(m_cubemesh);
	m_meshnode->setScale(v3f(1));

	m_meshnode->setVisible(true);
	m_is_cube = true;
	updateLight(m_light);
}

void ExtrudedSpriteSceneNode::setArm(video::ITexture *texture)
{
	const v3f cube_scale(0.3, 1.0, 0.3);
	if (m_cubemesh)
		m_cubemesh->drop();

	m_cubemesh = createCubeMesh(cube_scale);

	m_meshnode->setMesh(m_cubemesh);
	m_meshnode->setScale(v3f(1));

	// Get the tile texture and atlas transformation
	if (texture == NULL)
		texture = g_texturesource->getTextureRaw("character.png");
	v2f pos(0.75,0.5);
	v2f size(0.0625,0.125);

	// Set material flags and texture
	video::SMaterial& material = m_meshnode->getMaterial(0);
	material.setFlag(video::EMF_LIGHTING, false);
	material.setFlag(video::EMF_BILINEAR_FILTER, false);
	material.MaterialType = video::EMT_SOLID;
	material.BackfaceCulling = true;
	material.setTexture(0, texture);
	material.getTextureMatrix(0).setTextureTranslate(pos.X, pos.Y);
	material.getTextureMatrix(0).setTextureScale(size.X, size.Y);

	for (int i = 1; i < 6; ++i) {
		// Get the tile texture and atlas transformation
		v2f pos(0.75,1.);
		v2f size(0.0625,-0.375);

		// Set material flags and texture
		video::SMaterial& material = m_meshnode->getMaterial(i);
		material.setFlag(video::EMF_LIGHTING, false);
		material.setFlag(video::EMF_BILINEAR_FILTER, false);
		material.MaterialType = video::EMT_SOLID;
		material.BackfaceCulling = true;
		material.setTexture(0, texture);
		material.getTextureMatrix(0).setTextureTranslate(pos.X, pos.Y);
		material.getTextureMatrix(0).setTextureScale(size.X, size.Y);
	}
	m_meshnode->setVisible(true);
	m_is_cube = true;
	updateLight(m_light);
}

void ExtrudedSpriteSceneNode::updateLight(u8 light)
{
	m_light = light;
	video::SColor color(255,light,light,light);
	setMeshVerticesColor(m_meshnode->getMesh(), color);
}

void ExtrudedSpriteSceneNode::removeSpriteFromCache(video::ITexture* texture)
{
	scene::IMeshCache* cache = SceneManager->getMeshCache();
	scene::IAnimatedMesh* mesh = cache->getMeshByName(getExtrudedName(texture));
	if (mesh != NULL)
		cache->removeMesh(mesh);
}

void ExtrudedSpriteSceneNode::setSpriteThickness(f32 thickness)
{
	m_thickness = thickness;
	if (!m_is_cube)
		m_meshnode->setScale(v3f(1, 1, thickness));
}

const core::aabbox3d<f32>& ExtrudedSpriteSceneNode::getBoundingBox() const
{
	return m_meshnode->getBoundingBox();
}

void ExtrudedSpriteSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);
	ISceneNode::OnRegisterSceneNode();
}

void ExtrudedSpriteSceneNode::render()
{
	// do nothing
}

io::path ExtrudedSpriteSceneNode::getExtrudedName(video::ITexture* texture)
{
	io::path path = texture->getName();
	path.append("/[extruded]");
	return path;
}
