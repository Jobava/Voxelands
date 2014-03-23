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

// For g_settings
#include "main.h"

#include "content_mapnode.h"
#include "mapnode.h"
#include "content_nodemeta.h"
#include "settings.h"
#include "content_craftitem.h"
#include "content_toolitem.h"
#include "content_craft.h"
#include "content_list.h"
#ifndef SERVER
#include "tile.h"
#endif

#define WATER_ALPHA 160

#define WATER_VISC 1
#define LAVA_VISC 7

static void content_mapnode_nodebox_stair(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.,
		0.,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_stairud(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		0.,
		0.5*BS,
		0.,
		0.5*BS
	));
}

static void content_mapnode_nodebox_slab(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0,
		0.5*BS
	));
}

static void content_mapnode_nodebox_slabud(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_door(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3125*BS
	));
}

static void content_mapnode_nodebox_doorw(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.0625*BS,
		-0.3125*BS,
		-0.4375*BS,
		0.0625*BS,
		0.3125*BS,
		-0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,
		-0.0625*BS,
		-0.4375*BS,
		-0.0625*BS,
		0.0625*BS,
		-0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.0625*BS,
		-0.0625*BS,
		-0.4375*BS,
		0.3125*BS,
		0.0625*BS,
		-0.375*BS
	));
}

static void content_mapnode_nodebox_doorol(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.3125*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_doorwol(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.3125*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		-0.3125*BS,
		0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.3125*BS,
		-0.0625*BS,
		-0.375*BS,
		0.3125*BS,
		0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.0625*BS,
		-0.3125*BS,
		-0.375*BS,
		0.0625*BS,
		-0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.0625*BS,
		0.0625*BS,
		-0.375*BS,
		0.0625*BS,
		0.3125*BS
	));
}

static void content_mapnode_nodebox_dooror(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_doorwor(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.3125*BS,
		0.3125*BS,
		0.5*BS,
		0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.3125*BS,
		-0.0625*BS,
		0.4375*BS,
		0.3125*BS,
		0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.0625*BS,
		-0.3125*BS,
		0.4375*BS,
		0.0625*BS,
		-0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.0625*BS,
		0.0625*BS,
		0.4375*BS,
		0.0625*BS,
		0.3125*BS
	));
}

static void content_mapnode_nodebox_hatch(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_hatchw(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		0.3125*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		0.3125*BS,
		-0.3125*BS,
		0.5*BS,
		0.5*BS,
		0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		-0.3125*BS,
		-0.3125*BS,
		0.5*BS,
		0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.0625*BS,
		0.375*BS,
		-0.3125*BS,
		0.0625*BS,
		0.4375*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,
		0.375*BS,
		-0.0625*BS,
		-0.0625*BS,
		0.4375*BS,
		0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.0625*BS,
		0.375*BS,
		-0.0625*BS,
		0.3125*BS,
		0.4375*BS,
		0.0625*BS
	));
}

static void content_mapnode_nodebox_hatcho(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_hatchwo(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,
		-0.3125*BS,
		0.3125*BS,
		0.5*BS,
		0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.3125*BS,
		-0.0625*BS,
		0.4375*BS,
		0.3125*BS,
		0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.0625*BS,
		-0.3125*BS,
		0.4375*BS,
		0.0625*BS,
		-0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.0625*BS,
		0.0625*BS,
		0.4375*BS,
		0.0625*BS,
		0.3125*BS
	));
}

static void content_mapnode_nodebox_gate(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		-0.4*BS,
		-0.05*BS,
		0.4*BS,
		-0.2*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		0.1*BS,
		-0.05*BS,
		0.4*BS,
		0.3*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		-0.4*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.1*BS,
		-0.2*BS,
		-0.05*BS,
		0.1*BS,
		0.4*BS,
		0.05*BS
	));
}

static void content_mapnode_nodebox_gateo(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		-0.4*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.4*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.4*BS,
		0.4*BS,
		-0.4*BS,
		0.4*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.4*BS,
		0.4*BS,
		0.5*BS,
		0.4*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.1*BS,
		-0.05*BS,
		-0.4*BS,
		0.3*BS,
		0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.4*BS,
		-0.05*BS,
		-0.4*BS,
		-0.2*BS,
		0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		0.1*BS,
		-0.05*BS,
		0.5*BS,
		0.3*BS,
		0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4*BS,
		-0.4*BS,
		-0.05*BS,
		0.5*BS,
		-0.2*BS,
		0.4*BS
	));
}

static void content_mapnode_nodebox_carpet(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.4375*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_bed_head(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.125*BS,
		-0.5*BS,
		0.4375*BS,
		0.125*BS,
		0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		0.125*BS,
		0.*BS,
		0.375*BS,
		0.1875*BS,
		0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		0.375*BS,
		-0.375*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.5*BS,
		0.375*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		-0.125*BS,
		0.4375*BS,
		0.375*BS,
		0.*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		0.1925*BS,
		0.4375*BS,
		0.375*BS,
		0.375*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4375*BS,
		-0.125*BS,
		-0.5*BS,
		0.5*BS,
		0.*BS,
		0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.125*BS,
		-0.5*BS,
		-0.4375*BS,
		0.*BS,
		0.375*BS
	));
}

static void content_mapnode_nodebox_bed_foot(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.125*BS,
		-0.4375*BS,
		0.4375*BS,
		0.125*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.375*BS,
		0.5*BS,
		-0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		-0.125*BS,
		-0.5*BS,
		0.375*BS,
		0.*BS,
		-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		0.1925*BS,
		-0.5*BS,
		0.375*BS,
		0.25*BS,
		-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4375*BS,
		-0.125*BS,
		-0.375*BS,
		0.5*BS,
		0.*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.125*BS,
		-0.375*BS,
		-0.4375*BS,
		0.*BS,
		0.5*BS
	));
}

static void content_mapnode_nodebox_plant_1(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.25*BS,
		-0.5*BS,
		-0.25*BS,
		0.25*BS,
		0.*BS,
		0.25*BS
	));
}

static void content_mapnode_nodebox_plant_2(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		-0.5*BS,
		-0.375*BS,
		0.375*BS,
		0.25*BS,
		0.375*BS
	));
}

static void content_mapnode_nodebox_plant_3(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.5*BS,
		-0.4375*BS,
		0.4375*BS,
		0.375*BS,
		0.4375*BS
	));
}

static void content_mapnode_nodebox_guide(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.5*BS,
		-0.375*BS,

		0.4375*BS,
		-0.4375*BS,
		0.1875*BS
	));

	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,
		-0.4375*BS,
		-0.375*BS,

		0.375*BS,
		-0.375*BS,
		0.1875*BS
	));
}

// Painting nodebox - easy access!
static void content_mapnode_nodebox_painting(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.437500*BS,
		-0.375000*BS,
		0.437500*BS,
		0.437500*BS,
		0.375000*BS,
		0.500000*BS
	));
}

static void content_mapnode_nodebox_book(ContentFeatures *f)
{
	// lower cover
	f->setNodeBox(core::aabbox3d<f32>(
		0,
		-0.5*BS,
		// -6 / 16
		-0.375*BS,

		// 7 / 16
		0.4375*BS,
		// -7 / 16
		-0.4375*BS,
		// 3 / 16
		0.1875*BS
	));

	// pages
	f->addNodeBox(core::aabbox3d<f32>(
		0,
		// -7 / 16
		-0.4375*BS,
		// -6 / 16
		-0.375*BS,

		// 6 / 16
		0.375*BS,
		// -5 / 16
		-0.3125*BS,
		// 3 / 16
		0.1875*BS
	));

	// top cover
	f->addNodeBox(core::aabbox3d<f32>(
		0,
		// -5 / 16
		-0.3125*BS,
		// -6 / 16
		-0.375*BS,

		// 7 / 16
		0.4375*BS,
		// -4 / 16
		-0.25*BS,
		// 3 / 16
		0.1875*BS
	));
}

