Voxelands
---------------
An InfiniMiner/Minecraft inspired game.
Copyright (c) 2013-2014 Lisa 'darkrose' Milne <lisa@ltmnet.com>
Forked from Minetest 0.3.x
Copyright (c) 2010-2011 Perttu Ahola <celeron55@gmail.com>
(see source files for other contributors)

Further documentation:
----------------------
- Website: http://www.voxelands.com/
- doc/ directory of source distribution

This game is not finished:
--------------------------
- Don't expect it to work as well as a finished game will.
- Please report any bugs to me. debug.txt is useful.

Controls:
---------
- Settable in the Settings tab of the main menu
- Settable in the configuration file, see the section below.
	- WASD: Walk
	- Mouse left: dig/hit
	- Mouse right: place/use
	- Mouse wheel: select item
	- 0...9: select item
	- Shift: sneak
	- R: Toggle viewing all loaded chunks
	- I: Inventory menu
	- ESC: This menu
	- T: Chat

Map directory:
--------------
- Map is stored in a directory, which can be removed to generate a new map.
- There is a command-line option for it: --map-dir
- For a RUN_IN_PLACE build, it is located in:
		../world
- Otherwise something like this:
	Windows: C:\Documents and Settings\user\Application Data\voxelands\world
	Linux: ~/.voxelands/world
	OS X: ~/Library/Application Support/voxelands/world

Configuration file:
-------------------
- An optional configuration file can be used. See voxelands.conf.example.
- Path to file can be passed as a parameter to the executable:
	--config <path-to-file>
- Defaults:
	- If built with -DRUN_IN_PLACE=1:
		../voxelands.conf
		../../voxelands.conf
	- Otherwise something like this:
		Windows: C:\Documents and Settings\user\Application Data\voxelands/voxelands.conf
		Linux: ~/.voxelands/voxelands.conf
		OS X: ~/Library/Application Support/voxelands.conf

Command-line options:
---------------------
- Use --help

Compiling on GNU/Linux:
-----------------------

Install dependencies. Here's an example for Debian/Ubuntu:
$ apt-get install build-essential libirrlicht-dev cmake libbz2-dev libpng12-dev libjpeg8-dev libxxf86vm-dev libgl1-mesa-dev libsqlite3-dev libopenal-dev libvorbis-dev

Here's an example for RedHat/Fedora/CentOS:
$ yum install irrlicht-devel bzip2-devel libpng-devel libjpeg-turbo-devel libXxf86vm-devel mesa-libglapi libsqlite3x-devel

Download source, extract (replace <VERSION> in this URL with the latest stable release version, i.e. 1401.00):
$ wget http://www.voxelands.com/downloads/voxelands-<VERSION>-src.tar.bz2
$ tar xf voxelands-*-src.tar.bz2
$ cd voxelands

Build a version that runs directly from the source directory:
$ cmake . -DRUN_IN_PLACE=1
$ make -j2

Run it:
$ cd bin
$ ./voxelands

- Use cmake . -LH to see all CMake options and their current state
- If you want to install it system-wide (or are making a distribution package), you will want to use -DRUN_IN_PLACE=0
- You can build a bare server or a bare client by specifying -DBUILD_CLIENT=0 or -DBUILD_SERVER=0
- You can select between Release and Debug build by -DCMAKE_BUILD_TYPE=<Debug or Release>
  - Debug build is slower, but gives much more useful output in a debugger

Compiling on Mac OS X
---------------------
- You need:
	* XCode 5 - https://developer.apple.com/xcode/downloads/
	* XCode commandline tools - https://developer.apple.com/downloads/
	* homebrew - http://brew.sh/

