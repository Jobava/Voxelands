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
	f->setNodeBox(NodeBox(
		-0.5*BS,
		-0.5*BS,
		-0.25*BS,
		0.5*BS,
		0.,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		0.,
		0.,
		0.5*BS,
		0.25*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		0.25*BS,
		0.25*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.25*BS,
		-0.25*BS
	));
}

void content_nodebox_stairud(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,0.,-0.25*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.25*BS,-0.5*BS,0.5*BS,0.5*BS,-0.25*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.25*BS,0.000000,0.5*BS,0.000000,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,0.25*BS,0.5*BS,-0.25*BS,0.5*BS
	));
}

void content_nodebox_slab(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		-0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		-0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,
		-0.3125*BS,
		-0.4375*BS,
		0.0625*BS,
		0.3125*BS,
		-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,
		-0.0625*BS,
		-0.4375*BS,
		-0.0625*BS,
		0.0625*BS,
		-0.375*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.3125*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		-0.3125*BS,
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		-0.3125*BS,
		0.3125*BS,
		-0.3125*BS,
		0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,
		-0.3125*BS,
		-0.0625*BS,
		-0.375*BS,
		0.3125*BS,
		0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,
		-0.0625*BS,
		-0.3125*BS,
		-0.375*BS,
		0.0625*BS,
		-0.0625*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		0.3125*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		-0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		-0.3125*BS,
		0.3125*BS,
		0.5*BS,
		0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,
		-0.3125*BS,
		-0.0625*BS,
		0.4375*BS,
		0.3125*BS,
		0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,
		-0.0625*BS,
		-0.3125*BS,
		0.4375*BS,
		0.0625*BS,
		-0.0625*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.5*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		0.3125*BS,
		0.3125*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		0.3125*BS,
		-0.3125*BS,
		0.5*BS,
		0.5*BS,
		0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		0.3125*BS,
		-0.3125*BS,
		-0.3125*BS,
		0.5*BS,
		0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,
		0.375*BS,
		-0.3125*BS,
		0.0625*BS,
		0.4375*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,
		0.375*BS,
		-0.0625*BS,
		-0.0625*BS,
		0.4375*BS,
		0.0625*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		0.3125*BS,
		0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		-0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		-0.3125*BS,
		-0.5*BS,
		0.5*BS,
		0.3125*BS,
		-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,
		-0.3125*BS,
		0.3125*BS,
		0.5*BS,
		0.3125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,
		-0.3125*BS,
		-0.0625*BS,
		0.4375*BS,
		0.3125*BS,
		0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,
		-0.0625*BS,
		-0.3125*BS,
		0.4375*BS,
		0.0625*BS,
		-0.0625*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		0.4375*BS,-0.5*BS,-0.13*BS,0.5625*BS,0.625*BS,0.13*BS
	));
	f->addNodeBox(NodeBox(
		-0.5625*BS,-0.5*BS,-0.13*BS,-0.4375*BS,0.625*BS,0.13*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.25*BS,-0.0625*BS,0.0625*BS,0.625*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,0.375*BS,-0.0625*BS,-0.0625*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.125*BS,-0.0625*BS,-0.0625*BS,0.*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.0625*BS,0.375*BS,-0.0625*BS,0.4375*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.0625*BS,-0.125*BS,-0.0625*BS,0.4375*BS,0.*BS,0.0625*BS
	));
}

void content_nodebox_gateo(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		0.4375*BS,-0.5*BS,-0.13*BS,0.5625*BS,0.625*BS,0.13*BS
	));
	f->addNodeBox(NodeBox(
		-0.5625*BS,-0.5*BS,-0.13*BS,-0.4375*BS,0.625*BS,0.13*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.25*BS,0.4375*BS,-0.375*BS,0.625*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.375*BS,-0.0625*BS,-0.375*BS,0.5*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.125*BS,-0.0625*BS,-0.375*BS,0.*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,0.375*BS,-0.0625*BS,0.5*BS,0.5*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,-0.125*BS,-0.0625*BS,0.5*BS,0.*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,-0.25*BS,0.4375*BS,0.5*BS,0.625*BS,0.5*BS
	));
}

