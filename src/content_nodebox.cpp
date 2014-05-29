/************************************************************************
* Minetest-c55
* Copyright (C) 2010 celeron55, Perttu Ahola <celeron55@gmail.com>
*
* content_nodebox.cpp
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
************************************************************************/

#include "content_mapnode.h"
#include "content_nodebox.h"

void content_nodebox_stair(ContentFeatures *f)
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

void content_nodebox_stairud(ContentFeatures *f)
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

void content_nodebox_slab(ContentFeatures *f)
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

void content_nodebox_slabud(ContentFeatures *f)
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

void content_nodebox_door(ContentFeatures *f)
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

void content_nodebox_doorw(ContentFeatures *f)
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

void content_nodebox_doorol(ContentFeatures *f)
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

void content_nodebox_doorwol(ContentFeatures *f)
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

void content_nodebox_dooror(ContentFeatures *f)
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

void content_nodebox_doorwor(ContentFeatures *f)
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

void content_nodebox_hatch(ContentFeatures *f)
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

void content_nodebox_hatchw(ContentFeatures *f)
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

void content_nodebox_hatcho(ContentFeatures *f)
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

void content_nodebox_hatchwo(ContentFeatures *f)
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

void content_nodebox_gate(ContentFeatures *f)
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

void content_nodebox_gateo(ContentFeatures *f)
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

void content_nodebox_carpet(ContentFeatures *f)
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

void content_nodebox_bed_head(ContentFeatures *f)
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

void content_nodebox_bed_foot(ContentFeatures *f)
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

void content_nodebox_plant_1(ContentFeatures *f)
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

void content_nodebox_plant_2(ContentFeatures *f)
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

void content_nodebox_plant_3(ContentFeatures *f)
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

void content_nodebox_guide(ContentFeatures *f)
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
void content_nodebox_painting(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.375*BS,
		0.4375*BS,
		0.4375*BS,
		0.375*BS,
		0.5*BS
	));
}

void content_nodebox_book(ContentFeatures *f)
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

void content_nodebox_pie(ContentFeatures *f)
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

void content_nodebox_pie_3(ContentFeatures *f)
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

void content_nodebox_pie_2(ContentFeatures *f)
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

void content_nodebox_pie_1(ContentFeatures *f)
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

void content_nodebox_sign(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.0625*BS,
		-0.5*BS,
		-0.0625*BS,
		0.0625*BS,
		0.5*BS,
		0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.0625*BS,
		-0.125*BS,
		0.4375*BS,
		0.4375*BS,
		-0.0625*BS
	));
}

void content_nodebox_sign_ud(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.0625*BS,
		-0.5*BS,
		-0.0625*BS,
		0.0625*BS,
		0.5*BS,
		0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.4375*BS,
		-0.125*BS,
		0.4375*BS,
		0.0625*BS,
		-0.0625*BS
	));
}

void content_nodebox_sign_wall(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,
		-0.25*BS,
		0.4375*BS,
		0.4375*BS,
		0.25*BS,
		0.5*BS
	));
}

void content_nodebox_jackolantern(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.5*BS,-0.5*BS,-0.4375*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,-0.5*BS,-0.5*BS,0.5*BS,-0.375*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,0.25*BS,-0.5*BS,0.5*BS,0.5*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.375*BS,-0.375*BS,-0.5*BS,0.5*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,-0.375*BS,-0.5*BS,-0.375*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.1875*BS,0.1875*BS,-0.5*BS,0.125*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.25*BS,-0.0625*BS,-0.5*BS,0.375*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,-0.0625*BS,-0.5*BS,-0.3125*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.125*BS,0.0625*BS,-0.5*BS,0.0625*BS,0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.25*BS,-0.1875*BS,-0.5*BS,0.25*BS,-0.125*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,-0.125*BS,-0.5*BS,0.3125*BS,-0.0625*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.375*BS,-0.375*BS,-0.5*BS,-0.3125*BS,-0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.0625*BS,-0.375*BS,-0.5*BS,0.375*BS,-0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.375*BS,-0.5*BS,-0.0625*BS,-0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.3125*BS,-0.5*BS,-0.1875*BS,-0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.1875*BS,-0.3125*BS,-0.5*BS,0.375*BS,-0.25*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.3125*BS,-0.25*BS,-0.5*BS,0.375*BS,-0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.0625*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.0625*BS,-0.0625*BS,-0.5*BS,0.25*BS,0.125*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.1875*BS,0.125*BS,-0.5*BS,0.25*BS,0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,0.125*BS,-0.5*BS,-0.25*BS,0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.125*BS,0.*BS,-0.5*BS,-0.0625*BS,0.0625*BS,-0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		0.*BS,0.*BS,-0.5*BS,0.0625*BS,0.0625*BS,-0.4375*BS
	));
}

void content_nodebox_roofcollide(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.4375*BS,0.4375*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.1875*BS,-0.3125*BS,-0.1875*BS,0.1875*BS,0.0625*BS,0.1875*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.3125*BS,-0.4375*BS,-0.3125*BS,0.3125*BS,-0.3125*BS,0.3125*BS
	));
}

void content_nodebox_bookshelf(ContentFeatures *f)
{
	f->setNodeBox(core::aabbox3d<f32>(
		0.4375*BS,-0.5*BS,-0.5*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.5*BS,-0.5*BS,-0.5*BS,-0.4375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,-0.4375*BS,-0.0625*BS,0.4375*BS,0.4375*BS,0.0625*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,0.4375*BS,-0.5*BS,0.4375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,-0.5*BS,-0.5*BS,0.4375*BS,-0.4375*BS,0.5*BS
	));
	f->addNodeBox(core::aabbox3d<f32>(
		-0.4375*BS,-0.0625*BS,-0.5*BS,0.4375*BS,0.0625*BS,0.5*BS
	));
}