- Steps:
	* Install dependencies
		- brew install jpeg-turbo irrlicht
	* make XCode project for client only
		- cmake -DJPEG_INCLUDE_LIBRARY=/usr/local/Cellar/jpeg-turbo/<version>/include -DJPEG_LIBRARY=/usr/local/Cellar/jpeg-turbo/<version>lib/libturbojpeg.a -DBUILD_SERVER=0 -DRUN_IN_PLACE=0 -G Xcode .
	* build XCode project from commandline (If you're using the GUI, make sure to select "package" as build target)
		- xcodebuild -project voxelands.xcodeproj -target package

Compiling on Windows:
---------------------

- You need:
	* CMake:
		http://www.cmake.org/cmake/resources/software.html
	* MinGW or Visual Studio
		http://www.mingw.org/
		http://msdn.microsoft.com/en-us/vstudio/default
	* Irrlicht SDK 1.7:
		http://irrlicht.sourceforge.net/downloads.html
	* Zlib headers (zlib125.zip)
		http://www.winimage.com/zLibDll/index.html
	* Zlib library (zlibwapi.lib and zlibwapi.dll from zlib125dll.zip):
		http://www.winimage.com/zLibDll/index.html
	* Optional: gettext bibrary and tools:
		http://gnuwin32.sourceforge.net/downlinks/gettext.php
		- This is used for other UI languages. Feel free to leave it out.
	* And, of course, Voxelands:
		https://gitorious.org/minetest-classic/minetest-classic/archive-tarball/master
- Steps:
	- Select a directory called DIR hereafter in which you will operate.
	- Make sure you have CMake and a compiler installed.
	- Download all the other stuff to DIR and extract them into there.
	  ("extract here", not "extract to packagename/")
	- All those packages contain a nice base directory in them, which
	  should end up being the direct subdirectories of DIR.
	- You will end up with a directory structure like this (+=dir, -=file):
	-----------------
	+ DIR
		- zlib-1.2.5.tar.gz
		- zlib125dll.zip
		- irrlicht-1.8.1.zip
		- voxelands.zip (or whatever, this is the voxelands source)
		+ zlib-1.2.5
			- zlib.h
			+ win32
			...
		+ zlib125dll
			- readme.txt
			+ dll32
			...
		+ irrlicht-1.8.1
			+ lib
			+ include
			...
		+ gettext (optional)
			+bin
			+include
			+lib
		+ voxelands
			+ src
			+ doc
			- CMakeLists.txt
			...
	-----------------
	- Start up the CMake GUI
	- Select "Browse Source..." and select DIR/voxelands
	- Now, if using MSVC:
		- Select "Browse Build..." and select DIR/voxelands-build
	- Else if using MinGW:
		- Select "Browse Build..." and select DIR/voxelands
	- Select "Configure"
	- Select your compiler
	- It will warn about missing stuff, ignore that at this point. (later don't)
	- Make sure the configuration is as follows
	  (note that the versions may differ for you):
	-----------------
	BUILD_CLIENT             [X]
	BUILD_SERVER             [ ]
	CMAKE_BUILD_TYPE         Release
	CMAKE_INSTALL_PREFIX     DIR/voxelands-install
	IRRLICHT_SOURCE_DIR      DIR/irrlicht-1.8.1
	RUN_IN_PLACE             [X]
	WARN_ALL                 [ ]
	ZLIB_DLL                 DIR/zlib125dll/dll32/zlibwapi.dll
	ZLIB_INCLUDE_DIR         DIR/zlib-1.2.5
	ZLIB_LIBRARIES           DIR/zlib125dll/dll32/zlibwapi.lib
	GETTEXT_BIN_DIR          DIR/gettext/bin
	GETTEXT_INCLUDE_DIR      DIR/gettext/include
	GETTEXT_LIBRARIES        DIR/gettext/lib/intl.lib
	GETTEXT_MSGFMT           DIR/gettext/bin/msgfmt
	-----------------
	- Hit "Configure"
	- Hit "Configure" once again 8)
	- If something is still coloured red, you have a problem.
	- Hit "Generate"
	If using MSVC:
		- Open the generated voxelands.sln
		- The project defaults to the "Debug" configuration. Make very sure to
		  select "Release", unless you want to debug some stuff (it's slower
		  and might not even work at all)
		- Build the ALL_BUILD project
		- Build the INSTALL project
		- You should now have a working game with the executable in
			DIR/minetest-install/bin/voxelands.exe
		- Additionally you may create a zip package by building the PACKAGE
		  project.
	If using MinGW:
		- Using the command line, browse to the build directory and run 'make'
		  (or mingw32-make or whatever it happens to be)
		- You should now have a working game with the executable in
			DIR/minetest/bin/voxelands.exe

Windows releases of Voxelands are cross compiled on Linux, using Menche's build script:
--------------------------------------------------------------------

$ wget http://menche.us/files/minetest-classic/build-minetest-classic-win32.sh
$ sh ./build-minetest-classic-win32.sh

this script will install and compile the build-chain and all libraries needed for Voxelands

License of Voxelands textures
--------------------------------

This does not apply to texture packs made by others.

Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0)
http://creativecommons.org/licenses/by-sa/3.0/

License of Voxelands
-----------------------

Voxelands
Copyright (C) 2010-2013 celeron55, Perttu Ahola <celeron55@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.


Minetest-c55 was licensed under GPLv2 or later, the 'or later' option
 was taken by Lisa 'darkrose' Milne when forking Voxelands, thus
 updating the license to GPLv3

Irrlicht
---------------

This program uses the Irrlicht Engine. http://irrlicht.sourceforge.net/

 The Irrlicht Engine License

Copyright © 2002-2005 Nikolaus Gebhardt

This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute
it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you
      must not claim that you wrote the original software. If you use
	  this software in a product, an acknowledgment in the product
	  documentation would be appreciated but is not required.
   2. Altered source versions must be plainly marked as such, and must
      not be misrepresented as being the original software.
   3. This notice may not be removed or altered from any source
      distribution.


JThread
---------------

This program uses the JThread library. License for JThread follows:

Copyright (c) 2000-2006  Jori Liesenborgs (jori.liesenborgs@gmail.com)

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
IN THE SOFTWARE.


