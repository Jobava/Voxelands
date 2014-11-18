/************************************************************************
* Minetest-c55
* Copyright (C) 2010-2011 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* common_irrlicht.h
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

#ifndef COMMON_IRRLICHT_HEADER
#define COMMON_IRRLICHT_HEADER

#define endSceneX(d){d->draw2DLine(v2s32(0,0),v2s32(1,0),\
video::SColor(255,30,30,30));d->endScene();}

#include <irrTypes.h>
#include <vector2d.h>
#include <vector3d.h>
#include <irrMap.h>
#include <irrList.h>
#include <irrArray.h>
#include <aabbox3d.h>
#ifndef SERVER
#include <SColor.h>
#include <IMesh.h>
#include <IImage.h>
#include <IrrlichtDevice.h>
#include <IMeshSceneNode.h>
// this is to stop unqualified-id compiler problems
#ifdef PI
# undef PI
#endif
#include <ITextSceneNode.h>
#include <IAnimatedMesh.h>
#include <IAnimatedMeshSceneNode.h>
#include <SMesh.h>
#include <ISceneManager.h>
#include <IMeshBuffer.h>
#include <SMeshBuffer.h>
#include <IGUIElement.h>
#include <IGUIEnvironment.h>
#endif
using namespace irr;
typedef core::vector3df v3f;
typedef core::vector3d<s16> v3s16;
typedef core::vector3d<s32> v3s32;

typedef core::vector2d<f32> v2f;
typedef core::vector2d<s16> v2s16;
typedef core::vector2d<s32> v2s32;
typedef core::vector2d<u32> v2u32;
typedef core::vector2d<f32> v2f32;

typedef core::aabbox3d<f32> aabb3f;

#include <stdint.h>

#endif