void content_nodebox_carpet(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.4375*BS,
		-0.125*BS,
		-0.5*BS,
		0.4375*BS,
		0.125*BS,
		0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,
		0.125*BS,
		0.*BS,
		0.375*BS,
		0.1875*BS,
		0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		-0.5*BS,
		0.375*BS,
		-0.375*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,
		-0.5*BS,
		0.375*BS,
		0.5*BS,
		0.5*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,
		-0.125*BS,
		0.4375*BS,
		0.375*BS,
		0.*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,
		0.1925*BS,
		0.4375*BS,
		0.375*BS,
		0.375*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.4375*BS,
		-0.125*BS,
		-0.5*BS,
		0.5*BS,
		0.*BS,
		0.375*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.4375*BS,
		-0.125*BS,
		-0.4375*BS,
		0.4375*BS,
		0.125*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,
		-0.5*BS,
		-0.5*BS,
		-0.375*BS,
		0.5*BS,
		-0.375*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,
		-0.5*BS,
		-0.5*BS,
		0.5*BS,
		0.5*BS,
		-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,
		-0.125*BS,
		-0.5*BS,
		0.375*BS,
		0.*BS,
		-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,
		0.1925*BS,
		-0.5*BS,
		0.375*BS,
		0.25*BS,
		-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.4375*BS,
		-0.125*BS,
		-0.375*BS,
		0.5*BS,
		0.*BS,
		0.5*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.4375*BS,
		-0.5*BS,
		-0.375*BS,

		0.4375*BS,
		-0.4375*BS,
		0.1875*BS
	));

	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		v3s16(0,0,1),v3f(0,0,0),
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
	f->setNodeBox(NodeBox(
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
	f->addNodeBox(NodeBox(
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
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	(4.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	0,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(NodeBox(
	(-5.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(-5.0 / 16.0)*BS,

	0,
	(-5.0 / 16.0)*BS,
	(5.0 / 16.0)*BS
	));

	f->addNodeBox(NodeBox(
	0,
	-0.5*BS,
	0,

	(4.0 / 16.0)*BS,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	0,
	(-6.0 / 16.0)*BS,
	(4.0 / 16.0)*BS
	));

	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
	(-4.0 / 16.0)*BS,
	-0.5*BS,
	(-4.0 / 16.0)*BS,

	0,
	(-6.0 / 16.0)*BS,
	0
	));

	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.0625*BS,
		-0.5*BS,
		-0.0625*BS,
		0.0625*BS,
		0.5*BS,
		0.0625*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.0625*BS,
		-0.5*BS,
		-0.0625*BS,
		0.0625*BS,
		0.5*BS,
		0.0625*BS
	));
	f->addNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
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
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.4375*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.5*BS,0.5*BS,-0.375*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.25*BS,-0.5*BS,0.5*BS,0.5*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,-0.375*BS,-0.5*BS,0.5*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.375*BS,-0.5*BS,-0.375*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,0.1875*BS,-0.5*BS,0.125*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.25*BS,-0.0625*BS,-0.5*BS,0.375*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.0625*BS,-0.5*BS,-0.3125*BS,0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,0.0625*BS,-0.5*BS,0.0625*BS,0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.25*BS,-0.1875*BS,-0.5*BS,0.25*BS,-0.125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.125*BS,-0.5*BS,0.3125*BS,-0.0625*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.375*BS,-0.5*BS,-0.3125*BS,-0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.0625*BS,-0.375*BS,-0.5*BS,0.375*BS,-0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.375*BS,-0.5*BS,-0.0625*BS,-0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.3125*BS,-0.5*BS,-0.1875*BS,-0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,-0.3125*BS,-0.5*BS,0.375*BS,-0.25*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.25*BS,-0.5*BS,0.375*BS,-0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.0625*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.0625*BS,-0.0625*BS,-0.5*BS,0.25*BS,0.125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.125*BS,-0.5*BS,0.25*BS,0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,0.125*BS,-0.5*BS,-0.25*BS,0.1875*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,0.*BS,-0.5*BS,-0.0625*BS,0.0625*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.*BS,0.*BS,-0.5*BS,0.0625*BS,0.0625*BS,-0.4375*BS
	));
}

