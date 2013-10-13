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

#ifndef CONTENT_MAPNODE_HEADER
#define CONTENT_MAPNODE_HEADER

#include "mapnode.h"

void content_mapnode_init();

extern content_t trans_table_19[21][2];

MapNode mapnode_translate_from_internal(MapNode n_from, u8 version);
MapNode mapnode_translate_to_internal(MapNode n_from, u8 version);

/*
	Node content type IDs
	Ranges:
*/

// 0x000...0x07f (0...127): param2 is fully usable
// 126 and 127 are reserved.
// Use these sparingly, only when the extra space in param2 might be needed.
// Add a space when there is unused space between numbers.
#define CONTENT_STONE 0

#define CONTENT_WATER 2
#define CONTENT_TORCH 3
#define CONTENT_GLASSLIGHT 4

#define CONTENT_WATERSOURCE 9

#define CONTENT_SIGN_WALL 14
#define CONTENT_CHEST 15
#define CONTENT_FURNACE 16
#define CONTENT_LOCKABLE_CHEST 17
#define CONTENT_SIGN 18
#define CONTENT_SIGN_UD 19

#define CONTENT_FENCE 21

#define CONTENT_RAIL 30
#define CONTENT_LADDER 31
#define CONTENT_LAVA 32
#define CONTENT_LAVASOURCE 33

// 0x800...0xfff (2048...4095): higher 4 bytes of param2 are not usable
#define CONTENT_GRASS 0x800 //1
#define CONTENT_TREE 0x801 //4
#define CONTENT_LEAVES 0x802 //5
#define CONTENT_FARM_DIRT 0x803 //6
#define CONTENT_MESE 0x804 //7
#define CONTENT_MUD 0x805 //8
#define CONTENT_COTTON 0x806 //10
#define CONTENT_BORDERSTONE 0x807 //11
#define CONTENT_WOOD 0x808 //12
#define CONTENT_SAND 0x809 //13
#define CONTENT_COBBLE 0x80a //18
#define CONTENT_STEEL 0x80b //19
#define CONTENT_GLASS 0x80c //20
#define CONTENT_MOSSYCOBBLE 0x80d //22
#define CONTENT_GRAVEL 0x80e //23
#define CONTENT_SANDSTONE 0x80f //24
#define CONTENT_CACTUS 0x810 //25
#define CONTENT_BRICK 0x811 //26
#define CONTENT_CLAY 0x812 //27
#define CONTENT_PAPYRUS 0x813 //28
#define CONTENT_BOOKSHELF 0x814 //29
#define CONTENT_JUNGLETREE 0x815
#define CONTENT_JUNGLEGRASS 0x816
#define CONTENT_NC 0x817
#define CONTENT_NC_RB 0x818
#define CONTENT_APPLE 0x819
#define CONTENT_JUNGLEWOOD 0x81a
#define CONTENT_STONEBRICK 0x81b
#define CONTENT_STONEBLOCK 0x81c
#define CONTENT_SAPLING 0x820

// slabs
#define CONTENT_COBBLE_SLAB 0x821
#define CONTENT_MOSSYCOBBLE_SLAB 0x822
#define CONTENT_STONE_SLAB 0x823
#define CONTENT_WOOD_SLAB 0x824
#define CONTENT_JUNGLE_SLAB 0x825
#define CONTENT_BRICK_SLAB 0x826
#define CONTENT_SANDSTONE_SLAB 0x827

// stairs
#define CONTENT_COBBLE_STAIR 0x828
#define CONTENT_MOSSYCOBBLE_STAIR 0x829
#define CONTENT_STONE_STAIR 0x82a
#define CONTENT_WOOD_STAIR 0x82b
#define CONTENT_JUNGLE_STAIR 0x82c
#define CONTENT_BRICK_STAIR 0x82d
#define CONTENT_SANDSTONE_STAIR 0x82e

#define CONTENT_SLAB_MASK 0x827
#define CONTENT_SLAB_STAIR_MIN 0x821
#define CONTENT_SLAB_STAIR_MAX 0x82F
#define CONTENT_SLAB_STAIR_FLIP 0x010

// upside down slabs
#define CONTENT_COBBLE_SLAB_UD 0x831
#define CONTENT_MOSSYCOBBLE_SLAB_UD 0x832
#define CONTENT_STONE_SLAB_UD 0x833
#define CONTENT_WOOD_SLAB_UD 0x834
#define CONTENT_JUNGLE_SLAB_UD 0x835
#define CONTENT_BRICK_SLAB_UD 0x836
#define CONTENT_SANDSTONE_SLAB_UD 0x837

