#!/bin/sh
# options to make. -j# should be set to the number of CPU cores + 1
export MAKEFLAGS="-j5"
export DIR=`pwd`

# First, download and compile the cross-compiler
if [ ! -e mxe ]; then
	git clone -b master git://github.com/mxe/mxe.git
else
	cd mxe && git pull
	cd $DIR
fi

cd mxe
	git checkout master
	make $MAKEFLAGS gcc
cd $DIR

# environment variables for cross-compiling
export CC="$DIR/mxe/usr/bin/i686-pc-mingw32.static-gcc"
export CXX="$DIR/mxe/usr/bin/i686-pc-mingw32.static-g++"
export AR="$DIR/mxe/usr/bin/i686-pc-mingw32.static-ar"
export C_INCLUDE_PATH="$DIR/mxe/usr/i686-pc-mingw32.static/include"
export CMAKE_TOOLCHAIN="$DIR/mxe/usr/i686-pc-mingw32.static/share/cmake/mxe-conf.cmake"
export CFLAGS="-m32 -march=i686 -O2 -fomit-frame-pointer -fwrapv -fvisibility=hidden -Wformat-security -Wformat-nonliteral -Wpointer-arith -Winit-self -pipe"
export CXXFLAGS="$CFLAGS"
export LDFLAGS="-Wl,-O1 -Wl,--discard-all,--no-undefined,--sort-common"

# cmake needs "windres.exe" exactly in the PATH.
# can't figure out how to change it and it doesn't like aliases.
ln -s $DIR/mxe/usr/bin/i686-pc-mingw32.static-windres $DIR/mxe/usr/bin/windres.exe
ln -s $DIR/mxe/usr/bin/i686-pc-mingw32.static-windres $DIR/mxe/usr/bin/windres
export PATH="$DIR/mxe/usr/bin:$PATH"

# Download the required libraries
if [ ! -e irrlicht-1.8/ ]; then
	wget http://downloads.sourceforge.net/irrlicht/irrlicht-1.8.zip
	unzip irrlicht-1.8.zip || exit 1
fi

# Build the required libraries
cd irrlicht-1.8/source/Irrlicht/
make $MAKEFLAGS NDEBUG=1 win32 || exit 1
cd $DIR

# Get the minetest-classic source
if [ ! -e voxelands ]; then
	git clone git://gitorious.org/minetest-classic/minetest-classic.git voxelands
else
	cd voxelands && git reset --hard
	cd $DIR
	cd voxelands && git pull
	cd $DIR
fi

BRANCH='master'
if [ $# = '1' ]; then
	export BRANCH=$1
fi
echo "Building branch: $BRANCH"

# Configure and build voxelands
cd voxelands
git checkout $BRANCH
sed -i "s/[\\][\\]/\//g" src/winresource.rc # Fix nasty Windoze paths
rm CMakeCache.txt
cmake . -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN \
	-DCMAKE_INSTALL_PREFIX=/tmp \
	-DCMAKE_C_FLAGS_RELEASE=-DNDEBUG -DCMAKE_CXX_FLAGS_RELEASE=-DNDEBUG \
	-DBUILD_SERVER=1 \
	-DRUN_IN_PLACE=1 \
	-DIRRLICHT_INCLUDE_DIR=$DIR/irrlicht-1.8/include \
	-DIRRLICHT_LIBRARY=$DIR/irrlicht-1.8/lib/Win32-gcc/libIrrlicht.a \
	-DIRRLICHT_DLL=$DIR/irrlicht-1.8/bin/Win32-gcc/Irrlicht.dll \
	-DZLIB_INCLUDE_DIR=$DIR/zlib/include \
	-DZLIB_LIBRARIES=$DIR/zlib/bin/zlibwapi.dll \
	-DZLIB_DLL=$DIR/zlib/bin/zlibwapi.dll \
	-DOGG_INCLUDE_DIR=$DIR/libogg/include \
	-DOGG_LIBRARY=$DIR/libogg/lib/libogg.dll.a \
	-DOGG_DLL=$DIR/libogg/bin/libogg-0.dll \
	-DVORBIS_INCLUDE_DIR=$DIR/libvorbis/include \
	-DVORBIS_LIBRARY=$DIR/libvorbis/lib/libvorbis.dll.a \
	-DVORBIS_DLL=$DIR/libvorbis/bin/libvorbis-0.dll \
	-DVORBISFILE_LIBRARY=$DIR/libvorbis/lib/libvorbisfile.dll.a \
	-DVORBISFILE_DLL=$DIR/libvorbis/bin/libvorbisfile-3.dll \
	-DOPENAL_INCLUDE_DIR=$DIR/libopenal/include \
	-DOPENAL_LIBRARY=$DIR/libopenal/lib/libOpenAL32.dll.a \
	-DOPENAL_DLL=$DIR/libopenal/bin/OpenAL32.dll \
	-DENABLE_AUDIO=1 || exit 1

make $MAKEFLAGS package || exit 1

cd $DIR

# Keep the environment clean!
rm $DIR/mxe/usr/bin/windres.exe
unset DIR
unset CC
unset CXX
unset AR
unset C_INCLUDE_PATH
unset CFLAGS
unset CXXFLAGS
unset CPPFLAGS
unset MAKEFLAGS