void content_nodebox_roofcollide(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.4375*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.4375*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,-0.3125*BS,-0.1875*BS,0.1875*BS,0.0625*BS,0.1875*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.4375*BS,-0.3125*BS,0.3125*BS,-0.3125*BS,0.3125*BS
	));
}

void content_nodebox_bookshelf(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		0.4375*BS,-0.5*BS,-0.5*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.5*BS,-0.4375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.4375*BS,-0.0625*BS,0.4375*BS,0.4375*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,0.4375*BS,-0.5*BS,0.4375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.5*BS,-0.5*BS,0.4375*BS,-0.4375*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.0625*BS,-0.5*BS,0.4375*BS,0.0625*BS,0.5*BS
	));
}

// middle section
void content_nodebox_couch_centre(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.3125*BS,0.5*BS,0.*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.*BS,0.1875*BS,0.5*BS,0.4375*BS,0.375*BS
	));
}

// left end
void content_nodebox_couch_left(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.375*BS,-0.5*BS,-0.3125*BS,0.5*BS,0.*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,0.*BS,0.1875*BS,0.5*BS,0.4375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.1875*BS,-0.25*BS,-0.3125*BS,0.25*BS,0.3125*BS
	));
}

// right end
void content_nodebox_couch_right(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.3125*BS,0.375*BS,0.*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.*BS,0.1875*BS,0.375*BS,0.4375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		0.25*BS,-0.1875*BS,-0.25*BS,0.4375*BS,0.25*BS,0.3125*BS
	));
}

// arm chair
void content_nodebox_couch_chair(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.375*BS,-0.5*BS,-0.3125*BS,0.375*BS,0.*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,0.*BS,0.1875*BS,0.375*BS,0.4375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.1875*BS,-0.25*BS,-0.3125*BS,0.25*BS,0.3125*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.1875*BS,-0.25*BS,0.4375*BS,0.25*BS,0.3125*BS
	));
}

// inner corner
void content_nodebox_couch_inner(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.3125*BS,0.375*BS,0.*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.*BS,0.1875*BS,0.375*BS,0.4375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.5*BS,-0.5*BS,0.375*BS,0.*BS,-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.*BS,-0.5*BS,0.375*BS,0.4375*BS,0.1875*BS
	));
}

// outer corner
void content_nodebox_couch_outer(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.3125*BS,-0.5*BS,-0.3125*BS,0.375*BS,0.*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,-0.5*BS,-0.3125*BS,0.5*BS,0.*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.*BS,0.1875*BS,0.5*BS,0.4375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.*BS,0.375*BS,0.375*BS,0.4375*BS,0.5*BS
	));
}

void content_nodebox_knob(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		// -3 / 16
		-0.1875*BS,
		// -3 / 16
		-0.1875*BS,
		// 3 / 16
		0.1875*BS,

		// 3 / 16
		0.1875*BS,
		// 3 / 16
		0.1875*BS,
		// 8 / 16
		0.5*BS
	));
}

void content_nodebox_battery(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.375*BS,-0.5*BS,-0.375*BS,0.375*BS,0.25*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,0.25*BS,-0.1875*BS,0.1875*BS,0.5*BS,0.1875*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,-0.5*BS,-0.0625*BS,0.5*BS,-0.4375*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.0625*BS,-0.375*BS,-0.4375*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.5*BS,0.375*BS,0.0625*BS,-0.4375*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.5*BS,-0.5*BS,0.0625*BS,-0.4375*BS,-0.375*BS
	));
}