// upside down stairs
#define CONTENT_COBBLE_STAIR_UD 0x838
#define CONTENT_MOSSYCOBBLE_STAIR_UD 0x839
#define CONTENT_STONE_STAIR_UD 0x83a
#define CONTENT_WOOD_STAIR_UD 0x83b
#define CONTENT_JUNGLE_STAIR_UD 0x83c
#define CONTENT_BRICK_STAIR_UD 0x83d
#define CONTENT_SANDSTONE_STAIR_UD 0x83e

#define CONTENT_SLAB_STAIR_UD_MIN 0x831
#define CONTENT_SLAB_STAIR_UD_MAX 0x83F

// doors
#define CONTENT_WOOD_DOOR_LB 0x840
#define CONTENT_WOOD_DOOR_LT 0x841
#define CONTENT_STEEL_DOOR_LB 0x842
#define CONTENT_STEEL_DOOR_LT 0x843
#define CONTENT_WOOD_W_DOOR_LB 0x844
#define CONTENT_WOOD_W_DOOR_LT 0x845
#define CONTENT_STEEL_W_DOOR_LB 0x846
#define CONTENT_STEEL_W_DOOR_LT 0x847
#define CONTENT_WOOD_DOOR_RB 0x848
#define CONTENT_WOOD_DOOR_RT 0x849
#define CONTENT_STEEL_DOOR_RB 0x84A
#define CONTENT_STEEL_DOOR_RT 0x84B
#define CONTENT_WOOD_W_DOOR_RB 0x84C
#define CONTENT_WOOD_W_DOOR_RT 0x84D
#define CONTENT_STEEL_W_DOOR_RB 0x84E
#define CONTENT_STEEL_W_DOOR_RT 0x84F

#define CONTENT_WOOD_HATCH 0x850
#define CONTENT_STEEL_HATCH 0x852
#define CONTENT_WOOD_W_HATCH 0x854
#define CONTENT_STEEL_W_HATCH 0x856

// open doors
#define CONTENT_WOOD_DOOR_LB_OPEN 0x860
#define CONTENT_WOOD_DOOR_LT_OPEN 0x861
#define CONTENT_STEEL_DOOR_LB_OPEN 0x862
#define CONTENT_STEEL_DOOR_LT_OPEN 0x863
#define CONTENT_WOOD_W_DOOR_LB_OPEN 0x864
#define CONTENT_WOOD_W_DOOR_LT_OPEN 0x865
#define CONTENT_STEEL_W_DOOR_LB_OPEN 0x866
#define CONTENT_STEEL_W_DOOR_LT_OPEN 0x867
#define CONTENT_WOOD_DOOR_RB_OPEN 0x868
#define CONTENT_WOOD_DOOR_RT_OPEN 0x869
#define CONTENT_STEEL_DOOR_RB_OPEN 0x86A
#define CONTENT_STEEL_DOOR_RT_OPEN 0x86B
#define CONTENT_WOOD_W_DOOR_RB_OPEN 0x86C
#define CONTENT_WOOD_W_DOOR_RT_OPEN 0x86D
#define CONTENT_STEEL_W_DOOR_RB_OPEN 0x86E
#define CONTENT_STEEL_W_DOOR_RT_OPEN 0x86F

#define CONTENT_WOOD_HATCH_OPEN 0x870
#define CONTENT_STEEL_HATCH_OPEN 0x872
#define CONTENT_WOOD_W_HATCH_OPEN 0x874
#define CONTENT_STEEL_W_HATCH_OPEN 0x876

#define CONTENT_DOOR_MIN 0x840
#define CONTENT_DOOR_MAX 0x87F
#define CONTENT_DOOR_STEEL_MASK 0x002
#define CONTENT_DOOR_SECT_MASK 0x001
#define CONTENT_DOOR_OPEN_MASK 0x020
#define CONTENT_HATCH_MASK 0x050

// plants
#define CONTENT_WILDGRASS_SHORT 0x880
#define CONTENT_WILDGRASS_LONG 0x881
#define CONTENT_DEADGRASS 0x882
#define CONTENT_FLOWER_STEM 0x883
#define CONTENT_FLOWER_ROSE 0x884
#define CONTENT_FLOWER_DAFFODIL 0x885
#define CONTENT_FLOWER_TULIP 0x886
#define CONTENT_FLOWER_POT_RAW 0x887
#define CONTENT_FLOWER_POT 0x888
#define CONTENT_TRIMMED_LEAVES 0x889

#define CONTENT_COTTON_BLUE 0x890
#define CONTENT_COTTON_GREEN 0x891
#define CONTENT_COTTON_ORANGE 0x892
#define CONTENT_COTTON_PURPLE 0x893
#define CONTENT_COTTON_RED 0x894
#define CONTENT_COTTON_YELLOW 0x895
#define CONTENT_COTTON_BLACK 0x896

#endif