static void content_mapnode_nodebox_pie(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	(4.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(core::aabbox3d<f32>(
	(-5.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,

	(5.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,
	(5.0 / 16.0)*BS
	));
}

static void content_mapnode_nodebox_pie_3(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	0,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(core::aabbox3d<f32>(
	(-5.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,

	0,
	(-5.0 / 16.0)*BS,
	(5.0 / 16.0)*BS
	));

	f->addNodeBox(core::aabbox3d<f32>(
	0,
	-0.5*BS,
	0,

	(4.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(core::aabbox3d<f32>(
	0,
	(-6.0 / 16.0)*BS,
	0,

	(5.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,
	(5.0 / 16.0)*BS
	));
}

static void content_mapnode_nodebox_pie_2(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	0,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(core::aabbox3d<f32>(
	(-5.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,

	0,
	(-5.0 / 16.0)*BS,
	(5.0 / 16.0)*BS
	));
}

static void content_mapnode_nodebox_pie_1(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	0,
	(-6.0 / 16.0)*BS,
	0
	));

	f->addNodeBox(core::aabbox3d<f32>(
	(-5.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,

	0,
	(-5.0 / 16.0)*BS,
	0
	));
}

/*
	A conversion table for backwards compatibility.
	Maps <=v19 content types to current ones.
	Should never be touched.
*/
content_t trans_table_19[21][2] = {
	{CONTENT_GRASS, 1},
	{CONTENT_TREE, 4},
	{CONTENT_LEAVES, 5},
	{CONTENT_FARM_DIRT, 6},
	{CONTENT_MESE, 7},
	{CONTENT_MUD, 8},
	{CONTENT_COTTON, 10},
	{CONTENT_BORDERSTONE, 11},
	{CONTENT_WOOD, 12},
	{CONTENT_SAND, 13},
	{CONTENT_COBBLE, 18},
	{CONTENT_STEEL, 19},
	{CONTENT_GLASS, 20},
	{CONTENT_MOSSYCOBBLE, 22},
	{CONTENT_GRAVEL, 23},
	{CONTENT_SANDSTONE, 24},
	{CONTENT_CACTUS, 25},
	{CONTENT_BRICK, 26},
	{CONTENT_CLAY, 27},
	{CONTENT_PAPYRUS, 28},
	{CONTENT_BOOKSHELF, 29},
};

MapNode mapnode_translate_from_internal(MapNode n_from, u8 version)
{
	MapNode result = n_from;
	if(version <= 19)
	{
		content_t c_from = n_from.getContent();
		for(u32 i=0; i<sizeof(trans_table_19)/sizeof(trans_table_19[0]); i++)
		{
			if(trans_table_19[i][0] == c_from)
			{
				result.setContent(trans_table_19[i][1]);
				break;
			}
		}
	}
	return result;
}
MapNode mapnode_translate_to_internal(MapNode n_from, u8 version)
{
	MapNode result = n_from;
	if(version <= 19)
	{
		content_t c_from = n_from.getContent();
		for(u32 i=0; i<sizeof(trans_table_19)/sizeof(trans_table_19[0]); i++)
		{
			if(trans_table_19[i][1] == c_from)
			{
				result.setContent(trans_table_19[i][0]);
				break;
			}
		}
	}
	// stairs, slabs, gates, doors, and hatches were moved for 1311.00
	if (n_from.getContent() >= 0x826 && n_from.getContent() <= 0x87F) {
		// slabs
		if (n_from.getContent() == 0x826) {
			result.setContent(CONTENT_BRICK_SLAB);
		}else if (n_from.getContent() == 0x827) {
			result.setContent(CONTENT_SANDSTONE_SLAB);
		}
		// stairs
		if (n_from.getContent() == 0x828) {
			result.setContent(CONTENT_ROUGHSTONE_STAIR);
		}else if (n_from.getContent() == 0x829) {
			result.setContent(CONTENT_MOSSYCOBBLE_STAIR);
		}else if (n_from.getContent() == 0x82a) {
			result.setContent(CONTENT_STONE_STAIR);
		}else if (n_from.getContent() == 0x82b) {
			result.setContent(CONTENT_WOOD_STAIR);
		}else if (n_from.getContent() == 0x82c) {
			result.setContent(CONTENT_JUNGLE_STAIR);
		}else if (n_from.getContent() == 0x82d) {
			result.setContent(CONTENT_BRICK_STAIR);
		}else if (n_from.getContent() == 0x82e) {
			result.setContent(CONTENT_SANDSTONE_STAIR);
		}
		// upside down slabs
		if (n_from.getContent() == 0x831) {
			result.setContent(CONTENT_ROUGHSTONE_SLAB_UD);
		}else if (n_from.getContent() == 0x832) {
			result.setContent(CONTENT_MOSSYCOBBLE_SLAB_UD);
		}else if (n_from.getContent() == 0x833) {
			result.setContent(CONTENT_STONE_SLAB_UD);
		}else if (n_from.getContent() == 0x834) {
			result.setContent(CONTENT_WOOD_SLAB_UD);
		}else if (n_from.getContent() == 0x835) {
			result.setContent(CONTENT_JUNGLE_SLAB_UD);
		}else if (n_from.getContent() == 0x836) {
			result.setContent(CONTENT_BRICK_SLAB_UD);
		}else if (n_from.getContent() == 0x837) {
			result.setContent(CONTENT_SANDSTONE_SLAB_UD);
		}
		// upside down stairs
		if (n_from.getContent() == 0x838) {
			result.setContent(CONTENT_ROUGHSTONE_STAIR_UD);
		}else if (n_from.getContent() == 0x839) {
			result.setContent(CONTENT_MOSSYCOBBLE_STAIR_UD);
		}else if (n_from.getContent() == 0x83a) {
			result.setContent(CONTENT_STONE_STAIR_UD);
		}else if (n_from.getContent() == 0x83b) {
			result.setContent(CONTENT_WOOD_STAIR_UD);
		}else if (n_from.getContent() == 0x83c) {
			result.setContent(CONTENT_JUNGLE_STAIR_UD);
		}else if (n_from.getContent() == 0x83d) {
			result.setContent(CONTENT_BRICK_STAIR_UD);
		}else if (n_from.getContent() == 0x83e) {
			result.setContent(CONTENT_SANDSTONE_STAIR_UD);
		}


		// doors
		if (n_from.getContent() == 0x840) {
			result.setContent(CONTENT_WOOD_DOOR_LB);
		}else if (n_from.getContent() == 0x841) {
			result.setContent(CONTENT_WOOD_DOOR_LT);
		}else if (n_from.getContent() == 0x842) {
			result.setContent(CONTENT_STEEL_DOOR_LB);
		}else if (n_from.getContent() == 0x843) {
			result.setContent(CONTENT_STEEL_DOOR_LT);
		}else if (n_from.getContent() == 0x844) {
			result.setContent(CONTENT_WOOD_W_DOOR_LB);
		}else if (n_from.getContent() == 0x845) {
			result.setContent(CONTENT_WOOD_W_DOOR_LT);
		}else if (n_from.getContent() == 0x846) {
			result.setContent(CONTENT_STEEL_W_DOOR_LB);
		}else if (n_from.getContent() == 0x847) {
			result.setContent(CONTENT_STEEL_W_DOOR_LT);
		}else if (n_from.getContent() == 0x848) {
			result.setContent(CONTENT_WOOD_DOOR_RB);
		}else if (n_from.getContent() == 0x849) {
			result.setContent(CONTENT_WOOD_DOOR_RT);
		}else if (n_from.getContent() == 0x84A) {
			result.setContent(CONTENT_STEEL_DOOR_RB);
		}else if (n_from.getContent() == 0x84B) {
			result.setContent(CONTENT_STEEL_DOOR_RT);
		}else if (n_from.getContent() == 0x84C) {
			result.setContent(CONTENT_WOOD_W_DOOR_RB);
		}else if (n_from.getContent() == 0x84D) {
			result.setContent(CONTENT_WOOD_W_DOOR_RT);
		}else if (n_from.getContent() == 0x84E) {
			result.setContent(CONTENT_STEEL_W_DOOR_RB);
		}else if (n_from.getContent() == 0x84F) {
			result.setContent(CONTENT_STEEL_W_DOOR_RT);
		}
		// hatches
		if (n_from.getContent() == 0x850) {
			result.setContent(CONTENT_WOOD_HATCH);
		}else if (n_from.getContent() == 0x852) {
			result.setContent(CONTENT_STEEL_HATCH);
		}else if (n_from.getContent() == 0x854) {
			result.setContent(CONTENT_WOOD_W_HATCH);
		}else if (n_from.getContent() == 0x856) {
			result.setContent(CONTENT_STEEL_W_HATCH);
		}
		// gates
		if (n_from.getContent() == 0x858) {
			result.setContent(CONTENT_WOOD_GATE);
		}else if (n_from.getContent() == 0x85A) {
			result.setContent(CONTENT_STEEL_GATE);
		}

		// open doors
		if (n_from.getContent() == 0x860) {
			result.setContent(CONTENT_WOOD_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x861) {
			result.setContent(CONTENT_WOOD_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x862) {
			result.setContent(CONTENT_STEEL_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x863) {
			result.setContent(CONTENT_STEEL_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x864) {
			result.setContent(CONTENT_WOOD_W_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x865) {
			result.setContent(CONTENT_WOOD_W_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x866) {
			result.setContent(CONTENT_STEEL_W_DOOR_LB_OPEN);
		}else if (n_from.getContent() == 0x867) {
			result.setContent(CONTENT_STEEL_W_DOOR_LT_OPEN);
		}else if (n_from.getContent() == 0x868) {
			result.setContent(CONTENT_WOOD_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x869) {
			result.setContent(CONTENT_WOOD_DOOR_RT_OPEN);
		}else if (n_from.getContent() == 0x86A) {
			result.setContent(CONTENT_STEEL_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x86B) {
			result.setContent(CONTENT_STEEL_DOOR_RT_OPEN);
		}else if (n_from.getContent() == 0x86C) {
			result.setContent(CONTENT_WOOD_W_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x86D) {
			result.setContent(CONTENT_WOOD_W_DOOR_RT_OPEN);
		}else if (n_from.getContent() == 0x86E) {
			result.setContent(CONTENT_STEEL_W_DOOR_RB_OPEN);
		}else if (n_from.getContent() == 0x86F) {
			result.setContent(CONTENT_STEEL_W_DOOR_RT_OPEN);
		}
		// open hatches
		if (n_from.getContent() == 0x870) {
			result.setContent(CONTENT_WOOD_HATCH_OPEN);
		}else if (n_from.getContent() == 0x872) {
			result.setContent(CONTENT_STEEL_HATCH_OPEN);
		}else if (n_from.getContent() == 0x874) {
			result.setContent(CONTENT_WOOD_W_HATCH_OPEN);
		}else if (n_from.getContent() == 0x876) {
			result.setContent(CONTENT_STEEL_W_HATCH_OPEN);
		}
		// open gates
		if (n_from.getContent() == 0x878) {
			result.setContent(CONTENT_WOOD_GATE_OPEN);
		}else if (n_from.getContent() == 0x87A) {
			result.setContent(CONTENT_STEEL_GATE_OPEN);
		}
	}
	return result;
}

void content_mapnode_init()
{
	// Read some settings
	bool new_style_water = g_settings->getBool("new_style_water");
	bool new_style_leaves = g_settings->getBool("new_style_leaves");
	bool invisible_stone = g_settings->getBool("invisible_stone");
#ifndef SERVER
	bool opaque_water = g_settings->getBool("opaque_water");
#endif

	content_t i;
	ContentFeatures *f = NULL;

	i = CONTENT_STONE;
	f = &content_features(i);
	f->description = std::string("Stone");
	f->setAllTextures("stone.png");
	f->setInventoryTextureCube("stone.png", "stone.png", "stone.png");
	f->param_type = CPT_MINERAL;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->often_contains_mineral = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	if(invisible_stone)
		f->solidness = 0; // For debugging, hides regular stone
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_ICE;
	f = &content_features(i);
	f->description = std::string("Ice");
	f->setAllTextures("ice.png");
	f->setInventoryTextureCube("ice.png", "ice.png", "ice.png");
	f->draw_type = CDT_CUBELIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_COAL;
	f = &content_features(i);
	f->description = std::string("Coal Block");
	f->setAllTextures("coal.png");
	f->setInventoryTextureCube("coal.png", "coal.png", "coal.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->fuel_time = 100;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_COAL,CONTENT_COAL);
	crafting::setUncraftHardBlockRecipe(CONTENT_COAL,CONTENT_CRAFTITEM_COAL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CHARCOAL;
	f = &content_features(i);
	f->description = std::string("Charcoal Block");
	f->setAllTextures("charcoal.png");
	f->setInventoryTextureCube("charcoal.png", "charcoal.png", "charcoal.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->fuel_time = 100;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_CHARCOAL,CONTENT_CHARCOAL);
	crafting::setUncraftHardBlockRecipe(CONTENT_CHARCOAL,CONTENT_CRAFTITEM_CHARCOAL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONEBRICK;
	f = &content_features(i);
	f->description = std::string("Stone Brick");
	f->setAllTextures("stonebrick.png");
	f->setInventoryTextureCube("stonebrick.png", "stonebrick.png", "stonebrick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_STONE,CONTENT_STONEBRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONEBLOCK;
	f = &content_features(i);
	f->description = std::string("Stone Block");
	f->setAllTextures("stoneblock.png");
	f->setInventoryTextureCube("stoneblock.png", "stoneblock.png", "stoneblock.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_STONE,CONTENT_STONEBLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONEBRICK;
	f = &content_features(i);
	f->description = std::string("Rough Stone Brick");
	f->setAllTextures("roughstone_brick.png");
	f->setInventoryTextureCube("roughstone_brick.png", "roughstone_brick.png", "roughstone_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONEBRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONEBLOCK;
	f = &content_features(i);
	f->description = std::string("Rough Stone Block");
	f->setAllTextures("roughstone_block.png");
	f->setInventoryTextureCube("roughstone_block.png", "roughstone_block.png", "roughstone_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONEBLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GRASS;
	f = &content_features(i);
	f->description = std::string("Grass");
	f->setAllTextures("mud.png^grass_side.png");
	f->setTexture(0, "grass.png");
	f->setTexture(1, "mud.png");
	f->setInventoryTextureCube("grass.png","mud.png^grass_side.png","mud.png^grass_side.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WILDGRASS_SHORT)+" 1";
	f->extra_dug_item_rarity = 10;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("decrafting",i);

	i = CONTENT_GRASS_FOOTSTEPS;
	f = &content_features(i);
	f->description = std::string("Grass");
	f->setAllTextures("mud.png^grass_side.png");
	f->setTexture(0, "grass_footsteps.png");
	f->setTexture(1, "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;

	i = CONTENT_MUDSNOW;
	f = &content_features(i);
	f->description = std::string("Muddy Snow");
	f->setAllTextures("mud.png^snow_side.png");
	f->setTexture(0, "snow.png");
	f->setTexture(1, "mud.png");
	f->setInventoryTextureCube("snow.png","mud.png^snow_side.png","mud.png^snow_side.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->extra_dug_item = std::string("CraftItem snow_ball 1");
	f->extra_dug_item_rarity = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("decrafting",i);

	i = CONTENT_FARM_DIRT;
	f = &content_features(i);
	f->description = std::string("Farm Dirt");
	f->setAllTextures("dirt.png");
	f->setInventoryTextureCube("dirt.png","dirt.png","dirt.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MUD)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("decrafting",i);

	i = CONTENT_MUD;
	f = &content_features(i);
	f->description = std::string("Mud");
	f->setAllTextures("mud.png");
	f->setInventoryTextureCube("mud.png", "mud.png", "mud.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_ASH;
	f = &content_features(i);
	f->description = std::string("Ash Block");
	f->setAllTextures("ash.png");
	f->setInventoryTextureCube("ash.png", "ash.png", "ash.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_ash 9");
	f->type = CMT_DIRT;
	f->hardness = 0.5;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_ASH,CONTENT_ASH);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SAND;
	f = &content_features(i);
	f->description = std::string("Sand");
	f->setAllTextures("sand.png");
	f->setInventoryTextureCube("sand.png", "sand.png", "sand.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_GLASS)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_GRAVEL;
	f = &content_features(i);
	f->description = std::string("Gravel");
	f->setAllTextures("gravel.png");
	f->setInventoryTextureCube("gravel.png", "gravel.png", "gravel.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->extra_dug_item = std::string("CraftItem lump_of_flint 1");
	f->extra_dug_item_rarity = 10;
	f->type = CMT_DIRT;
	f->hardness = 1.75;
	lists::add("creative",i);

	i = CONTENT_SANDSTONE;
	f = &content_features(i);
	f->description = std::string("Sand Stone");
	f->setAllTextures("sandstone.png");
	f->setInventoryTextureCube("sandstone.png", "sandstone.png", "sandstone.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_SAND,CONTENT_SANDSTONE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SANDSTONE_BRICK;
	f = &content_features(i);
	f->description = std::string("Sand Stone Bricks");
	f->setAllTextures("sandstone_brick.png");
	f->setInventoryTextureCube("sandstone_brick.png", "sandstone_brick.png", "sandstone_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_BRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SANDSTONE_BLOCK;
	f = &content_features(i);
	f->description = std::string("Sand Stone Blocks");
	f->setAllTextures("sandstone_block.png");
	f->setInventoryTextureCube("sandstone_block.png", "sandstone_block.png", "sandstone_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_BLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY;
	f = &content_features(i);
	f->description = std::string("Clay");
	f->setAllTextures("clay.png");
	f->setInventoryTextureCube("clay.png", "clay.png", "clay.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_TERRACOTTA)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_CLAY,CONTENT_CLAY);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Clay");
	f->setAllTextures("clay_blue.png");
	f->setInventoryTextureCube("clay_blue.png", "clay_blue.png", "clay_blue.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_blue 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CLAY,CONTENT_CLAY_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Clay");
	f->setAllTextures("clay_green.png");
	f->setInventoryTextureCube("clay_green.png", "clay_green.png", "clay_green.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_green 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CLAY,CONTENT_CLAY_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Clay");
	f->setAllTextures("clay_orange.png");
	f->setInventoryTextureCube("clay_orange.png", "clay_orange.png", "clay_orange.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_orange 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CLAY,CONTENT_CLAY_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Clay");
	f->setAllTextures("clay_purple.png");
	f->setInventoryTextureCube("clay_purple.png", "clay_purple.png", "clay_purple.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_purple 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CLAY,CONTENT_CLAY_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_RED;
	f = &content_features(i);
	f->description = std::string("Red Clay");
	f->setAllTextures("clay_red.png");
	f->setInventoryTextureCube("clay_red.png", "clay_red.png", "clay_red.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_red 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_CLAY,CONTENT_CLAY_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Clay");
	f->setAllTextures("clay_yellow.png");
	f->setInventoryTextureCube("clay_yellow.png", "clay_yellow.png", "clay_yellow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_yellow 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CLAY,CONTENT_CLAY_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CLAY_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Clay");
	f->setAllTextures("clay_black.png");
	f->setInventoryTextureCube("clay_black.png", "clay_black.png", "clay_black.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem lump_of_clay 4");
	f->extra_dug_item = std::string("CraftItem dye_black 1");
	f->extra_dug_item_rarity = 4;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CLAY,CONTENT_CLAY_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_BRICK;
	f = &content_features(i);
	f->description = std::string("Brick");
	f->setAllTextures("brick.png");
	f->setInventoryTextureCube("brick.png", "brick.png", "brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_CLAY_BRICK,CONTENT_BRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_TERRACOTTA;
	f = &content_features(i);
	f->description = std::string("Terracotta");
	f->setAllTextures("terracotta.png");
	f->setInventoryTextureCube("terracotta.png", "terracotta.png", "terracotta.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_TERRACOTTA_BRICK;
	f = &content_features(i);
	f->description = std::string("Terracotta Brick");
	f->setAllTextures("terracotta_brick.png");
	f->setInventoryTextureCube("terracotta_brick.png", "terracotta_brick.png", "terracotta_brick.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBrickRecipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA_BRICK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TERRACOTTA_BLOCK;
	f = &content_features(i);
	f->description = std::string("Terracotta Block");
	f->setAllTextures("terracotta_block.png");
	f->setInventoryTextureCube("terracotta_block.png", "terracotta_block.png", "terracotta_block.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setBlockRecipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA_BLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TERRACOTTA_TILE;
	f = &content_features(i);
	f->description = std::string("Terracotta Tile");
	f->setAllTextures("terracotta_tile.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 1.0;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"terracotta_tile.png", "terracotta_tile.png", "terracotta_tile.png");
	crafting::setTileRecipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA_TILE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TREE;
	f = &content_features(i);
	f->description = std::string("Tree");
	f->setAllTextures("tree.png");
	f->setTexture(0, "tree_top.png");
	f->setTexture(1, "tree_top.png");
	f->setInventoryTextureCube("tree_top.png", "tree.png", "tree.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("CraftItem lump_of_charcoal 1");
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_APPLE_TREE;
	f = &content_features(i);
	f->description = std::string("Apple Tree");
	f->setAllTextures("apple_tree.png");
	f->setTexture(0, "apple_tree_top.png");
	f->setTexture(1, "apple_tree_top.png");
	f->setInventoryTextureCube("apple_tree_top.png", "apple_tree.png", "apple_tree.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("CraftItem lump_of_charcoal 1");
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_JUNGLETREE;
	f = &content_features(i);
	f->description = std::string("Jungle Tree");
	f->setAllTextures("jungletree.png");
	f->setTexture(0, "jungletree_top.png");
	f->setTexture(1, "jungletree_top.png");
	f->setInventoryTextureCube("jungletree_top.png", "jungletree.png", "jungletree.png");
	f->draw_type = CDT_CUBELIKE;
	//f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_CONIFER_TREE;
	f = &content_features(i);
	f->description = std::string("Conifer Tree");
	f->setAllTextures("conifer_tree.png");
	f->setTexture(0, "conifer_tree_top.png");
	f->setTexture(1, "conifer_tree_top.png");
	f->is_ground_content = true;
	f->flammable = 2; // can be set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("CraftItem lump_of_charcoal 1");
	f->fuel_time = 30;
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 0;
	f->fuel_time = 25;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.5*BS,
		-0.4375*BS,
		0.4375*BS,
		0.5*BS,
		0.4375*BS
	));
	f->setInventoryTextureNodeBox(i,"conifer_tree_top.png", "conifer_tree.png", "conifer_tree.png");
	f->type = CMT_WOOD;
	f->hardness = 0.8;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_JUNGLEGRASS;
	f = &content_features(i);
	f->description = std::string("Jungle Grass");
	f->setInventoryTexture("junglegrass.png");
	f->setAllTextures("junglegrass.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE_LGE;
	//f->is_ground_content = true;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);

	i = CONTENT_LEAVES;
	f = &content_features(i);
	f->description = std::string("Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("leaves.png", "leaves.png", "leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("decrafting",i);

	i = CONTENT_APPLE_LEAVES;
	f = &content_features(i);
	f->description = std::string("Apple Tree Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("apple_leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("apple_leaves.png", "apple_leaves.png", "apple_leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:apple_leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_APPLE_SAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("decrafting",i);

	i = CONTENT_JUNGLELEAVES;
	f = &content_features(i);
	f->description = std::string("Jungle Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("jungleleaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("jungleleaves.png", "jungleleaves.png", "jungleleaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:jungleleaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLESAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("decrafting",i);

	i = CONTENT_CONIFER_LEAVES;
	f = &content_features(i);
	f->description = std::string("Conifer Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("conifer_leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("conifer_leaves.png", "conifer_leaves.png", "conifer_leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:conifer_leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->extra_dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CONIFER_SAPLING)+" 1";
	f->extra_dug_item_rarity = 20;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("decrafting",i);

	i = CONTENT_TRIMMED_LEAVES;
	f = &content_features(i);
	f->description = std::string("Trimmed Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("leaves.png", "leaves.png", "leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_TRIMMED_APPLE_LEAVES;
	f = &content_features(i);
	f->description = std::string("Trimmed Apple Tree Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("apple_leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("apple_leaves.png", "apple_leaves.png", "apple_leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:apple_leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_TRIMMED_JUNGLE_LEAVES;
	f = &content_features(i);
	f->description = std::string("Trimmed Jungle Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("jungleleaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("jungleleaves.png", "jungleleaves.png", "jungleleaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:jungleleaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_TRIMMED_CONIFER_LEAVES;
	f = &content_features(i);
	f->description = std::string("Trimmed Conifer Leaves");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("conifer_leaves.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("conifer_leaves.png", "conifer_leaves.png", "conifer_leaves.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:conifer_leaves.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_APPLE_BLOSSOM;
	f = &content_features(i);
	f->description = std::string("Apple Tree Blossom");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	//f->is_ground_content = true;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("apple_leaves.png^apple_blossom.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("apple_leaves.png^apple_blossom.png", "apple_leaves.png^apple_blossom.png", "apple_leaves.png^apple_blossom.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:apple_leaves.png^apple_blossom.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->extra_dug_item = std::string("CraftItem apple_blossom 1");
	f->extra_dug_item_rarity = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_APPLE_LEAVES)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("decrafting",i);

	i = CONTENT_TRIMMED_APPLE_BLOSSOM;
	f = &content_features(i);
	f->description = std::string("Trimmed Apple Tree Blossom");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	if (new_style_leaves) {
		f->draw_type = CDT_GLASSLIKE;
		f->solidness = 0; // drawn separately, makes no faces
		f->visual_solidness = 1;
		f->setAllTextures("apple_leaves.png^apple_blossom.png");
#ifndef SERVER
		f->setAllTextureTypes(MATERIAL_ALPHA_SIMPLE);
#endif
		f->setInventoryTextureCube("apple_leaves.png^apple_blossom.png", "apple_leaves.png^apple_blossom.png", "apple_leaves.png^apple_blossom.png");
	}else{
		f->draw_type = CDT_CUBELIKE;
		f->setAllTextures("[noalpha:apple_leaves.png^apple_blossom.png");
	}
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_CACTUS_BLOSSOM;
	f = &content_features(i);
	f->description = std::string("Cactus Blossom");
	f->setInventoryTexture("cactus_blossom.png");
	f->setAllTextures("cactus_blossom.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);

	i = CONTENT_CACTUS_FLOWER;
	f = &content_features(i);
	f->description = std::string("Cactus Flower");
	f->setInventoryTexture("cactus_flower.png");
	f->setAllTextures("cactus_flower.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_CACTUS_BLOSSOM)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_CACTUS_FRUIT;
	f = &content_features(i);
	f->description = std::string("Cactus Berry");
	f->setInventoryTexture("cactus_fruit.png");
	f->setAllTextures("cactus_fruit.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("CraftItem cactus_fruit 1");
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);

	i = CONTENT_CACTUS;
	f = &content_features(i);
	f->description = std::string("Cactus");
	f->setAllTextures("cactus_side.png");
	f->setTexture(0, "cactus_top.png");
	f->setTexture(1, "cactus_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->damage_per_second = 2;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.5*BS,
		-0.4375*BS,
		0.4375*BS,
		0.5*BS,
		0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		-0.4375*BS,
		0.375*BS,
		-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4375*BS,
		0.125*BS,
		0.4375*BS,
		0.5*BS,
		0.1875*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.4375*BS,
		-0.1875*BS,
		-0.5*BS,
		0.5*BS,
		-0.125*BS,
		-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.375*BS,
		0.4375*BS,
		-0.4375*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->setInventoryTextureCube("cactus_top.png", "cactus_side.png", "cactus_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	lists::add("creative",i);

	i = CONTENT_PAPYRUS;
	f = &content_features(i);
	f->description = std::string("Papyrus");
	f->setInventoryTexture("papyrus.png");
	f->setAllTextures("papyrus.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	lists::add("creative",i);

	i = CONTENT_BOOKSHELF;
	f = &content_features(i);
	f->description = std::string("Book Shelf");
	f->setAllTextures("bookshelf.png");
	f->setTexture(0, "wood.png");
	f->setTexture(1, "wood.png");
	f->setInventoryTextureCube("wood.png", "bookshelf.png", "bookshelf.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	{
		u16 r[9] = {
			CONTENT_WOOD,	CONTENT_WOOD,	CONTENT_WOOD,
			CONTENT_BOOK, CONTENT_BOOK, CONTENT_BOOK,
			CONTENT_WOOD,	CONTENT_WOOD,	CONTENT_WOOD
		};
		crafting::setRecipe(r,CONTENT_BOOKSHELF,1);
	}
	{
		u16 r[9] = {
			CONTENT_JUNGLEWOOD,	CONTENT_JUNGLEWOOD,	CONTENT_JUNGLEWOOD,
			CONTENT_BOOK, CONTENT_BOOK, CONTENT_BOOK,
			CONTENT_JUNGLEWOOD,	CONTENT_JUNGLEWOOD,	CONTENT_JUNGLEWOOD
		};
		crafting::setRecipe(r,CONTENT_BOOKSHELF,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS;
	f = &content_features(i);
	f->description = std::string("Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass.png", "glass.png", "glass.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	lists::add("creative",i);

	i = CONTENT_GLASS_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_blue.png", "glass_blue.png", "glass_blue.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_GLASS,CONTENT_GLASS_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_green.png", "glass_green.png", "glass_green.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_GLASS,CONTENT_GLASS_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_orange.png", "glass_orange.png", "glass_orange.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_GLASS,CONTENT_GLASS_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_purple.png", "glass_purple.png", "glass_purple.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_GLASS,CONTENT_GLASS_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_RED;
	f = &content_features(i);
	f->description = std::string("Red Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_red.png", "glass_red.png", "glass_red.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_GLASS,CONTENT_GLASS_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_yellow.png", "glass_yellow.png", "glass_yellow.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_GLASS,CONTENT_GLASS_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Glass");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glass_black.png", "glass_black.png", "glass_black.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_GLASS,CONTENT_GLASS_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE;
	f = &content_features(i);
	f->description = std::string("Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_side.png");
	f->setTexture(4,"glass.png");
	f->setTexture(5,"glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_side.png", "glass.png", "glass_pane_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS,CONTENT_GLASS_PANE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_blue_side.png");
	f->setTexture(4,"glass_blue.png");
	f->setTexture(5,"glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_blue_side.png", "glass_blue.png", "glass_pane_blue_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_BLUE,CONTENT_GLASS_PANE_BLUE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_green_side.png");
	f->setTexture(4,"glass_green.png");
	f->setTexture(5,"glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_green_side.png", "glass_green.png", "glass_pane_green_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_GREEN,CONTENT_GLASS_PANE_GREEN);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_orange_side.png");
	f->setTexture(4,"glass_orange.png");
	f->setTexture(5,"glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_orange_side.png", "glass_orange.png", "glass_pane_orange_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_ORANGE,CONTENT_GLASS_PANE_ORANGE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_purple_side.png");
	f->setTexture(4,"glass_purple.png");
	f->setTexture(5,"glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_purple_side.png", "glass_purple.png", "glass_pane_purple_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_PURPLE,CONTENT_GLASS_PANE_PURPLE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_RED;
	f = &content_features(i);
	f->description = std::string("Red Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_red_side.png");
	f->setTexture(4,"glass_red.png");
	f->setTexture(5,"glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_red_side.png", "glass_red.png", "glass_pane_red_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_RED,CONTENT_GLASS_PANE_RED);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_yellow_side.png");
	f->setTexture(4,"glass_yellow.png");
	f->setTexture(5,"glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_yellow_side.png", "glass_yellow.png", "glass_pane_yellow_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_YELLOW,CONTENT_GLASS_PANE_YELLOW);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PANE_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Glass Pane");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_pane_black_side.png");
	f->setTexture(4,"glass_black.png");
	f->setTexture(5,"glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.5*BS,
		-0.05*BS,
		0.5*BS,
		0.5*BS,
		0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"glass_pane_black_side.png", "glass_black.png", "glass_pane_black_side.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setCol3Recipe(CONTENT_GLASS_BLACK,CONTENT_GLASS_PANE_BLACK);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_GLASS_PANE,CONTENT_GLASS_PANE_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASSLIGHT;
	f = &content_features(i);
	f->description = std::string("Glass Light");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->visual_solidness = 1;
	f->setAllTextures("glasslight.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	f->setInventoryTextureCube("glasslight.png", "glasslight.png", "glasslight.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	f->light_source = LIGHT_MAX-1;
	crafting::setSurroundRecipe(CONTENT_GLASS,CONTENT_TORCH,CONTENT_GLASSLIGHT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FENCE;
	f = &content_features(i);
	f->description = std::string("Fence");
	f->setAllTextures("wood.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_FENCELIKE;
	f->is_ground_content = true;
	f->jumpable = false;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("fence.png");
	f->used_texturenames["fence.png"] = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::setWallRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_FENCE);
	crafting::setWallRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_FENCE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_RAIL;
	f = &content_features(i);
	f->description = std::string("Rail");
	f->setAllTextures("rail.png");
	f->setTexture(1,"rail_curved.png");
	f->setTexture(2,"rail_t_junction.png");
	f->setTexture(3,"rail_crossing.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_RAILLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->walkable = false;
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_IGNORE,			CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_CRAFTITEM_WOOD_PLANK,	CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(r,CONTENT_RAIL,15);
		r[1] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[7] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		crafting::setRecipe(r,CONTENT_RAIL,15);
	}

	i = CONTENT_ROOFTILE_TERRACOTTA;
	f = &content_features(i);
	f->description = std::string("Terracotta Roof Tile");
	f->setAllTextures("rooftile_terracotta.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	crafting::set1over4Recipe(CONTENT_TERRACOTTA,CONTENT_TERRACOTTA,CONTENT_ROOFTILE_TERRACOTTA);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_WOOD;
	f = &content_features(i);
	f->description = std::string("Wood Roof Tile");
	f->setAllTextures("rooftile_wood.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	crafting::set1over4Recipe(CONTENT_WOOD,CONTENT_WOOD,CONTENT_ROOFTILE_WOOD);
	crafting::set1over4Recipe(CONTENT_JUNGLEWOOD,CONTENT_JUNGLEWOOD,CONTENT_ROOFTILE_WOOD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_ASPHALT;
	f = &content_features(i);
	f->description = std::string("Asphalt Roof Tile");
	f->setAllTextures("rooftile_asphalt.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	{
		u16 r[9] = {
			CONTENT_SAND,			CONTENT_CRAFTITEM_PAPER,	CONTENT_IGNORE,
			CONTENT_CRAFTITEM_PAPER,	CONTENT_CRAFTITEM_DYE_BLACK,	CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE,
		};
		crafting::setRecipe(r,CONTENT_ROOFTILE_ASPHALT,4);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROOFTILE_STONE;
	f = &content_features(i);
	f->description = std::string("Stone Roof Tile");
	f->setAllTextures("rooftile_stone.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_ROOFLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->type = CMT_DIRT;
	f->hardness = 0.75;
	crafting::set1over4Recipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE,CONTENT_ROOFTILE_STONE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LADDER;
	f = &content_features(i);
	f->description = std::string("Ladder");
	f->setInventoryTexture("ladder.png");
	f->setAllTextures("ladder.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLMOUNT;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem ")+itos(i)+" 1";
	f->wall_mounted = true;
	f->solidness = 0;
	f->air_equivalent = true;
	f->walkable = false;
	f->climbable = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PINE_PLANK,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PINE_PLANK,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_PINE_PLANK
		};
		crafting::setRecipe(r,CONTENT_LADDER,4);
		r[0] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[2] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[3] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[5] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[6] = CONTENT_CRAFTITEM_WOOD_PLANK;
		r[8] = CONTENT_CRAFTITEM_WOOD_PLANK;
		crafting::setRecipe(r,CONTENT_LADDER,4);
		r[0] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[2] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[3] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[5] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[6] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		r[8] = CONTENT_CRAFTITEM_JUNGLE_PLANK;
		crafting::setRecipe(r,CONTENT_LADDER,4);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BORDERSTONE;
	f = &content_features(i);
	f->description = std::string("Border Stone");
	f->setAllTextures("borderstone.png");
	f->setInventoryTextureCube("borderstone.png", "borderstone.png", "borderstone.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new BorderStoneNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 2.0;
	crafting::setFilledRoundRecipe(CONTENT_STONE,CONTENT_MESE,CONTENT_BORDERSTONE);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_STONE,			CONTENT_CRAFTITEM_STEEL_INGOT,
			CONTENT_STONE,			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_STONE,
			CONTENT_CRAFTITEM_STEEL_INGOT,	CONTENT_STONE,			CONTENT_CRAFTITEM_STEEL_INGOT
		};
		crafting::setRecipe(r,CONTENT_BORDERSTONE,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD;
	f = &content_features(i);
	f->description = std::string("Wood");
	f->setAllTextures("wood.png");
	f->setInventoryTextureCube("wood.png", "wood.png", "wood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1To4Recipe(CONTENT_TREE,CONTENT_WOOD);
	crafting::set1To4Recipe(CONTENT_APPLE_TREE,CONTENT_WOOD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_JUNGLEWOOD;
	f = &content_features(i);
	f->description = std::string("Jungle Wood");
	f->setAllTextures("junglewood.png");
	f->setInventoryTextureCube("junglewood.png", "junglewood.png", "junglewood.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1To4Recipe(CONTENT_JUNGLETREE,CONTENT_JUNGLEWOOD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_PINE;
	f = &content_features(i);
	f->description = std::string("Pine");
	f->setAllTextures("pine.png");
	f->setInventoryTextureCube("pine.png", "pine.png", "pine.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->type = CMT_WOOD;
	f->hardness = 0.5;
	crafting::set1To4Recipe(CONTENT_CONIFER_TREE,CONTENT_WOOD_PINE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TRELLIS;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set5Recipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_TRELLIS);
	crafting::set5Recipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_TRELLIS);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MESE;
	f = &content_features(i);
	f->description = std::string("Mese");
	f->setAllTextures("mese.png");
	f->setInventoryTextureCube("mese.png", "mese.png", "mese.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->onpunch_replace_node = CONTENT_MESE_DIGGING;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("CraftItem saltpeter 1");
	f->type = CMT_STONE;
	f->hardness = 0.5;
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_MESE_DIGGING;
	f = &content_features(i);
	f->description = std::string("Mese");
	f->setAllTextures("mese.png");
	f->setInventoryTextureCube("mese.png", "mese.png", "mese.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MESE)+" 1";
	f->light_source = 10;
	f->type = CMT_STONE;
	f->hardness = 0.5;

	i = CONTENT_SPONGE;
	f = &content_features(i);
	f->description = std::string("Sponge");
	f->setAllTextures("sponge.png");
	f->setInventoryTextureCube("sponge.png", "sponge.png", "sponge.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_SPONGE_FULL;
	f = &content_features(i);
	f->description = std::string("Waterlogged Sponge");
	f->setAllTextures("sponge_full.png");
	f->setInventoryTextureCube("sponge_full.png", "sponge_full.png", "sponge_full.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_DIRT;
	f->hardness = 1.0;

	i = CONTENT_HAY;
	f = &content_features(i);
	f->description = std::string("Hay Bale");
	f->setAllTextures("hay_bale.png");
	f->setInventoryTextureCube("hay_bale.png", "hay_bale.png", "hay_bale.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1;
	f->fuel_time = 20;
	f->type = CMT_PLANT;
	f->hardness = 0.6;
	crafting::setHardBlockRecipe(CONTENT_DEADGRASS,CONTENT_HAY);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FERTILIZER;
	f = &content_features(i);
	f->description = std::string("Fertilizer");
	f->setAllTextures("fertilizer.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"fertilizer.png", "fertilizer.png", "fertilizer.png");
	f->type = CMT_DIRT;
	f->hardness = 0.4;
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_ROTTEN_FRUIT,CONTENT_CRAFTITEM_ASH,CONTENT_FERTILIZER);
	crafting::set2Any2Recipe(CONTENT_CRAFTITEM_MUSH,CONTENT_CRAFTITEM_ASH,CONTENT_FERTILIZER);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SEEDS_WHEAT;
	f = &content_features(i);
	f->description = std::string("Wheat Seeds");
	f->setAllTextures("farm_seeds_wheat.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative",i);

	i = CONTENT_SEEDS_MELON;
	f = &content_features(i);
	f->description = std::string("Melon Seeds");
	f->setAllTextures("farm_seeds_melon.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_MELONSLICE,CONTENT_SEEDS_MELON);
	lists::add("creative",i);

	i = CONTENT_SEEDS_PUMPKIN;
	f = &content_features(i);
	f->description = std::string("Pumpkin Seeds");
	f->setAllTextures("farm_seeds_pumpkin.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	crafting::set1To1Recipe(CONTENT_CRAFTITEM_PUMPKINSLICE,CONTENT_SEEDS_PUMPKIN);
	lists::add("creative",i);

	i = CONTENT_SEEDS_POTATO;
	f = &content_features(i);
	f->description = std::string("Potato Seeds");
	f->setAllTextures("farm_seeds_potato.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative",i);

	i = CONTENT_SEEDS_CARROT;
	f = &content_features(i);
	f->description = std::string("Carrot Seeds");
	f->setAllTextures("farm_seeds_carrot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative",i);

	i = CONTENT_SEEDS_BEETROOT;
	f = &content_features(i);
	f->description = std::string("Beetroot Seeds");
	f->setAllTextures("farm_seeds_beetroot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative",i);

	i = CONTENT_SEEDS_GRAPE;
	f = &content_features(i);
	f->description = std::string("Grape Seeds");
	f->setAllTextures("farm_seeds_grape.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	crafting::set1To2Recipe(CONTENT_CRAFTITEM_GRAPE,CONTENT_SEEDS_GRAPE);
	lists::add("creative",i);

	i = CONTENT_SEEDS_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton Seeds");
	f->setAllTextures("farm_seeds_cotton.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	lists::add("creative",i);

	i = CONTENT_APPLE_PIE_RAW;
	f = &content_features(i);
	f->description = std::string("Raw Apple Pie");
	f->setTexture(0, "apple_pie_raw.png");
	f->setTexture(1, "apple_pie_raw.png");
	f->setTexture(2, "apple_pie_raw.png");
	f->setTexture(3, "apple_pie_raw.png");
	f->setTexture(4, "apple_pie_raw.png");
	f->setTexture(5, "apple_pie_raw.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_APPLE_PIE)+"  1";
	content_mapnode_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "apple_pie_raw.png", "apple_pie_raw.png", "apple_pie_raw.png");
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_APPLE,CONTENT_CRAFTITEM_DOUGH,CONTENT_APPLE_PIE_RAW);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_APPLE_PIE;
	f = &content_features(i);
	f->description = std::string("Apple Pie");
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png");
	f->setTexture(5, "apple_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "apple_pie.png", "apple_pie.png", "apple_pie.png");
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->ondig_replace_node=CONTENT_APPLE_PIE_3;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_APPLE_PIE_3;
	f = &content_features(i);
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png^[transformR90");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png");
	f->setTexture(5, "apple_pie.png^[transformR270");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie_3(f);
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->ondig_replace_node=CONTENT_APPLE_PIE_2;
	f->type = CMT_DIRT;
	f->hardness = 0.1;

	i = CONTENT_APPLE_PIE_2;
	f = &content_features(i);
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png^[transformFY");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png");
	f->setTexture(5, "apple_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie_2(f);
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->ondig_replace_node=CONTENT_APPLE_PIE_1;
	f->type = CMT_DIRT;
	f->hardness = 0.1;

	i = CONTENT_APPLE_PIE_1;
	f = &content_features(i);
	f->setTexture(0, "apple_pie.png");
	f->setTexture(1, "apple_pie.png");
	f->setTexture(2, "apple_pie.png^[transformFY");
	f->setTexture(3, "apple_pie.png");
	f->setTexture(4, "apple_pie.png^[transformFY");
	f->setTexture(5, "apple_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie_1(f);
	f->dug_item = std::string("CraftItem apple_pie_slice 1");
	f->type = CMT_DIRT;
	f->hardness = 0.1;

	i = CONTENT_PUMPKIN_PIE_RAW;
	f = &content_features(i);
	f->description = std::string("Raw Pumpkin Pie");
	f->setTexture(0, "pumpkin_pie_raw.png");
	f->setTexture(1, "pumpkin_pie_raw.png");
	f->setTexture(2, "pumpkin_pie_raw.png");
	f->setTexture(3, "pumpkin_pie_raw.png");
	f->setTexture(4, "pumpkin_pie_raw.png");
	f->setTexture(5, "pumpkin_pie_raw.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_PUMPKIN_PIE)+"  1";
	content_mapnode_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "pumpkin_pie_raw.png", "pumpkin_pie_raw.png", "pumpkin_pie_raw.png");
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	crafting::set1over1Recipe(CONTENT_CRAFTITEM_PUMPKINSLICE,CONTENT_CRAFTITEM_DOUGH,CONTENT_PUMPKIN_PIE_RAW);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_PUMPKIN_PIE;
	f = &content_features(i);
	f->description = std::string("Pumpkin Pie");
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png");
	f->setTexture(5, "pumpkin_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie(f);
	f->setInventoryTextureNodeBox(i, "pumpkin_pie.png", "pumpkin_pie.png", "pumpkin_pie.png");
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->ondig_replace_node=CONTENT_PUMPKIN_PIE_3;
	f->type = CMT_DIRT;
	f->hardness = 0.1;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_PUMPKIN_PIE_3;
	f = &content_features(i);
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png^[transformR90");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png");
	f->setTexture(5, "pumpkin_pie.png^[transformR270");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie_3(f);
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->ondig_replace_node=CONTENT_PUMPKIN_PIE_2;
	f->type = CMT_DIRT;
	f->hardness = 0.1;

	i = CONTENT_PUMPKIN_PIE_2;
	f = &content_features(i);
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png^[transformFY");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png");
	f->setTexture(5, "pumpkin_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie_2(f);
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->ondig_replace_node=CONTENT_PUMPKIN_PIE_1;
	f->type = CMT_DIRT;
	f->hardness = 0.1;

	i = CONTENT_PUMPKIN_PIE_1;
	f = &content_features(i);
	f->setTexture(0, "pumpkin_pie.png");
	f->setTexture(1, "pumpkin_pie.png");
	f->setTexture(2, "pumpkin_pie.png^[transformFY");
	f->setTexture(3, "pumpkin_pie.png");
	f->setTexture(4, "pumpkin_pie.png^[transformFY");
	f->setTexture(5, "pumpkin_pie.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->flammable = true;
	f->walkable = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	content_mapnode_nodebox_pie_1(f);
	f->dug_item = std::string("CraftItem pumpkin_pie_slice 1");
	f->type = CMT_DIRT;
	f->hardness = 0.1;

	i = CONTENT_FARM_WHEAT_1;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_WHEAT_2;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_WHEAT_3;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_wheat 1");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_WHEAT;
	f = &content_features(i);
	f->description = std::string("Wheat");
	f->setAllTextures("farm_wheat.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_WHEAT)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_wheat 2");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_MELON_1;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_MELON)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 3;
	content_mapnode_nodebox_plant_1(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_MELON_2;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_MELON)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 2;
	content_mapnode_nodebox_plant_2(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_MELON_3;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_MELON)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 1;
	content_mapnode_nodebox_plant_3(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_MELON;
	f = &content_features(i);
	f->description = std::string("Melon");
	f->setAllTextures("farm_melon.png");
	f->setTexture(0,"farm_melon_top.png");
	f->setTexture(1,"farm_melon_top.png");
	f->setInventoryTextureCube("farm_melon.png","farm_melon.png","farm_melon.png");
	f->draw_type = CDT_CUBELIKE;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_FARM_MELON)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_PUMPKIN_1;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_PUMPKIN)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 3;
	content_mapnode_nodebox_plant_1(f);
	f->type = CMT_PLANT;

	i = CONTENT_FARM_PUMPKIN_2;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_PUMPKIN)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 2;
	content_mapnode_nodebox_plant_2(f);
	f->type = CMT_PLANT;

	i = CONTENT_FARM_PUMPKIN_3;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->draw_type = CDT_NODEBOX;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_PUMPKIN)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 1;
	content_mapnode_nodebox_plant_3(f);
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_PUMPKIN;
	f = &content_features(i);
	f->description = std::string("Pumpkin");
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->setInventoryTextureCube("farm_pumpkin_top.png","farm_pumpkin.png","farm_pumpkin.png");
	f->draw_type = CDT_CUBELIKE;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_FARM_PUMPKIN)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_PUMPKIN_JACK;
	f = &content_features(i);
	f->description = std::string("Jack' O Lantern");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("farm_pumpkin.png");
	f->setTexture(0,"farm_pumpkin_top.png");
	f->setTexture(1,"farm_pumpkin_top.png");
	f->setTexture(5, "farm_pumpkin_jack.png"); // Z-
	f->setInventoryTextureCube("farm_pumpkin_top.png","farm_pumpkin_jack.png","farm_pumpkin.png");
	f->draw_type = CDT_CUBELIKE;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;
	f->light_source = LIGHT_MAX-1;
	crafting::set1Any2Recipe(CONTENT_TORCH,CONTENT_FARM_PUMPKIN,CONTENT_FARM_PUMPKIN_JACK);

	i = CONTENT_FARM_POTATO_1;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_POTATO_2;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_POTATO_3;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_potato 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_POTATO;
	f = &content_features(i);
	f->description = std::string("Potato");
	f->setAllTextures("farm_potato.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_POTATO)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_potato 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_CARROT_1;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_CARROT_2;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_CARROT_3;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_carrot 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_CARROT;
	f = &content_features(i);
	f->description = std::string("Carrot");
	f->setAllTextures("farm_carrot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_CARROT)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_carrot 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_BEETROOT_1;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_BEETROOT_2;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_BEETROOT_3;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_beetroot 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_BEETROOT;
	f = &content_features(i);
	f->description = std::string("Beetroot");
	f->setAllTextures("farm_beetroot.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_BEETROOT)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_beetroot 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_GRAPEVINE_1;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 3;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_GRAPEVINE_2;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem mushed_food 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_GRAPEVINE_3;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_grape 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_GRAPEVINE;
	f = &content_features(i);
	f->description = std::string("Grape");
	f->setAllTextures("farm_grapevine.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem harvested_grape 3");
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_COTTON_1;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTGROWTH_1;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_COTTON_2;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTGROWTH_2;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 1";
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_COTTON_3;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTGROWTH_3;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 1";
	f->extra_dug_item = std::string("CraftItem string 1");
	f->extra_dug_item_rarity = 2;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("farm_cotton.png");
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->setAllTextureFlags(0);
	f->walkable = false;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->flammable = 1;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_COTTON)+" 2";
	f->extra_dug_item = std::string("CraftItem string 3");
	f->extra_dug_item_rarity = 1;
	f->type = CMT_PLANT;
	f->hardness = 0.4;

	i = CONTENT_FARM_TRELLIS_GRAPE_1;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape_1.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;

	i = CONTENT_FARM_TRELLIS_GRAPE_2;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape_2.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;

	i = CONTENT_FARM_TRELLIS_GRAPE_3;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape_3.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 1";
	f->extra_dug_item = std::string("CraftItem harvested_grape 1");
	f->extra_dug_item_rarity = 1;
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;

	i = CONTENT_FARM_TRELLIS_GRAPE;
	f = &content_features(i);
	f->description = std::string("Trellis");
	f->setAllTextures("trellis_grape.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SEEDS_GRAPE)+" 2";
	f->extra_dug_item = std::string("CraftItem harvested_grape 3");
	f->extra_dug_item_rarity = 1;
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.4;

	i = CONTENT_TRELLIS_DEAD_VINE;
	f = &content_features(i);
	f->description = std::string("Dead Vine");
	f->setAllTextures("trellis_dead_vine.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ") + itos(CONTENT_DEAD_VINE)+" 1";
	f->ondig_replace_node = CONTENT_TRELLIS;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.3;

	i = CONTENT_DEAD_VINE;
	f = &content_features(i);
	f->description = std::string("Dead Vine");
	f->setAllTextures("dead_vine.png");
	f->setAllTextureFlags(0);
	f->draw_type = CDT_PLANTLIKE;
	f->param_type = CPT_LIGHT;
	f->solidness = 0;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ") + itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/4;
	f->type = CMT_WOOD;
	f->hardness = 0.3;

	i = CONTENT_SNOW_BLOCK;
	f = &content_features(i);
	f->description = std::string("Snow");
	f->setAllTextures("snow.png");
	f->setInventoryTextureCube("snow.png", "snow.png", "snow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem snow_ball 9");
	f->type = CMT_DIRT;
	f->hardness = 0.3;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_SNOW_BALL,CONTENT_SNOW_BLOCK);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SNOWMAN;
	f = &content_features(i);
	f->description = std::string("Snowman");
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->solidness = 0;
	f->setAllTextures("snowman.png");
	f->setTexture(0,"snowman_top.png");
	f->setTexture(1,"snowman_top.png");
	f->setTexture(5,"snowman_front.png");
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem snow_ball 18");
	f->extra_dug_item = std::string("CraftItem Stick 3");
	f->extra_dug_item_rarity = 1;
	f->rotate_tile_with_nodebox = true;
	f->type = CMT_DIRT;
	f->hardness = 0.3;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		0.125*BS,
		0.3125*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.25*BS,
		0.125*BS,
		-0.25*BS,
		0.25*BS,
		0.5*BS,
		0.25*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.0625*BS,
		0.3125*BS,
		-0.375*BS,
		0.*BS,
		0.375*BS,
		-0.25*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		-0.0625*BS,
		-0.0625*BS,
		0.5*BS,
		0.*BS,
		0.*BS
	));
	f->setInventoryTextureNodeBox(i,"snow.png", "snow.png", "snow.png");
	{
		content_t r[9] = {
			CONTENT_IGNORE,			CONTENT_IGNORE,		CONTENT_IGNORE,
			CONTENT_IGNORE,			CONTENT_SNOW_BLOCK,	CONTENT_CRAFTITEM_CARROT,
			CONTENT_CRAFTITEM_STICK,	CONTENT_SNOW_BLOCK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_SNOWMAN,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_SNOW;
	f = &content_features(i);
	f->description = std::string("Snow");
	f->setAllTextures("snow.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->buildable_to = true;
	f->material_pointable = false;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("CraftItem snow_ball 1");
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"snow.png", "snow.png", "snow.png");
	f->type = CMT_DIRT;
	f->hardness = 0.3;
	lists::add("creative",i);
	lists::add("decrafting",i);

	i = CONTENT_BOOK;
	f = &content_features(i);
	f->description = std::string("Book");
	f->setTexture(0, "book_cover.png");
	f->setTexture(1, "book_cover.png^[transformFX");
	f->setTexture(2, "book_side.png^[transformFY");
	f->setTexture(3, "book_side.png");
	f->setTexture(4, "book_end.png");
	f->setTexture(5, "book_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_cover.png", "book_end.png^[transformFX", "book_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setCol1Recipe(CONTENT_CRAFTITEM_PAPER,i);
	lists::add("craftguide",i);
	lists::add("creative",i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();

	i = CONTENT_COOK_BOOK;
	f = &content_features(i);
	f->description = std::string("Cook Book");
	f->setTexture(0, "book_cook_cover.png");
	f->setTexture(1, "book_cook_cover.png^[transformFX");
	f->setTexture(2, "book_cook_side.png^[transformFY");
	f->setTexture(3, "book_cook_side.png");
	f->setTexture(4, "book_cook_end.png");
	f->setTexture(5, "book_cook_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_COOK_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_cook_cover.png", "book_cook_end.png^[transformFX", "book_cook_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_COAL,i);
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_CHARCOAL,i);
	lists::add("craftguide",i);
	lists::add("creative",i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();

	i = CONTENT_DECRAFT_BOOK;
	f = &content_features(i);
	f->description = std::string("Decraft Book");
	f->setTexture(0, "book_decraft_cover.png");
	f->setTexture(1, "book_decraft_cover.png^[transformFX");
	f->setTexture(2, "book_decraft_side.png^[transformFY");
	f->setTexture(3, "book_decraft_side.png");
	f->setTexture(4, "book_decraft_end.png");
	f->setTexture(5, "book_decraft_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DECRAFT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_decraft_cover.png", "book_decraft_end.png^[transformFX", "book_decraft_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_GUNPOWDER,i);
	lists::add("craftguide",i);
	lists::add("creative",i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();

	i = CONTENT_DIARY_BOOK;
	f = &content_features(i);
	f->description = std::string("Diary");
	f->setTexture(0, "book_diary_cover.png");
	f->setTexture(1, "book_diary_cover.png^[transformFX");
	f->setTexture(2, "book_diary_side.png^[transformFY");
	f->setTexture(3, "book_diary_side.png");
	f->setTexture(4, "book_diary_end.png");
	f->setTexture(5, "book_diary_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DIARY_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_diary_cover.png", "book_diary_end.png^[transformFX", "book_diary_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_CRAFTITEM_STEEL_INGOT,i);
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CRAFT_BOOK;
	f = &content_features(i);
	f->description = std::string("Craft Book");
	f->setTexture(0, "book_craft_cover.png");
	f->setTexture(1, "book_craft_cover.png^[transformFX");
	f->setTexture(2, "book_craft_side.png^[transformFY");
	f->setTexture(3, "book_craft_side.png");
	f->setTexture(4, "book_craft_end.png");
	f->setTexture(5, "book_craft_end.png^[transformFX");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_CRAFT_BOOK_OPEN;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_book(f);
	f->setInventoryTextureNodeBox(i, "book_craft_cover.png", "book_craft_end.png^[transformFX", "book_craft_side.png^[transformFY");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_BOOK,CONTENT_MESE,i);
	{
		content_t r[9] = {
			CONTENT_CRAFTITEM_STICK, CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,
			CONTENT_IGNORE,		 CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,
			CONTENT_CRAFTITEM_STICK, CONTENT_IGNORE,	  CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_CRAFT_BOOK,1);
	}
	if (f->initial_metadata == NULL)
		f->initial_metadata = new ClosedBookNodeMetadata();
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BOOK_OPEN;
	f = &content_features(i);
	f->description = std::string("Guide");
	f->setAllTextures("guide_side.png");
	f->setTexture(0, "guide_top.png");
	f->setTexture(1, "guide_bottom.png");
	f->setTexture(4, "guide_end.png");
	f->setTexture(5, "guide_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BOOK)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_top.png", "guide_end.png", "guide_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new BookNodeMetadata();

	i = CONTENT_COOK_BOOK_OPEN;
	f = &content_features(i);
	f->description = std::string("Cooking Guide");
	f->setAllTextures("guide_cook_side.png");
	f->setTexture(0, "guide_cook_top.png");
	f->setTexture(1, "guide_cook_bottom.png");
	f->setTexture(4, "guide_cook_end.png");
	f->setTexture(5, "guide_cook_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_COOK_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_COOK_BOOK)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_cook_top.png", "guide_cook_end.png", "guide_cook_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CookBookNodeMetadata();

	i = CONTENT_DECRAFT_BOOK_OPEN;
	f = &content_features(i);
	f->description = std::string("Decrafting Guide");
	f->setAllTextures("guide_decraft_side.png");
	f->setTexture(0, "guide_decraft_top.png");
	f->setTexture(1, "guide_decraft_bottom.png");
	f->setTexture(4, "guide_decraft_end.png");
	f->setTexture(5, "guide_decraft_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DECRAFT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_DECRAFT_BOOK)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_decraft_top.png", "guide_decraft_end.png", "guide_decraft_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DeCraftNodeMetadata();

	i = CONTENT_DIARY_BOOK_OPEN;
	f = &content_features(i);
	f->description = std::string("Diary");
	f->setAllTextures("guide_diary_side.png");
	f->setTexture(0, "guide_diary_top.png");
	f->setTexture(1, "guide_diary_bottom.png");
	f->setTexture(4, "guide_diary_end.png");
	f->setTexture(5, "guide_diary_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_DIARY_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_DIARY_BOOK)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_diary_top.png", "guide_diary_end.png", "guide_diary_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new DiaryNodeMetadata();

	i = CONTENT_CRAFT_BOOK_OPEN;
	f = &content_features(i);
	f->description = std::string("Craft Guide");
	f->setAllTextures("guide_craft_side.png");
	f->setTexture(0, "guide_craft_top.png");
	f->setTexture(1, "guide_craft_bottom.png");
	f->setTexture(4, "guide_craft_end.png");
	f->setTexture(5, "guide_craft_end.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->rotate_tile_with_nodebox = true;
	f->light_propagates = true;
	f->air_equivalent = true;
	f->onpunch_replace_node = CONTENT_CRAFT_BOOK;
	f->flammable = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BOOK)+" 1";
	f->solidness = 0;
	content_mapnode_nodebox_guide(f);
	f->setInventoryTextureNodeBox(i, "guide_craft_top.png", "guide_craft_end.png", "guide_craft_side.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	if (f->initial_metadata == NULL)
		f->initial_metadata = new CraftGuideNodeMetadata();

	i = CONTENT_COTTON;
	f = &content_features(i);
	f->description = std::string("Cotton");
	f->setAllTextures("cotton.png");
	f->setInventoryTextureCube("cotton.png", "cotton.png", "cotton.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_JUNGLEGRASS,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_BLUE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_GREEN,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_ORANGE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_PURPLE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_RED,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_YELLOW,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_COTTON_BLACK,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_BLUE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_GREEN,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_ORANGE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_PURPLE,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_RED,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_YELLOW,CONTENT_COTTON);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_STARCH,CONTENT_COTTON_BLACK,CONTENT_COTTON);
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_STRING,CONTENT_COTTON);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Cotton");
	f->setAllTextures("cotton_blue.png");
	f->setInventoryTextureCube("cotton_blue.png", "cotton_blue.png", "cotton_blue.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_COTTON,CONTENT_COTTON_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Cotton");
	f->setAllTextures("cotton_green.png");
	f->setInventoryTextureCube("cotton_green.png", "cotton_green.png", "cotton_green.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_COTTON,CONTENT_COTTON_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Cotton");
	f->setAllTextures("cotton_orange.png");
	f->setInventoryTextureCube("cotton_orange.png", "cotton_orange.png", "cotton_orange.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_COTTON,CONTENT_COTTON_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Cotton");
	f->setAllTextures("cotton_purple.png");
	f->setInventoryTextureCube("cotton_purple.png", "cotton_purple.png", "cotton_purple.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_COTTON,CONTENT_COTTON_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_RED;
	f = &content_features(i);
	f->description = std::string("Red Cotton");
	f->setAllTextures("cotton_red.png");
	f->setInventoryTextureCube("cotton_red.png", "cotton_red.png", "cotton_red.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_COTTON,CONTENT_COTTON_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Cotton");
	f->setAllTextures("cotton_yellow.png");
	f->setInventoryTextureCube("cotton_yellow.png", "cotton_yellow.png", "cotton_yellow.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_COTTON,CONTENT_COTTON_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COTTON_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Cotton");
	f->setAllTextures("cotton_black.png");
	f->setInventoryTextureCube("cotton_black.png", "cotton_black.png", "cotton_black.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 15;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_COTTON,CONTENT_COTTON_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET;
	f = &content_features(i);
	f->description = std::string("Carpet");
	f->setAllTextures("cotton.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton.png", "cotton.png", "cotton.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON,CONTENT_CARPET);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Carpet");
	f->setAllTextures("cotton_blue.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_blue.png", "cotton_blue.png", "cotton_blue.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_BLUE,CONTENT_CARPET_BLUE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_CARPET,CONTENT_CARPET_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_GREEN;
	f = &content_features(i);
	f->description = std::string("Green Carpet");
	f->setAllTextures("cotton_green.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_green.png", "cotton_green.png", "cotton_green.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_GREEN,CONTENT_CARPET_GREEN);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_CARPET,CONTENT_CARPET_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_ORANGE;
	f = &content_features(i);
	f->description = std::string("Orange Carpet");
	f->setAllTextures("cotton_orange.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_orange.png", "cotton_orange.png", "cotton_orange.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_ORANGE,CONTENT_CARPET_ORANGE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_CARPET,CONTENT_CARPET_ORANGE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_PURPLE;
	f = &content_features(i);
	f->description = std::string("Purple Carpet");
	f->setAllTextures("cotton_purple.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_purple.png", "cotton_purple.png", "cotton_purple.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_PURPLE,CONTENT_CARPET_PURPLE);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_CARPET,CONTENT_CARPET_PURPLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_RED;
	f = &content_features(i);
	f->description = std::string("Red Carpet");
	f->setAllTextures("cotton_red.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_red.png", "cotton_red.png", "cotton_red.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_RED,CONTENT_CARPET_RED);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_RED,CONTENT_CARPET,CONTENT_CARPET_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_YELLOW;
	f = &content_features(i);
	f->description = std::string("Yellow Carpet");
	f->setAllTextures("cotton_yellow.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_yellow.png", "cotton_yellow.png", "cotton_yellow.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_YELLOW,CONTENT_CARPET_YELLOW);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_CARPET,CONTENT_CARPET_YELLOW);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CARPET_BLACK;
	f = &content_features(i);
	f->description = std::string("Black Carpet");
	f->setAllTextures("cotton_black.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_carpet(f);
	f->setInventoryTextureNodeBox(i,"cotton_black.png", "cotton_black.png", "cotton_black.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 5;
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setTileRecipe(CONTENT_COTTON_BLACK,CONTENT_CARPET_BLACK);
	crafting::set1Any2Recipe(CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_CARPET,CONTENT_CARPET_BLACK);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_AIR;
	f = &content_features(i);
	f->description = std::string("Air");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_AIRLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->air_equivalent = true;

	i = CONTENT_WATER;
	f = &content_features(i);
	f->description = std::string("Water");
	f->setAllTextures("water.png");
	f->setInventoryTextureCube("water.png", "water.png", "water.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID;
	f->light_propagates = true;
	f->solidness = 0; // Drawn separately, makes no faces
	f->visual_solidness = 1;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->liquid_type = LIQUID_FLOWING;
	f->liquid_alternative_flowing = CONTENT_WATER;
	f->liquid_alternative_source = CONTENT_WATERSOURCE;
	f->liquid_viscosity = WATER_VISC;
#ifndef SERVER
	if(!opaque_water)
		f->vertex_alpha = WATER_ALPHA;
	f->setAllTextureFlags(0);
	f->setAllTextureTypes(MATERIAL_ALPHA_VERTEX);
	f->post_effect_color = video::SColor(64, 100, 100, 200);
#endif

	i = CONTENT_WATERSOURCE;
	f = &content_features(i);
	f->description = std::string("Water");
	f->setAllTextures("water.png");
	f->setInventoryTextureCube("water.png", "water.png", "water.png");
	if(new_style_water)
	{
		f->solidness = 0; // drawn separately, makes no faces
	}
	else // old style
	{
		f->solidness = 1;
#ifndef SERVER
		TileSpec t;
		if(g_texturesource)
			t.texture = g_texturesource->getTexture("water.png");

		if(!opaque_water){
			t.alpha = WATER_ALPHA;
			t.material_type = MATERIAL_ALPHA_VERTEX;
		}
		t.material_flags &= ~MATERIAL_FLAG_BACKFACE_CULLING;
		f->setAllTiles(t);
#endif
	}
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = true;
	f->walkable = false;
	f->pointable = false;
	f->buildable_to = true;
	f->liquid_type = LIQUID_SOURCE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->liquid_alternative_flowing = CONTENT_WATER;
	f->liquid_alternative_source = CONTENT_WATERSOURCE;
	f->liquid_viscosity = WATER_VISC;
#ifndef SERVER
	if(!opaque_water)
		f->vertex_alpha = WATER_ALPHA;
	f->setAllTextureFlags(0);
	f->setAllTextureTypes(MATERIAL_ALPHA_VERTEX);
	f->post_effect_color = video::SColor(64, 100, 100, 200);
#endif
	f->type = CMT_LIQUID;
	f->hardness = 0.5;

	i = CONTENT_LAVA;
	f = &content_features(i);
	f->description = std::string("Lava");
	f->setAllTextures("lava.png");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID;
	f->light_propagates = false;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->visual_solidness = 1; // Does not completely cover block boundaries
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->flammable = 2; // can be set on fire
	f->fuel_time = 100;
	f->liquid_type = LIQUID_FLOWING;
	f->liquid_alternative_flowing = CONTENT_LAVA;
	f->liquid_alternative_source = CONTENT_LAVASOURCE;
	f->liquid_viscosity = LAVA_VISC;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif

	i = CONTENT_LAVASOURCE;
	f = &content_features(i);
	f->description = std::string("Lava");
	f->setAllTextures("lava.png");
	f->setInventoryTextureCube("lava.png", "lava.png", "lava.png");
	if(new_style_water)
	{
		f->solidness = 0; // drawn separately, makes no faces
	}
	else // old style
	{
		f->solidness = 2;
#ifndef SERVER
		TileSpec t;
		if(g_texturesource)
			t.texture = g_texturesource->getTexture("lava.png");

		//t.alpha = 255;
		//t.material_type = MATERIAL_ALPHA_VERTEX;
		//t.material_flags &= ~MATERIAL_FLAG_BACKFACE_CULLING;
		f->setAllTiles(t);
#endif
	}
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_LIQUID;
	f->draw_type = CDT_LIQUID_SOURCE;
	f->light_propagates = false;
	f->light_source = LIGHT_MAX-1;
	f->walkable = false;
	f->pointable = false;
	//f->diggable = false;
	f->buildable_to = true;
	f->flammable = 2; // can be set on fire
	f->fuel_time = 100;
	f->liquid_type = LIQUID_SOURCE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->liquid_alternative_flowing = CONTENT_LAVA;
	f->liquid_alternative_source = CONTENT_LAVASOURCE;
	f->liquid_viscosity = LAVA_VISC;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif
	f->type = CMT_LIQUID;
	f->hardness = 0.5;

	i = CONTENT_FIRE;
	f = &content_features(i);
	f->description = std::string("Fire");
	f->setAllTextures("fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif

	i = CONTENT_FIRE_SHORTTERM;
	f = &content_features(i);
	f->description = std::string("Fire");
	f->setAllTextures("fire.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->solidness = 0; // Drawn separately, makes no faces
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 4*2;
#ifndef SERVER
	f->post_effect_color = video::SColor(192, 255, 64, 0);
#endif

	i = CONTENT_TORCH;
	f = &content_features(i);
	f->description = std::string("Torch");
	f->setAllTextures("torch.png");
	f->setInventoryTexture("torch_inventory.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_TORCHLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 0.5;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_source = LIGHT_MAX-1;
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	crafting::set1over4Recipe(CONTENT_CRAFTITEM_COAL,CONTENT_CRAFTITEM_STICK,CONTENT_TORCH);
	crafting::set1over4Recipe(CONTENT_CRAFTITEM_CHARCOAL,CONTENT_CRAFTITEM_STICK,CONTENT_TORCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SIGN_WALL;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign_wall.png");
	f->setInventoryTexture("sign_inventory.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_SIGNLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->floormount_alternate_node = CONTENT_SIGN;
	f->roofmount_alternate_node = CONTENT_SIGN_UD;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;

	i = CONTENT_SIGN;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_front.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->wallmount_alternate_node = CONTENT_SIGN_WALL;
	f->roofmount_alternate_node = CONTENT_SIGN_UD;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.5*BS,
		-0.05*BS,
		0.05*BS,
		0.5*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		0.,
		-0.1*BS,
		0.4*BS,
		0.4*BS,
		-0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_front.png", "sign.png");
	crafting::setSignRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_SIGN);
	crafting::setSignRecipe(CONTENT_CRAFTITEM_PINE_PLANK,CONTENT_SIGN);
	crafting::setSignRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_SIGN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SIGN_UD;
	f = &content_features(i);
	f->description = std::string("Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back_ud.png");
	f->setTexture(5, "sign_front_ud.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->wallmount_alternate_node = CONTENT_SIGN_WALL;
	f->floormount_alternate_node = CONTENT_SIGN;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new SignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.5*BS,
		-0.05*BS,
		0.05*BS,
		0.5*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		-0.4*BS,
		-0.1*BS,
		0.4*BS,
		0.,
		-0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_front.png", "sign.png");

	i = CONTENT_LOCKABLE_SIGN_WALL;
	f = &content_features(i);
	f->description = std::string("Locking Sign");
	f->setAllTextures("sign_wall_lock.png");
	f->setInventoryTexture("sign_lock_inventory.png");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_SIGNLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->floormount_alternate_node = CONTENT_LOCKABLE_SIGN;
	f->roofmount_alternate_node = CONTENT_LOCKABLE_SIGN_UD;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_LOCKABLE_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingSignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;

	i = CONTENT_LOCKABLE_SIGN;
	f = &content_features(i);
	f->description = std::string("Locking Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back.png");
	f->setTexture(5, "sign_lock.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->wallmount_alternate_node = CONTENT_LOCKABLE_SIGN_WALL;
	f->roofmount_alternate_node = CONTENT_LOCKABLE_SIGN_UD;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingSignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.5*BS,
		-0.05*BS,
		0.05*BS,
		0.5*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		0.,
		-0.1*BS,
		0.4*BS,
		0.4*BS,
		-0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_lock.png", "sign.png");
	crafting::set1Any2Recipe(CONTENT_SIGN,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_SIGN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LOCKABLE_SIGN_UD;
	f = &content_features(i);
	f->description = std::string("Locking Sign");
	f->setAllTextures("sign.png");
	f->setTexture(4, "sign_back_ud.png");
	f->setTexture(5, "sign_lock_ud.png"); // Z-
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->wallmount_alternate_node = CONTENT_LOCKABLE_SIGN_WALL;
	f->floormount_alternate_node = CONTENT_LOCKABLE_SIGN;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_LOCKABLE_SIGN)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingSignNodeMetadata("Some sign");
	f->type = CMT_WOOD;
	f->hardness = 0.1;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.05*BS,
		-0.5*BS,
		-0.05*BS,
		0.05*BS,
		0.5*BS,
		0.05*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS,
		-0.4*BS,
		-0.1*BS,
		0.4*BS,
		0.,
		-0.05*BS
	));
	f->setInventoryTextureNodeBox(i,"sign.png", "sign_lock.png", "sign.png");

	i = CONTENT_CHEST;
	f = &content_features(i);
	f->description = std::string("Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_front.png"); // Z-
	f->setInventoryTexture("chest_top.png");
	f->setInventoryTextureCube("chest_top.png", "chest_front.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new ChestNodeMetadata();
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	crafting::setRoundRecipe(CONTENT_WOOD,CONTENT_CHEST);
	crafting::setRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CHEST);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_CREATIVE_CHEST;
	f = &content_features(i);
	f->description = std::string("Creative Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_creative.png"); // Z-
	f->setInventoryTexture("chest_top.png");
	f->setInventoryTextureCube("chest_top.png", "chest_creative.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new CreativeChestNodeMetadata();
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	lists::add("creative",i);

	i = CONTENT_LOCKABLE_CHEST;
	f = &content_features(i);
	f->description = std::string("Locking Chest");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("chest_side.png");
	f->setTexture(0, "chest_top.png");
	f->setTexture(1, "chest_top.png");
	f->setTexture(5, "chest_lock.png"); // Z-
	f->setInventoryTexture("chest_lock.png");
	f->setInventoryTextureCube("chest_top.png", "chest_lock.png", "chest_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingChestNodeMetadata();
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	crafting::setFilledRoundRecipe(CONTENT_WOOD,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::set1Any2Recipe(CONTENT_CHEST,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_WOOD,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::set1Any2Recipe(CONTENT_CHEST,CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_WOOD,CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::setFilledRoundRecipe(CONTENT_JUNGLEWOOD,CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_LOCKABLE_CHEST);
	crafting::set1Any2Recipe(CONTENT_CHEST,CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_LOCKABLE_CHEST);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FURNACE;
	f = &content_features(i);
	f->description = std::string("Furnace");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("furnace_side.png");
	f->setTexture(0, "furnace_top.png");
	f->setTexture(1, "furnace_top.png");
	f->setTexture(5, "furnace_front.png"); // Z-
	f->setInventoryTextureCube("furnace_top.png", "furnace_front.png", "furnace_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new FurnaceNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 3.0;
	crafting::setRoundRecipe(CONTENT_ROUGHSTONE,CONTENT_FURNACE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_LOCKABLE_FURNACE;
	f = &content_features(i);
	f->description = std::string("Locking Furnace");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("furnace_side.png");
	f->setTexture(0, "furnace_top.png");
	f->setTexture(1, "furnace_top.png");
	f->setTexture(5, "furnace_lock.png"); // Z-
	f->setInventoryTextureCube("furnace_top.png", "furnace_lock.png", "furnace_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockingFurnaceNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 3.0;
	crafting::setFilledRoundRecipe(CONTENT_ROUGHSTONE,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_FURNACE);
	crafting::set1Any2Recipe(CONTENT_FURNACE,CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_LOCKABLE_FURNACE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_INCINERATOR;
	f = &content_features(i);
	f->description = std::string("Incinerator");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("incinerator_side.png");
	f->setTexture(0, "incinerator_top.png"); // Z-
	f->setTexture(1, "incinerator_top.png"); // Z-
	f->setTexture(5, "incinerator_front.png"); // Z-
	f->setInventoryTextureCube("incinerator_top.png", "incinerator_front.png", "incinerator_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new IncineratorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 0.4;
	crafting::setFilledRoundRecipe(CONTENT_ROUGHSTONE,CONTENT_MESE,CONTENT_INCINERATOR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONE;
	f = &content_features(i);
	f->description = std::string("Rough Stone");
	f->setAllTextures("roughstone.png");
	f->setInventoryTextureCube("roughstone.png", "roughstone.png", "roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_STONE)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setBlockRecipe(CONTENT_COBBLE,CONTENT_ROUGHSTONE);
	lists::add("craftguide",i);
	lists::add("creative",i);
	lists::add("cooking",i);

	i = CONTENT_COBBLE;
	f = &content_features(i);
	f->description = std::string("Cobble Stone");
	f->setAllTextures("cobble.png");
	f->setInventoryTextureCube("cobble.png", "cobble.png", "cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::set5Recipe(CONTENT_ROUGHSTONE,CONTENT_COBBLE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE;
	f = &content_features(i);
	f->description = std::string("Mossy Cobble Stone");
	f->setAllTextures("mossycobble.png");
	f->setInventoryTextureCube("mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.8;
	lists::add("creative",i);

	i = CONTENT_STEEL;
	f = &content_features(i);
	f->description = std::string("Steel Block");
	f->setAllTextures("steel_block.png");
	f->setInventoryTextureCube("steel_block.png", "steel_block.png", "steel_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL);
	crafting::setUncraftHardBlockRecipe(CONTENT_STEEL,CONTENT_CRAFTITEM_STEEL_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COPPER;
	f = &content_features(i);
	f->description = std::string("Copper Block");
	f->setAllTextures("copper_block.png");
	f->setInventoryTextureCube("copper_block.png", "copper_block.png", "copper_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_COPPER_INGOT,CONTENT_COPPER);
	crafting::setUncraftHardBlockRecipe(CONTENT_COPPER,CONTENT_CRAFTITEM_COPPER_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GOLD;
	f = &content_features(i);
	f->description = std::string("Gold Block");
	f->setAllTextures("gold_block.png");
	f->setInventoryTextureCube("gold_block.png", "gold_block.png", "gold_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_GOLD_INGOT,CONTENT_GOLD);
	crafting::setUncraftHardBlockRecipe(CONTENT_GOLD,CONTENT_CRAFTITEM_GOLD_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SILVER;
	f = &content_features(i);
	f->description = std::string("Silver Block");
	f->setAllTextures("silver_block.png");
	f->setInventoryTextureCube("silver_block.png", "silver_block.png", "silver_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_SILVER_INGOT,CONTENT_SILVER);
	crafting::setUncraftHardBlockRecipe(CONTENT_SILVER,CONTENT_CRAFTITEM_SILVER_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TIN;
	f = &content_features(i);
	f->description = std::string("Tin Block");
	f->setAllTextures("tin_block.png");
	f->setInventoryTextureCube("tin_block.png", "tin_block.png", "tin_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_TIN_INGOT,CONTENT_TIN);
	crafting::setUncraftHardBlockRecipe(CONTENT_TIN,CONTENT_CRAFTITEM_TIN_INGOT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_QUARTZ;
	f = &content_features(i);
	f->description = std::string("Quartz Block");
	f->setAllTextures("quartz_block.png");
	f->setInventoryTextureCube("quartz_block.png", "quartz_block.png", "quartz_block.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setHardBlockRecipe(CONTENT_CRAFTITEM_QUARTZ,CONTENT_QUARTZ);
	crafting::setUncraftHardBlockRecipe(CONTENT_QUARTZ,CONTENT_CRAFTITEM_QUARTZ);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_NC;
	f = &content_features(i);
	f->description = std::string("Nyan Cat");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("nc_side.png");
	f->setTexture(5, "nc_front.png"); // Z-
	f->setTexture(4, "nc_back.png"); // Z+
	f->setInventoryTextureCube("nc_front.png", "nc_side.png", "nc_side.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 3.0;
	lists::add("creative",i);

	i = CONTENT_NC_RB;
	f = &content_features(i);
	f->description = std::string("Rainbow");
	f->draw_type = CDT_CUBELIKE;
	f->setAllTextures("nc_rb.png");
	f->setInventoryTextureCube("nc_rb.png", "nc_rb.png", "nc_rb.png");
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 3.0;
	lists::add("creative",i);

	i = CONTENT_SAPLING;
	f = &content_features(i);
	f->description = std::string("Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("sapling.png");
	f->setInventoryTexture("sapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	lists::add("creative",i);

	i = CONTENT_APPLE_SAPLING;
	f = &content_features(i);
	f->description = std::string("Apple Tree Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("apple_sapling.png");
	f->setInventoryTexture("apple_sapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	lists::add("creative",i);

	i = CONTENT_JUNGLESAPLING;
	f = &content_features(i);
	f->description = std::string("Jungle Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("junglesapling.png");
	f->setInventoryTexture("junglesapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	lists::add("creative",i);

	i = CONTENT_CONIFER_SAPLING;
	f = &content_features(i);
	f->description = std::string("Conifer Sapling");
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->setAllTextures("conifer_sapling.png");
	f->setInventoryTexture("conifer_sapling.png");
	f->setAllTextureFlags(0);
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->light_propagates = true;
	f->air_equivalent = false;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	lists::add("creative",i);

	i = CONTENT_APPLE;
	f = &content_features(i);
	f->description = std::string("Apple");
	f->setInventoryTexture("apple.png");
	f->setAllTextures("apple.png");
	f->setAllTextureFlags(0);
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE_SML;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("CraftItem apple 1");
	f->type = CMT_WOOD;
	f->hardness = 0.0;
	lists::add("creative",i);

	// slabs
	i = CONTENT_ROUGHSTONE_SLAB;
	f = &content_features(i);
	f->description = std::string("Rough Stone Slab");
	f->setAllTextures("roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_ROUGHSTONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setRow3Recipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COBBLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Cobble Stone Slab");
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_COBBLE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setRow3Recipe(CONTENT_COBBLE,CONTENT_COBBLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Mossy Cobble Stone Slab");
	f->setAllTextures("mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_MOSSYCOBBLE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;
	crafting::setRow3Recipe(CONTENT_MOSSYCOBBLE,CONTENT_MOSSYCOBBLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONE_SLAB;
	f = &content_features(i);
	f->description = std::string("Stone Slab");
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_STONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setRow3Recipe(CONTENT_STONE,CONTENT_STONE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_SLAB;
	f = &content_features(i);
	f->description = std::string("Wood Slab");
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_WOOD;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::setRow3Recipe(CONTENT_WOOD,CONTENT_WOOD_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_JUNGLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Jungle Wood Slab");
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_JUNGLEWOOD;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	crafting::setRow3Recipe(CONTENT_JUNGLEWOOD,CONTENT_JUNGLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BRICK_SLAB;
	f = &content_features(i);
	f->description = std::string("Brick Slab");
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->slab_cube_type = CONTENT_BRICK;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setRow3Recipe(CONTENT_BRICK,CONTENT_BRICK_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SANDSTONE_SLAB;
	f = &content_features(i);
	f->description = std::string("Sand Stone Slab");
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->slab_cube_type = CONTENT_SANDSTONE;
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setRow3Recipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_SLAB;
	f = &content_features(i);
	f->description = std::string("Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass.png", "glass.png", "glass.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_BLUE_SLAB;
	f = &content_features(i);
	f->description = std::string("Blue Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLUE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_blue.png", "glass_blue.png", "glass_blue.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_BLUE_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_GLASS_BLUE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_GREEN_SLAB;
	f = &content_features(i);
	f->description = std::string("Green Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_GREEN;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_green.png", "glass_green.png", "glass_green.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_GREEN_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_GLASS_GREEN_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_ORANGE_SLAB;
	f = &content_features(i);
	f->description = std::string("Orange Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_ORANGE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_orange.png", "glass_orange.png", "glass_orange.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_ORANGE_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_ORANGE,CONTENT_GLASS_ORANGE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_PURPLE_SLAB;
	f = &content_features(i);
	f->description = std::string("Purple Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_PURPLE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_purple.png", "glass_purple.png", "glass_purple.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_PURPLE_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_PURPLE,CONTENT_GLASS_PURPLE_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_RED_SLAB;
	f = &content_features(i);
	f->description = std::string("Red Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_RED;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_red.png", "glass_red.png", "glass_red.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_RED_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_RED,CONTENT_GLASS_RED_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_YELLOW_SLAB;
	f = &content_features(i);
	f->description = std::string("Yellow Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_YELLOW;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_yellow.png", "glass_yellow.png", "glass_yellow.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_YELLOW_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_YELLOW,CONTENT_GLASS_YELLOW_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_BLACK_SLAB;
	f = &content_features(i);
	f->description = std::string("Black Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLACK;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slab(f);
	f->setInventoryTextureNodeBox(i,"glass_black.png", "glass_black.png", "glass_black.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::setRow3Recipe(CONTENT_GLASS,CONTENT_GLASS_BLACK_SLAB);
	crafting::set1Any2Recipe(CONTENT_GLASS_SLAB,CONTENT_CRAFTITEM_DYE_BLACK,CONTENT_GLASS_BLACK_SLAB);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// upside down slabs
	i = CONTENT_ROUGHSTONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("roughstone.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_ROUGHSTONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;

	i = CONTENT_COBBLE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Cobble Stone Slab");
	f->setAllTextures("cobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_COBBLE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;

	i = CONTENT_MOSSYCOBBLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("mossycobble.png");
	f->param_type = CPT_NONE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MOSSYCOBBLE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_MOSSYCOBBLE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;

	i = CONTENT_STONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STONE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_STONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;

	i = CONTENT_WOOD_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_SLAB)+" 1";
	f->slab_cube_type = CONTENT_WOOD;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_JUNGLE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLE_SLAB)+" 1";
	f->slab_cube_type = CONTENT_JUNGLEWOOD;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;

	i = CONTENT_BRICK_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	f->slab_cube_type = CONTENT_BRICK;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;

	i = CONTENT_SANDSTONE_SLAB_UD;
	f = &content_features(i);
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	f->slab_cube_type = CONTENT_SANDSTONE;
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;

	i = CONTENT_GLASS_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass.png", "glass.png", "glass.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_BLUE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Blue Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLUE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_blue.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_blue.png", "glass_blue.png", "glass_blue.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_GREEN_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Green Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_GREEN;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_green.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_green.png", "glass_green.png", "glass_green.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_ORANGE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Orange Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_ORANGE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_orange.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_orange.png", "glass_orange.png", "glass_orange.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_PURPLE_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Purple Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_PURPLE;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_purple.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_purple.png", "glass_purple.png", "glass_purple.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_RED_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Red Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_RED;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_red.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_red.png", "glass_red.png", "glass_red.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_YELLOW_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Yellow Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_YELLOW;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_yellow.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_yellow.png", "glass_yellow.png", "glass_yellow.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_GLASS_BLACK_SLAB_UD;
	f = &content_features(i);
	f->description = std::string("Black Glass Slab");
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_NODEBOX;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->slab_cube_type = CONTENT_GLASS_BLACK;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("glass_black.png");
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
#endif
	content_mapnode_nodebox_slabud(f);
	f->setInventoryTextureNodeBox(i,"glass_black.png", "glass_black.png", "glass_black.png");
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	// stairs
	i = CONTENT_ROUGHSTONE_STAIR;
	f = &content_features(i);
	f->description = std::string("Rough Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("roughstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setStairRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_COBBLE_STAIR;
	f = &content_features(i);
	f->description = std::string("Cobble Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("cobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setStairRecipe(CONTENT_COBBLE,CONTENT_COBBLE_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE_STAIR;
	f = &content_features(i);
	f->description = std::string("Mossy Cobble Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("mossycobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;
	crafting::setStairRecipe(CONTENT_MOSSYCOBBLE,CONTENT_MOSSYCOBBLE_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONE_STAIR;
	f = &content_features(i);
	f->description = std::string("Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->often_contains_mineral = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setStairRecipe(CONTENT_STONE,CONTENT_STONE_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_STAIR;
	f = &content_features(i);
	f->description = std::string("Wood Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->setAllTextures("wood.png");
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::setStairRecipe(CONTENT_WOOD,CONTENT_WOOD_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_JUNGLE_STAIR;
	f = &content_features(i);
	f->description = std::string("Jungle Wood Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;
	crafting::setStairRecipe(CONTENT_JUNGLEWOOD,CONTENT_JUNGLE_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BRICK_STAIR;
	f = &content_features(i);
	f->description = std::string("Brick Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setStairRecipe(CONTENT_BRICK,CONTENT_BRICK_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SANDSTONE_STAIR;
	f = &content_features(i);
	f->description = std::string("Sand Stone Stair");
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	content_mapnode_nodebox_stair(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setStairRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_STAIR);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// upside down stairs
	i = CONTENT_ROUGHSTONE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("roughstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"roughstone.png", "roughstone.png", "roughstone.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;

	i = CONTENT_COBBLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("cobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_ROUGHSTONE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"cobble.png", "cobble.png", "cobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;

	i = CONTENT_MOSSYCOBBLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("mossycobble.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_MOSSYCOBBLE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"mossycobble.png", "mossycobble.png", "mossycobble.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;

	i = CONTENT_STONE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("stone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STONE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"stone.png", "stone.png", "stone.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;

	i = CONTENT_WOOD_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("wood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"wood.png", "wood.png", "wood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_JUNGLE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("junglewood.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	//f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_JUNGLE_STAIR)+" 1";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"junglewood.png", "junglewood.png", "junglewood.png");
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	f->type = CMT_WOOD;
	f->hardness = 1.0;

	i = CONTENT_BRICK_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("brick.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("CraftItem clay_brick 4");
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"brick.png", "brick.png", "brick.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;

	i = CONTENT_SANDSTONE_STAIR_UD;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->setAllTextures("sandstone.png");
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_SAND)+" 4";
	content_mapnode_nodebox_stairud(f);
	f->setInventoryTextureNodeBox(i,"sandstone.png", "sandstone.png", "sandstone.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;

	// doors
	i = CONTENT_WOOD_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(2,"door_wood_b.png^[transformFX");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setTexture(4,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(2,"door_wood_t.png^[transformFX");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setTexture(4,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1over1Recipe(CONTENT_WOOD_HATCH,CONTENT_WOOD_HATCH,CONTENT_WOOD_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_WOOD_DOOR_RT,CONTENT_WOOD_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(2,"door_steel_b.png^[transformFX");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setTexture(4,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_t.png");
	f->setTexture(2,"door_steel_t.png^[transformFX");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setTexture(4,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::set1over1Recipe(CONTENT_STEEL_HATCH,CONTENT_STEEL_HATCH,CONTENT_STEEL_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_STEEL_DOOR_RT,CONTENT_STEEL_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(2,"door_glass_b.png^[transformFX");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setTexture(4,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	i = CONTENT_GLASS_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(2,"door_glass_t.png^[transformFX");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setTexture(4,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1over1Recipe(CONTENT_GLASS_PANE,CONTENT_GLASS_PANE,CONTENT_GLASS_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_GLASS_DOOR_RT,CONTENT_GLASS_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_W_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(2,"door_wood_wb.png^[transformFX");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setTexture(4,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_W_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(2,"door_wood_wt.png^[transformFX");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setTexture(4,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorw(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1over1Recipe(CONTENT_WOOD_W_HATCH,CONTENT_WOOD_HATCH,CONTENT_WOOD_W_DOOR_LT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_WOOD_DOOR_LT,CONTENT_WOOD_W_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_WOOD_W_DOOR_RT,CONTENT_WOOD_W_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_W_DOOR_LB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(2,"door_steel_wb.png^[transformFX");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setTexture(4,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_W_DOOR_LT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(2,"door_steel_wt.png^[transformFX");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setTexture(4,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_doorw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::set1over1Recipe(CONTENT_STEEL_W_HATCH,CONTENT_STEEL_HATCH,CONTENT_STEEL_W_DOOR_LT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_STEEL_DOOR_LT,CONTENT_STEEL_W_DOOR_LT);
	crafting::set1To1Recipe(CONTENT_STEEL_W_DOOR_RT,CONTENT_STEEL_W_DOOR_LT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// right doors
	i = CONTENT_WOOD_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_b.png^[transformFX");
	f->setTexture(2,"door_wood_b.png");
	f->setTexture(3,"door_wood_b.png");
	f->setTexture(4,"door_wood_b.png");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_t.png^[transformFX");
	f->setTexture(2,"door_wood_t.png");
	f->setTexture(3,"door_wood_t.png");
	f->setTexture(4,"door_wood_t.png");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1To1Recipe(CONTENT_WOOD_DOOR_LT,CONTENT_WOOD_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_GLASS_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_b.png^[transformFX");
	f->setTexture(2,"door_glass_b.png");
	f->setTexture(3,"door_glass_b.png");
	f->setTexture(4,"door_glass_b.png");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	i = CONTENT_GLASS_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_t.png^[transformFX");
	f->setTexture(2,"door_glass_t.png");
	f->setTexture(3,"door_glass_t.png");
	f->setTexture(4,"door_glass_t.png");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	crafting::set1To1Recipe(CONTENT_GLASS_DOOR_LT,CONTENT_GLASS_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png^[transformFX");
	f->setTexture(2,"door_steel_b.png");
	f->setTexture(3,"door_steel_b.png");
	f->setTexture(4,"door_steel_b.png");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_t.png^[transformFX");
	f->setTexture(2,"door_steel_t.png");
	f->setTexture(3,"door_steel_t.png");
	f->setTexture(4,"door_steel_t.png");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::set1To1Recipe(CONTENT_GLASS_DOOR_LT,CONTENT_GLASS_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_W_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png^[transformFX");
	f->setTexture(2,"door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png");
	f->setTexture(4,"door_wood_wb.png");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_door(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_W_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png^[transformFX");
	f->setTexture(2,"door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png");
	f->setTexture(4,"door_wood_wt.png");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorw(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1To1Recipe(CONTENT_WOOD_W_DOOR_LT,CONTENT_WOOD_W_DOOR_RT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_WOOD_DOOR_RT,CONTENT_WOOD_W_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_W_DOOR_RB;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png^[transformFX");
	f->setTexture(2,"door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png");
	f->setTexture(4,"door_steel_wb.png");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	content_mapnode_nodebox_door(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_W_DOOR_RT;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png^[transformFX");
	f->setTexture(2,"door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png");
	f->setTexture(4,"door_steel_wt.png");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_doorw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::set1To1Recipe(CONTENT_STEEL_W_DOOR_LT,CONTENT_STEEL_W_DOOR_RT);
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_STEEL_DOOR_RT,CONTENT_STEEL_W_DOOR_RT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// open doors
	i = CONTENT_WOOD_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_GLASS_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	i = CONTENT_GLASS_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_STEEL_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Door");
	f->setAllTextures("door_steel_t.png");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	i = CONTENT_WOOD_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_LT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorwol(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_STEEL_W_DOOR_LB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_W_DOOR_LT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_LT)+" 1";
	content_mapnode_nodebox_doorwol(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	// right open doors
	i = CONTENT_WOOD_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_b.png");
	f->setTexture(3,"door_wood_b.png^[transformFX");
	f->setTexture(4,"door_wood_b.png^[transformFX");
	f->setTexture(5,"door_wood_b.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Door");
	f->setAllTextures("door_wood_t.png");
	f->setTexture(3,"door_wood_t.png^[transformFX");
	f->setTexture(4,"door_wood_t.png^[transformFX");
	f->setTexture(5,"door_wood_t.png^[transformFX");
	f->setInventoryTexture("door_wood_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_GLASS_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_b.png");
	f->setTexture(2,"door_glass_b.png^[transformFX");
	f->setTexture(3,"door_glass_b.png^[transformFX");
	f->setTexture(4,"door_glass_b.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i+1)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;
	i = CONTENT_GLASS_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Glass Door");
	f->setAllTextures("door_glass_t.png");
	f->setTexture(2,"door_glass_t.png^[transformFX");
	f->setTexture(3,"door_glass_t.png^[transformFX");
	f->setTexture(4,"door_glass_t.png^[transformFX");
	f->setInventoryTexture("door_glass_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.15;

	i = CONTENT_STEEL_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_b.png^[transformFX");
	f->setTexture(4,"door_steel_b.png^[transformFX");
	f->setTexture(5,"door_steel_b.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Door");
	f->setAllTextures("door_steel_b.png");
	f->setTexture(3,"door_steel_t.png^[transformFX");
	f->setTexture(4,"door_steel_t.png^[transformFX");
	f->setTexture(5,"door_steel_t.png^[transformFX");
	f->setInventoryTexture("door_steel_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	i = CONTENT_WOOD_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wb.png");
	f->setTexture(3,"door_wood_wb.png^[transformFX");
	f->setTexture(4,"door_wood_wb.png^[transformFX");
	f->setTexture(5,"door_wood_wb.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_dooror(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	i = CONTENT_WOOD_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Wood Windowed Door");
	f->setAllTextures("door_wood_wt.png");
	f->setTexture(3,"door_wood_wt.png^[transformFX");
	f->setTexture(4,"door_wood_wt.png^[transformFX");
	f->setTexture(5,"door_wood_wt.png^[transformFX");
	f->setInventoryTexture("door_wood_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_DOOR_RT)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_doorwor(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_STEEL_W_DOOR_RB_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wb.png");
	f->setTexture(3,"door_steel_wb.png^[transformFX");
	f->setTexture(4,"door_steel_wb.png^[transformFX");
	f->setTexture(5,"door_steel_wb.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_RT)+" 1";
	content_mapnode_nodebox_dooror(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	i = CONTENT_STEEL_W_DOOR_RT_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Right Hanging Steel Windowed Door");
	f->setAllTextures("door_steel_wt.png");
	f->setTexture(3,"door_steel_wt.png^[transformFX");
	f->setTexture(4,"door_steel_wt.png^[transformFX");
	f->setTexture(5,"door_steel_wt.png^[transformFX");
	f->setInventoryTexture("door_steel_w_inv.png^[transformFX");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_DOOR_RT)+" 1";
	content_mapnode_nodebox_doorwor(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	// hatches
	i = CONTENT_WOOD_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_inv.png");
	f->wield_nodebox = false;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_hatch(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_WOOD_HATCH);
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_WOOD_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_HATCH;
	f = &content_features(i);
	f->param_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_inv.png");
	f->wield_nodebox = false;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatch(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_WOOD_W_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Windowed Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_hatchw(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_WOOD_HATCH,CONTENT_WOOD_W_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_W_HATCH;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Windowed Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_hatchw(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::set1over1Recipe(CONTENT_GLASS,CONTENT_STEEL_HATCH,CONTENT_STEEL_W_HATCH);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// gates
	i = CONTENT_WOOD_GATE;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Gate");
	f->setAllTextures("gate_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_wood_inv.png");
	f->wield_nodebox = false;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_gate(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;
	crafting::setGateRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_WOOD,CONTENT_WOOD_GATE);
	crafting::setGateRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_JUNGLEWOOD,CONTENT_WOOD_GATE);
	crafting::setGateRecipe(CONTENT_CRAFTITEM_WOOD_PLANK,CONTENT_JUNGLEWOOD,CONTENT_WOOD_GATE);
	crafting::setGateRecipe(CONTENT_CRAFTITEM_JUNGLE_PLANK,CONTENT_WOOD,CONTENT_WOOD_GATE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STEEL_GATE;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Gate");
	f->setAllTextures("gate_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_steel_inv.png");
	f->wield_nodebox = false;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_gate(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;
	crafting::setGateRecipe(CONTENT_CRAFTITEM_STEEL_INGOT,CONTENT_STEEL,CONTENT_STEEL_GATE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	// open hatches
	i = CONTENT_WOOD_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood.png");
	f->setTexture(2,"hatch_wood.png^[transformR90");
	f->setTexture(3,"hatch_wood.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_HATCH)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_hatcho(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_STEEL_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel.png");
	f->setTexture(2,"hatch_steel.png^[transformR90");
	f->setTexture(3,"hatch_steel.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_HATCH)+" 1";
	content_mapnode_nodebox_hatcho(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	i = CONTENT_WOOD_W_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Hatch");
	f->setAllTextures("hatch_wood_w.png");
	f->setTexture(2,"hatch_wood.png^[transformR90");
	f->setTexture(3,"hatch_wood.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_wood_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_W_HATCH)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_hatchwo(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_STEEL_W_HATCH_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Hatch");
	f->setAllTextures("hatch_steel_w.png");
	f->setTexture(2,"hatch_steel.png^[transformR90");
	f->setTexture(3,"hatch_steel.png^[transformR90");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("hatch_steel_w_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->climbable = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_W_HATCH)+" 1";
	content_mapnode_nodebox_hatchwo(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	// open gates
	i = CONTENT_WOOD_GATE_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Wood Gate");
	f->setAllTextures("gate_wood.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_wood_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WOOD_GATE)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30;
	content_mapnode_nodebox_gateo(f);
	f->type = CMT_WOOD;
	f->hardness = 0.75;

	i = CONTENT_STEEL_GATE_OPEN;
	f = &content_features(i);
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->description = std::string("Steel Gate");
	f->setAllTextures("gate_steel.png");
	f->rotate_tile_with_nodebox = true;
	f->setInventoryTexture("gate_steel_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->draw_type = CDT_NODEBOX;
	f->solidness = 0; // drawn separately, makes no faces
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_STEEL_GATE)+" 1";
	content_mapnode_nodebox_gateo(f);
	if(f->initial_metadata == NULL)
		f->initial_metadata = new LockedDoorNodeMetadata();
	f->type = CMT_STONE;
	f->hardness = 5.0;

	// plants
	i = CONTENT_WILDGRASS_SHORT;
	f = &content_features(i);
	f->description = std::string("Wild Grass");
	f->setInventoryTexture("wildgrass_short.png");
	f->setAllTextures("wildgrass_short.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	//f->pointable = false;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;

	i = CONTENT_WILDGRASS_LONG;
	f = &content_features(i);
	f->description = std::string("Wild Grass");
	f->setInventoryTexture("wildgrass_long.png");
	f->setAllTextures("wildgrass_long.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->air_equivalent = false; // grass grows underneath
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_WILDGRASS_SHORT)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;

	i = CONTENT_DEADGRASS;
	f = &content_features(i);
	f->description = std::string("Dead Grass");
	f->setInventoryTexture("deadgrass.png");
	f->setAllTextures("deadgrass.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;

	i = CONTENT_FLOWER_STEM;
	f = &content_features(i);
	f->description = std::string("Flower Stem");
	f->setInventoryTexture("flower_stem.png");
	f->setAllTextures("flower_stem.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;

	i = CONTENT_FLOWER_ROSE;
	f = &content_features(i);
	f->description = std::string("Rose");
	f->setInventoryTexture("flower_rose.png");
	f->setAllTextures("flower_rose.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);

	i = CONTENT_FLOWER_DAFFODIL;
	f = &content_features(i);
	f->description = std::string("Daffodil");
	f->setInventoryTexture("flower_daffodil.png");
	f->setAllTextures("flower_daffodil.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);

	i = CONTENT_FLOWER_TULIP;
	f = &content_features(i);
	f->description = std::string("Tulip");
	f->setInventoryTexture("flower_tulip.png");
	f->setAllTextures("flower_tulip.png");
	f->setAllTextureFlags(0);
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_PLANTLIKE;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	f->material_pointable = false;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->buildable_to = true;
	f->type = CMT_WOOD;
	f->hardness = 0.10;
	lists::add("creative",i);

	i = CONTENT_FLOWER_POT_RAW;
	f = &content_features(i);
	f->param_type = CPT_LIGHT;
	f->description = std::string("Unbaked Flower Pot");
	f->setAllTextures("flower_pot_raw.png");
	f->setTexture(0,"flower_pot_raw_top.png");
	f->setTexture(1,"flower_pot_raw_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->cook_result = std::string("MaterialItem2 ")+itos(CONTENT_FLOWER_POT)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.75;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS,
		0.2*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS,
		0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.3*BS,
		-0.3*BS,
		0.5*BS,
		0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS,
		-0.5*BS,
		-0.3*BS,
		0.3*BS,
		0.35*BS,
		0.3*BS
	));
	f->setInventoryTextureNodeBox(i,"flower_pot_raw_top.png","flower_pot_raw.png","flower_pot_raw.png");
	crafting::setVRecipe(CONTENT_CRAFTITEM_CLAY,CONTENT_FLOWER_POT_RAW);
	lists::add("craftguide",i);
	lists::add("cooking",i);

	i = CONTENT_FLOWER_POT;
	f = &content_features(i);
	f->param_type = CPT_LIGHT;
	f->description = std::string("Flower Pot");
	f->setAllTextures("flower_pot.png");
	f->setTexture(0,"flower_pot_top.png");
	f->setTexture(1,"flower_pot_bottom.png");
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->type = CMT_STONE;
	f->hardness = 0.75;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		0.3*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS,
		0.2*BS,
		-0.3*BS,
		0.5*BS,
		0.5*BS,
		0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,
		0.2*BS,
		-0.3*BS,
		-0.3*BS,
		0.5*BS,
		0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS,
		-0.5*BS,
		-0.3*BS,
		0.3*BS,
		0.35*BS,
		0.3*BS
	));
	f->setInventoryTextureNodeBox(i,"flower_pot_top.png","flower_pot.png","flower_pot.png");
	lists::add("creative",i);

	// walls
	i = CONTENT_COBBLE_WALL;
	f = &content_features(i);
	f->description = std::string("Cobblestone Wall");
	f->setAllTextures("cobble.png");
	f->light_propagates = true;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("cobble_wall_inv.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setWallRecipe(CONTENT_COBBLE,CONTENT_COBBLE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_ROUGHSTONE_WALL;
	f = &content_features(i);
	f->description = std::string("Rough Stone Wall");
	f->setAllTextures("roughstone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("roughstone_wall_inv.png");
	f->type = CMT_STONE;
	f->hardness = 0.9;
	crafting::setWallRecipe(CONTENT_ROUGHSTONE,CONTENT_ROUGHSTONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_MOSSYCOBBLE_WALL;
	f = &content_features(i);
	f->description = std::string("Mossy Cobblestone Wall");
	f->setAllTextures("mossycobble.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("mossycobble_wall_inv.png");
	f->type = CMT_STONE;
	f->hardness = 0.8;
	crafting::setWallRecipe(CONTENT_MOSSYCOBBLE,CONTENT_MOSSYCOBBLE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_STONE_WALL;
	f = &content_features(i);
	f->description = std::string("Stone Wall");
	f->setAllTextures("stone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("stone_wall_inv.png");
	f->type = CMT_STONE;
	f->hardness = 1.0;
	crafting::setWallRecipe(CONTENT_STONE,CONTENT_STONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_SANDSTONE_WALL;
	f = &content_features(i);
	f->description = std::string("Sand Stone Wall");
	f->setAllTextures("sandstone.png");
	f->light_propagates = true;
	f->jumpable = false;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_WALLLIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->solidness = 0; // drawn separately, makes no faces
	f->air_equivalent = true; // grass grows underneath
	f->setInventoryTexture("sandstone_wall_inv.png");
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setWallRecipe(CONTENT_SANDSTONE,CONTENT_SANDSTONE_WALL);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TNT;
	f = &content_features(i);
	f->description = std::string("TNT");
	f->setAllTextures("tnt.png");
	f->setTexture(0, "tnt_top.png");
	f->setTexture(1, "tnt_bottom.png");
	f->setInventoryTextureCube("tnt_top.png", "tnt.png", "tnt.png");
	f->draw_type = CDT_CUBELIKE;
	f->is_ground_content = true;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	if(f->initial_metadata == NULL)
		f->initial_metadata = new TNTNodeMetadata();
	f->type = CMT_DIRT;
	f->hardness = 1.0;
	crafting::setSoftBlockRecipe(CONTENT_CRAFTITEM_TNT,CONTENT_TNT);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_FLASH;
	f = &content_features(i);
	f->description = std::string("In-Progress explosion - how did you get this???");
	f->setAllTextures("flash.png");
	f->solidness = 0; // drawn separately, makes no faces
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->light_source = LIGHT_MAX-1;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 50;
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
	f->post_effect_color = video::SColor(192, 255, 255, 64);
#endif

	i = CONTENT_STEAM;
	f = &content_features(i);
	f->description = std::string("Steam");
	f->setAllTextures("steam.png");
	f->solidness = 0;
	f->param_type = CPT_LIGHT;
	f->draw_type = CDT_GLASSLIKE;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->walkable = false;
	f->pointable = false;
	f->diggable = false;
	f->buildable_to = true;
	f->damage_per_second = 4;
#ifndef SERVER
	f->setAllTextureTypes(MATERIAL_ALPHA_BLEND);
	f->post_effect_color = video::SColor(120, 200, 200, 200);
#endif

	i = CONTENT_CHAIR;
	f = &content_features(i);
	f->description = std::string("Chair");
	f->setAllTextures("pine.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->rotate_tile_with_nodebox = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.5*BS, 0.20*BS, -0.2*BS, 0.5*BS, 0.30*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.2*BS, -0.5*BS, 0.20*BS, 0.3*BS, 0.5*BS, 0.30*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.5*BS, -0.30*BS, -0.2*BS, -0.1*BS, -0.20*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.2*BS, -0.5*BS, -0.30*BS, 0.3*BS, -0.1*BS, -0.20*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.1*BS, -0.30*BS, 0.3*BS, 0.0*BS, 0.20*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.2*BS, 0.1*BS, 0.25*BS, 0.2*BS, 0.4*BS, 0.26*BS
	));
	f->setInventoryTextureNodeBox(i,"pine.png", "pine.png", "pine.png");
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,			CONTENT_IGNORE,
			CONTENT_WOOD_PINE,		CONTENT_WOOD_PINE,		CONTENT_IGNORE,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE
		};
		crafting::setRecipe(r,CONTENT_CHAIR,2);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_TABLE;
	f = &content_features(i);
	f->description = std::string("Table");
	f->setAllTextures("pine.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/16;
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4*BS, -0.5*BS, -0.4*BS, -0.3*BS, 0.4*BS, -0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS, -0.5*BS, -0.4*BS, 0.4*BS, 0.4*BS, -0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS, -0.5*BS, 0.3*BS, -0.3*BS, 0.4*BS, 0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS, -0.5*BS, 0.3*BS, 0.4*BS, 0.4*BS, 0.4*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS, 0.4*BS, -0.5*BS, 0.5*BS, 0.5*BS, 0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4*BS, -0.2*BS, -0.3*BS, -0.3*BS, -0.1*BS, 0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3*BS, -0.2*BS, -0.4*BS, 0.4*BS, -0.1*BS, 0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.2*BS, -0.4*BS, 0.4*BS, -0.1*BS, -0.3*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3*BS, -0.2*BS, 0.3*BS, 0.3*BS, -0.1*BS, 0.4*BS
	));
	f->setInventoryTextureNodeBox(i,"pine.png", "pine.png", "pine.png");
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	{
		u16 r[9] = {
			CONTENT_WOOD_PINE,		CONTENT_WOOD_PINE,		CONTENT_WOOD_PINE,
			CONTENT_CRAFTITEM_PINE_PLANK,	CONTENT_IGNORE,			CONTENT_CRAFTITEM_PINE_PLANK,
			CONTENT_IGNORE,			CONTENT_IGNORE,			CONTENT_IGNORE,
		};
		crafting::setRecipe(r,CONTENT_TABLE,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_HEAD;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"cotton.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton.png^bed_side_head.png");
	f->setTexture(3,"cotton.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton.png^bed_end.png");
	f->setTexture(5,"cotton.png^bed_end.png");
	f->setInventoryTexture("bed_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON,CONTENT_BED_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_FOOT;
	f = &content_features(i);
	f->description = std::string("Bed");
	f->setTexture(0,"cotton.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton.png^bed_side.png");
	f->setTexture(4,"cotton.png^bed_end.png");
	f->setTexture(5,"cotton.png^bed_end.png");
	f->setInventoryTexture("bed_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_BLUE_HEAD;
	f = &content_features(i);
	f->description = std::string("Blue Bed");
	f->setTexture(0,"cotton_blue.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_blue.png^bed_side_head.png");
	f->setTexture(3,"cotton_blue.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_blue.png^bed_end.png");
	f->setTexture(5,"cotton_blue.png^bed_end.png");
	f->setInventoryTexture("bed_blue_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_BLUE,CONTENT_BED_BLUE_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_BLUE_FOOT;
	f = &content_features(i);
	f->description = std::string("Blue Bed");
	f->setTexture(0,"cotton_blue.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_blue.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_blue.png^bed_side.png");
	f->setTexture(4,"cotton_blue.png^bed_end.png");
	f->setTexture(5,"cotton_blue.png^bed_end.png");
	f->setInventoryTexture("bed_blue_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_BLUE_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_GREEN_HEAD;
	f = &content_features(i);
	f->description = std::string("Green Bed");
	f->setTexture(0,"cotton_green.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_green.png^bed_side_head.png");
	f->setTexture(3,"cotton_green.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_green.png^bed_end.png");
	f->setTexture(5,"cotton_green.png^bed_end.png");
	f->setInventoryTexture("bed_green_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_GREEN,CONTENT_BED_GREEN_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_GREEN_FOOT;
	f = &content_features(i);
	f->description = std::string("Green Bed");
	f->setTexture(0,"cotton_green.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_green.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_green.png^bed_side.png");
	f->setTexture(4,"cotton_green.png^bed_end.png");
	f->setTexture(5,"cotton_green.png^bed_end.png");
	f->setInventoryTexture("bed_green_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_GREEN_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_ORANGE_HEAD;
	f = &content_features(i);
	f->description = std::string("Orange Bed");
	f->setTexture(0,"cotton_orange.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_orange.png^bed_side_head.png");
	f->setTexture(3,"cotton_orange.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_orange.png^bed_end.png");
	f->setTexture(5,"cotton_orange.png^bed_end.png");
	f->setInventoryTexture("bed_orange_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_ORANGE,CONTENT_BED_ORANGE_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_ORANGE_FOOT;
	f = &content_features(i);
	f->description = std::string("Orange Bed");
	f->setTexture(0,"cotton_orange.png^bed_top.png");
	f->setTexture(1,"cotton_orange.png^bed_bottom.png");
	f->setTexture(2,"cotton_orange.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_orange.png^bed_side.png");
	f->setTexture(4,"cotton_orange.png^bed_end.png");
	f->setTexture(5,"cotton_orange.png^bed_end.png");
	f->setInventoryTexture("bed_orange_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_ORANGE_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_PURPLE_HEAD;
	f = &content_features(i);
	f->description = std::string("Purple Bed");
	f->setTexture(0,"cotton_purple.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_purple.png^bed_side_head.png");
	f->setTexture(3,"cotton_purple.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_purple.png^bed_end.png");
	f->setTexture(5,"cotton_purple.png^bed_end.png");
	f->setInventoryTexture("bed_purple_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_PURPLE,CONTENT_BED_PURPLE_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_PURPLE_FOOT;
	f = &content_features(i);
	f->description = std::string("Purple Bed");
	f->setTexture(0,"cotton_purple.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_purple.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_purple.png^bed_side.png");
	f->setTexture(4,"cotton_purple.png^bed_end.png");
	f->setTexture(5,"cotton_purple.png^bed_end.png");
	f->setInventoryTexture("bed_purple_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_PURPLE_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_RED_HEAD;
	f = &content_features(i);
	f->description = std::string("Red Bed");
	f->setTexture(0,"cotton_red.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_red.png^bed_side_head.png");
	f->setTexture(3,"cotton_red.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_red.png^bed_end.png");
	f->setTexture(5,"cotton_red.png^bed_end.png");
	f->setInventoryTexture("bed_red_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_RED,CONTENT_BED_RED_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_RED_FOOT;
	f = &content_features(i);
	f->description = std::string("Red Bed");
	f->setTexture(0,"cotton_red.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_red.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_red.png^bed_side.png");
	f->setTexture(4,"cotton_red.png^bed_end.png");
	f->setTexture(5,"cotton_red.png^bed_end.png");
	f->setInventoryTexture("bed_red_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_RED_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_YELLOW_HEAD;
	f = &content_features(i);
	f->description = std::string("Yellow Bed");
	f->setTexture(0,"cotton_yellow.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_yellow.png^bed_side_head.png");
	f->setTexture(3,"cotton_yellow.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_yellow.png^bed_end.png");
	f->setTexture(5,"cotton_yellow.png^bed_end.png");
	f->setInventoryTexture("bed_yellow_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_YELLOW,CONTENT_BED_YELLOW_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_YELLOW_FOOT;
	f = &content_features(i);
	f->description = std::string("Yellow Bed");
	f->setTexture(0,"cotton_yellow.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_yellow.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_yellow.png^bed_side.png");
	f->setTexture(4,"cotton_yellow.png^bed_end.png");
	f->setTexture(5,"cotton_yellow.png^bed_end.png");
	f->setInventoryTexture("bed_yellow_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_YELLOW_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	i = CONTENT_BED_BLACK_HEAD;
	f = &content_features(i);
	f->description = std::string("Black Bed");
	f->setTexture(0,"cotton_black.png^bed_head.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_black.png^bed_side_head.png");
	f->setTexture(3,"cotton_black.png^bed_side_head.png^[transformFX");
	f->setTexture(4,"cotton_black.png^bed_end.png");
	f->setTexture(5,"cotton_black.png^bed_end.png");
	f->setInventoryTexture("bed_black_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(i)+" 1";
	content_mapnode_nodebox_bed_head(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;
	crafting::setBedRecipe(CONTENT_COTTON_BLACK,CONTENT_BED_BLACK_HEAD);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_BED_BLACK_FOOT;
	f = &content_features(i);
	f->description = std::string("Black Bed");
	f->setTexture(0,"cotton_black.png^bed_top.png");
	f->setTexture(1,"bed_bottom.png");
	f->setTexture(2,"cotton_black.png^bed_side.png^[transformFX");
	f->setTexture(3,"cotton_black.png^bed_side.png");
	f->setTexture(4,"cotton_black.png^bed_end.png");
	f->setTexture(5,"cotton_black.png^bed_end.png");
	f->setInventoryTexture("bed_black_inv.png");
	f->wield_nodebox = false;
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->rotate_tile_with_nodebox = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 30/32;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_BED_BLACK_HEAD)+" 1";
	content_mapnode_nodebox_bed_foot(f);
	f->type = CMT_WOOD;
	f->hardness = 0.25;

	// Paintings
	i = CONTENT_PAINTING_WHITE;
	f = &content_features(i);
	f->description = std::string("Colorful Painting");
	f->setAllTextures("painting.png");
	f->setTexture(4, "painting.png");
	f->setTexture(5, "painting_white.png");
	f->setInventoryTexture("painting_white.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	//f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_PAINTING_WHITE)+" 1";
	f->type = CMT_WOOD;
	content_mapnode_nodebox_painting(f);
	f->hardness = 0.1;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,
			CONTENT_CRAFTITEM_STRING, CONTENT_CRAFTITEM_DYE_WHITE, CONTENT_CRAFTITEM_STRING,
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_PAINTING_WHITE,1);
	}
	crafting::set1Any2Recipe(CONTENT_PAINTING_CANVAS,CONTENT_CRAFTITEM_DYE_WHITE,CONTENT_PAINTING_WHITE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_PAINTING_RED;
	f = &content_features(i);
	f->description = std::string("Red Rose Painting");
	f->setAllTextures("painting.png");
	f->setTexture(4, "painting.png");
	f->setTexture(5, "painting_red.png");
	f->setInventoryTexture("painting_red.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	//f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_PAINTING_RED)+" 1";
	f->type = CMT_WOOD;
	content_mapnode_nodebox_painting(f);
	f->hardness = 0.1;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,
			CONTENT_CRAFTITEM_STRING, CONTENT_CRAFTITEM_DYE_RED, CONTENT_CRAFTITEM_STRING,
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_PAINTING_RED,1);
	}
	crafting::set1Any2Recipe(CONTENT_PAINTING_CANVAS,CONTENT_CRAFTITEM_DYE_RED,CONTENT_PAINTING_RED);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_PAINTING_GREEN;
	f = &content_features(i);
	f->description = std::string("Field Painting");
	f->setAllTextures("painting.png");
	f->setTexture(4, "painting.png");
	f->setTexture(5, "painting_green.png");
	f->setInventoryTexture("painting_green.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	//f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_PAINTING_GREEN)+" 1";
	f->type = CMT_WOOD;
	content_mapnode_nodebox_painting(f);
	f->hardness = 0.1;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,
			CONTENT_CRAFTITEM_STRING, CONTENT_CRAFTITEM_DYE_GREEN, CONTENT_CRAFTITEM_STRING,
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_PAINTING_GREEN,1);
	}
	crafting::set1Any2Recipe(CONTENT_PAINTING_CANVAS,CONTENT_CRAFTITEM_DYE_GREEN,CONTENT_PAINTING_GREEN);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_PAINTING_BLUE;
	f = &content_features(i);
	f->description = std::string("Blue Flower Painting");
	f->setAllTextures("painting.png");
	f->setTexture(4, "painting.png");
	f->setTexture(5, "painting_blue.png");
	f->setInventoryTexture("painting_blue.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	//f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_PAINTING_BLUE)+" 1";
	f->type = CMT_WOOD;
	content_mapnode_nodebox_painting(f);
	f->hardness = 0.1;
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,
			CONTENT_CRAFTITEM_STRING, CONTENT_CRAFTITEM_DYE_BLUE, CONTENT_CRAFTITEM_STRING,
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_PAINTING_BLUE,1);
	}
	crafting::set1Any2Recipe(CONTENT_PAINTING_CANVAS,CONTENT_CRAFTITEM_DYE_BLUE,CONTENT_PAINTING_BLUE);
	lists::add("craftguide",i);
	lists::add("creative",i);

	i = CONTENT_PAINTING_CANVAS;
	f = &content_features(i);
	f->description = std::string("Painting Canvas");
	f->setAllTextures("painting.png");
	f->setTexture(4, "painting.png");
	f->setTexture(5, "painting_canvas.png");
	f->setInventoryTexture("painting_canvas.png");
	f->param_type = CPT_LIGHT;
	f->param2_type = CPT_FACEDIR_SIMPLE;
	f->draw_type = CDT_NODEBOX;
	f->light_propagates = true;
	f->sunlight_propagates = true;
	f->solidness = 0; // drawn separately, makes no faces
	f->walkable = false;
	//f->wall_mounted = true;
	f->air_equivalent = true;
	f->flammable = 1; // can be replaced by fire if the node under it is set on fire
	f->fuel_time = 1;
	f->dug_item = std::string("MaterialItem2 ")+itos(CONTENT_PAINTING_CANVAS)+" 1";
	f->type = CMT_WOOD;
	content_mapnode_nodebox_painting(f);
	f->hardness = 0.1;
	crafting::set1Any2Recipe(CONTENT_PAINTING_WHITE,CONTENT_CRAFTITEM_STARCH,CONTENT_PAINTING_CANVAS);
	crafting::set1Any2Recipe(CONTENT_PAINTING_RED,CONTENT_CRAFTITEM_STARCH,CONTENT_PAINTING_CANVAS);
	crafting::set1Any2Recipe(CONTENT_PAINTING_GREEN,CONTENT_CRAFTITEM_STARCH,CONTENT_PAINTING_CANVAS);
	crafting::set1Any2Recipe(CONTENT_PAINTING_BLUE,CONTENT_CRAFTITEM_STARCH,CONTENT_PAINTING_CANVAS);
	{
		u16 r[9] = {
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,
			CONTENT_CRAFTITEM_STRING, CONTENT_IGNORE, CONTENT_CRAFTITEM_STRING,
			CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK,	CONTENT_CRAFTITEM_STICK
		};
		crafting::setRecipe(r,CONTENT_PAINTING_CANVAS,1);
	}
	lists::add("craftguide",i);
	lists::add("creative",i);
}