void content_nodebox_switch(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.1875*BS,-0.0625*BS,0.4375*BS,0.1875*BS,0.125*BS,0.5*BS
	));
}

void content_nodebox_button(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.1875*BS,-0.0625*BS,0.375*BS,0.1875*BS,0.125*BS,0.5*BS
	));
}

void content_nodebox_logicgate(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.375*BS,0.25*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.5*BS,0.25*BS,0.0625*BS,-0.4375*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.5*BS,-0.5*BS,0.0625*BS,-0.4375*BS,-0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.0625*BS,-0.125*BS,-0.4375*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.125*BS,-0.5*BS,-0.0625*BS,0.5*BS,-0.4375*BS,0.0625*BS
	));
}

void content_nodebox_piston(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.25*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.125*BS,-0.5*BS,0.125*BS,0.125*BS,-0.25*BS
	));
}

void content_nodebox_pistonarm(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.125*BS,-0.25*BS,0.125*BS,0.125*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.5*BS,0.5*BS,0.5*BS,-0.25*BS
	));
}

void content_nodebox_piston_up(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.5*BS,0.5*BS,0.25*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,0.25*BS,-0.125*BS,0.125*BS,0.5*BS,0.125*BS
	));
}

void content_nodebox_pistonarm_up(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.25*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.25*BS,-0.5*BS,0.5*BS,0.5*BS,0.5*BS
	));
}

void content_nodebox_piston_down(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.25*BS,-0.5*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.25*BS,0.125*BS
	));
}

void content_nodebox_pistonarm_down(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.25*BS,-0.125*BS,0.125*BS,0.5*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.5*BS,0.5*BS,-0.25*BS,0.5*BS
	));
}

void content_nodebox_waterwheel(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		0.375*BS,-0.5*BS,-0.5*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.5*BS,-0.375*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.0625*BS,-0.0625*BS,0.375*BS,0.0625*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,0.0625*BS,0.*BS,0.375*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.0625*BS,0.0625*BS,0.375*BS,0.*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.5*BS,-0.0625*BS,0.375*BS,-0.0625*BS,0.*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,0.*BS,-0.5*BS,0.375*BS,0.0625*BS,-0.0625*BS
	));
}

void content_nodebox_clock(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.4375*BS,-0.0625*BS,0.5*BS,0.25*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.125*BS,-0.25*BS,-0.4375*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		0.25*BS,-0.5*BS,-0.125*BS,0.5*BS,-0.4375*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		0.4375*BS,-0.4375*BS,-0.125*BS,0.5*BS,0.25*BS,-0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.4375*BS,-0.125*BS,-0.4375*BS,0.25*BS,-0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.4375*BS,-0.125*BS,0.4375*BS,-0.375*BS,-0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,0.1875*BS,-0.125*BS,0.4375*BS,0.25*BS,-0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.*BS,-0.0625*BS,-0.125*BS,0.0625*BS,0.*BS,-0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.*BS,-0.1875*BS,-0.125*BS,0.0625*BS,-0.125*BS,-0.0625*BS
	));
}

void content_nodebox_fence_inv(ContentFeatures *f)
{
	f->setWieldNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.625*BS,0.125*BS
	));
	f->addWieldNodeBox(NodeBox(
		-0.5*BS,0.375*BS,-0.0625*BS,-0.125*BS,0.5*BS,0.0625*BS
	));
	f->addWieldNodeBox(NodeBox(
		-0.5*BS,-0.125*BS,-0.0625*BS,-0.125*BS,0.*BS,0.0625*BS
	));
	f->addWieldNodeBox(NodeBox(
		0.125*BS,0.375*BS,-0.0625*BS,0.5*BS,0.5*BS,0.0625*BS
	));
	f->addWieldNodeBox(NodeBox(
		0.125*BS,-0.125*BS,-0.0625*BS,0.5*BS,0.*BS,0.0625*BS
	));
}

