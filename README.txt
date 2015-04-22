========================================================================
				README
========================================================================

ABOUT:
Voxelands is a sandbox construction game based on Minetest, which was
 inspired by earlier "voxel world" games such as Infiniminer. Gameplay
 puts players in a fully destructible 3D game world where they can place
 and dig numerous types of blocks, items and creatures using a variety
 of tools. Inside the game world players can build structures, artworks
 and anything else their creativity can think of on multiplayer servers
 and singleplayer worlds across multiple game modes.

DEPENDENCIES:
Voxelands requires the standard C++ library, irrlicht 1.8 or later,
 freetype, openAL, vorbisfile, ogg, and zlib. CMake, make, and g++ (or other
 C++ compiler) are required to build from source.

INSTALL:
If you're reading this then you've already unpacked the tarball, so
 just:
1) cd /path/to/voxelands
2) cmake -DRUN_IN_PLACE=1 .
2) make -j3

WINDOWS:
Voxelands can be cross compiled with mingw, or built natively with MSVC,
 instructions for the latter are available on the forums:
 http://forum.voxelands.com

USE:
Simply run bin/voxelands from any console/terminal.

CONTROLS:
Controls can be set in the Settings tab of the main menu
 W: walk forward
 A: walk left
 S: walk backwards
 D: walk right
 Space: jump, move up
 E: run, move down
 Mouse left: dig/hit
 Mouse right: place/use
 Mouse wheel: select wielded item
 Shift: sneak
 R: Toggle viewing all loaded chunks
 I: Inventory menu
 ESC: Pause Menu / Exit game
 T: Chat

LICENSE:
Code:
Voxelands is licensed under the GPLv3 or later.
Copyright (c) 2013-2015 Lisa 'darkrose' Milne <lisa@ltmnet.com>
Forked from Minetest 0.3.x
Copyright (c) 2010-2011 Perttu Ahola <celeron55@gmail.com>

Textures, Sounds, and Models:

This does not apply to texture packs made by others.

Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
http://creativecommons.org/licenses/by-sa/3.0/

Artists include:
sdzen
darkrose
sapier
Tom Peter
Telaron
juskiddink
With special thanks to http://www.opengameart.org/

DEVELOPMENT:
Voxelands is under active development, the latest updates can be found
 at:
  https://gitlab.com/voxelands/voxelands
 or on IRC at:
  irc.entropynet.net #voxelands

CHANGES:
See doc/changelog.txt