void content_nodebox_fence(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.625*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.5*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		0.125*BS,0.375*BS,-0.0625*BS,0.5*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.125*BS,-0.125*BS,-0.0625*BS,0.5*BS,0.*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.375*BS,-0.0625*BS,-0.125*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.125*BS,-0.0625*BS,-0.125*BS,0.*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,0.375*BS,0.125*BS,0.0625*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.125*BS,0.125*BS,0.0625*BS,0.*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,0.375*BS,-0.5*BS,0.0625*BS,0.5*BS,-0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.125*BS,-0.5*BS,0.0625*BS,0.*BS,-0.125*BS
	));
}

void content_nodebox_wall_inv(ContentFeatures *f)
{
	f->setWieldNodeBox(NodeBox(
		-0.1875*BS,-0.5*BS,-0.1875*BS,0.1875*BS,0.625*BS,0.1875*BS
	));
	f->addWieldNodeBox(NodeBox(
		0.1875*BS,-0.5*BS,-0.125*BS,0.5*BS,0.5*BS,0.125*BS
	));
	f->addWieldNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.125*BS,-0.1875*BS,0.5*BS,0.125*BS
	));
}

void content_nodebox_wall(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.1875*BS,-0.5*BS,-0.1875*BS,0.1875*BS,0.625*BS,0.1875*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,-0.5*BS,-0.1875*BS,0.1875*BS,0.5*BS,0.1875*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.5*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		0.125*BS,-0.5*BS,-0.125*BS,0.5*BS,0.5*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.125*BS,-0.125*BS,0.5*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,0.125*BS,0.125*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.5*BS,0.125*BS,0.5*BS,-0.125*BS
	));
}

void content_nodebox_bars(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.25*BS,-0.125*BS,0.5*BS,-0.1875*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,0.375*BS,-0.125*BS,0.5*BS,0.4375*BS,0.125*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.5*BS,-0.0625*BS,-0.375*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.5*BS,-0.0625*BS,0.375*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		0.0625*BS,-0.5*BS,-0.0625*BS,0.125*BS,0.5*BS,0.0625*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,-0.5*BS,-0.0625*BS,-0.125*BS,0.5*BS,0.0625*BS
	));
}

void content_nodebox_youngtree(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.5*BS,0.125*BS
	));
}

void content_nodebox_flag(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,0.5*BS,0.125*BS
	));
}

void content_nodebox_flower_pot(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		v3s16(0,-1,0),v3f(0,0,0),-0.3125*BS,-0.5*BS,-0.3125*BS,0.3125*BS,0.25*BS,0.3125*BS
	));
	f->addNodeBox(NodeBox(
		v3s16(0,-1,0),v3f(0,0,0),-0.375*BS,0.125*BS,-0.375*BS,-0.3125*BS,0.375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		v3s16(0,-1,0),v3f(0,0,0),0.3125*BS,0.125*BS,-0.375*BS,0.375*BS,0.375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		v3s16(0,-1,0),v3f(0,0,0),-0.3125*BS,0.125*BS,-0.375*BS,0.3125*BS,0.375*BS,-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		v3s16(0,-1,0),v3f(0,0,0),-0.3125*BS,0.125*BS,0.3125*BS,0.3125*BS,0.375*BS,0.375*BS
	));
}

void content_nodebox_parcel(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		v3s16(0,40,0),v3f(0,0,0),-0.375*BS,-0.5*BS,-0.3125*BS,0.375*BS,-0.1875*BS,0.3125*BS
	));
	f->addNodeBox(NodeBox(
		v3s16(0,-10,0),v3f(0,0,0),-0.0625*BS,-0.1875*BS,-0.0625*BS,0.,-0.125*BS,0.125*BS
	));
}

void content_nodebox_cauldron(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.375*BS,-0.125*BS,-0.375*BS,0.375*BS,-0.0625*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.5*BS,-0.4375*BS,-0.3125*BS,-0.125*BS,-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.5*BS,-0.4375*BS,0.4375*BS,-0.125*BS,-0.3125*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.5*BS,0.3125*BS,-0.3125*BS,-0.125*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.5*BS,0.3125*BS,0.4375*BS,-0.125*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.4375*BS,-0.125*BS,-0.4375*BS,-0.375*BS,0.5*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.375*BS,-0.125*BS,-0.4375*BS,0.4375*BS,0.5*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.125*BS,0.375*BS,0.375*BS,0.5*BS,0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,-0.125*BS,-0.4375*BS,0.375*BS,0.5*BS,-0.375*BS
	));
}

void content_nodebox_forge(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.375*BS,-0.5*BS,-0.375*BS,0.375*BS,0.375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,0.375*BS,-0.375*BS,-0.1875*BS,0.4375*BS,-0.1875*BS
	));
	f->addNodeBox(NodeBox(
		-0.375*BS,0.375*BS,0.1875*BS,-0.1875*BS,0.4375*BS,0.375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.375*BS,-0.375*BS,0.375*BS,0.4375*BS,-0.1875*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.375*BS,0.1875*BS,0.375*BS,0.4375*BS,0.375*BS
	));
}

void content_nodebox_furnace(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.375*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.5*BS,-0.4375*BS,0.5*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.4375*BS,-0.3125*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,0.25*BS,-0.4375*BS,0.3125*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.5*BS,-0.4375*BS,0.3125*BS,-0.25*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.0625*BS,-0.4375*BS,0.3125*BS,0.0625*BS,-0.375*BS
	));
}

void content_nodebox_lockedfurnace(ContentFeatures *f)
{
	content_nodebox_furnace(f);
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.375*BS,-0.5*BS,0.3125*BS,-0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,0.3125*BS,-0.5*BS,0.3125*BS,0.375*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.3125*BS,-0.3125*BS,-0.5*BS,-0.25*BS,0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.25*BS,-0.3125*BS,-0.5*BS,0.3125*BS,0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		-0.125*BS,-0.3125*BS,-0.5*BS,-0.0625*BS,0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.0625*BS,-0.3125*BS,-0.5*BS,0.125*BS,0.3125*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.3125*BS,-0.1875*BS,-0.5*BS,0.375*BS,0.*BS,-0.4375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,-0.1875*BS,-0.5*BS,0.25*BS,0.*BS,-0.4375*BS
	));
}

void content_nodebox_incinerator(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.375*BS,0.5*BS,0.5*BS,0.5*BS
	));
	f->addNodeBox(NodeBox(
		0.25*BS,-0.5*BS,-0.4375*BS,0.5*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.5*BS,-0.5*BS,-0.4375*BS,-0.25*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.25*BS,0.25*BS,-0.4375*BS,-0.1875*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		0.1875*BS,0.25*BS,-0.4375*BS,0.25*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,0.3125*BS,-0.4375*BS,0.1875*BS,0.5*BS,-0.375*BS
	));
	f->addNodeBox(NodeBox(
		-0.25*BS,-0.5*BS,-0.4375*BS,0.25*BS,-0.0625*BS,-0.375*BS
	));
}

void content_nodebox_rock(ContentFeatures *f)
{
	f->setNodeBox(NodeBox(
		-0.125*BS,-0.5*BS,-0.125*BS,0.125*BS,-0.3125*BS,0.1875*BS
	));
	f->addNodeBox(NodeBox(
		-0.1875*BS,-0.4375*BS,-0.0625*BS,-0.0625*BS,-0.375*BS,0.25*BS
	));
	f->addNodeBox(NodeBox(
		0.125*BS,-0.5*BS,-0.125*BS,0.1875*BS,-0.4375*BS,0.1875*BS
	));
	f->addNodeBox(NodeBox(
		-0.0625*BS,-0.5*BS,-0.25*BS,0.1875*BS,-0.375*BS,-0.125*BS
	));
}
