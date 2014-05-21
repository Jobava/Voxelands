PACKAGE       =  minetest-classic
TARGET        =  $(PACKAGE)
VERSION       =  1404.00
VERSION_EXTRA =  :next
VERSION_LOCAL ?=
BUILD_FLAGS   ?=
ARCH          ?= POSIX

PWD=$(shell pwd)
MKDIR    ?= mkdir -p
RM       ?= rm -f
EXT      =
DEFS     = -DNDEBUG -DVERSION=\"$(VERSION)$(VERSION_EXTRA)$(VERSION_LOCAL)\" -DPACKAGE=\"$(PACKAGE)\" $(BUILD_FLAGS)

POSIX_CXX      ?= g++
POSIX_CXXFLAGS ?= -march=native -O2 -fomit-frame-pointer -fwrapv -fvisibility=hidden
POSIX_LIBS     ?= -ljthread -lIrrlicht -lsqlite3 -lz -lpthread
POSIX_LDFLAGS  ?= $(POSIX_LIBS) -Wl,-O1,--discard-all,--no-undefined,--sort-common,--as-needed,--hash-style=gnu,-z,now,-z,relro
POSIX_CPPFLAGS ?= -I./src -I/usr/include/irrlicht -I/usr/include/jthread $(DEFS)

MINGW        ?=
W32_CXX      ?= $(MINGW)i686-pc-mingw32-g++
W32_CXXFLAGS ?= -m32 -march=i686 -O2 -fomit-frame-pointer -fwrapv -fvisibility=hidden -mwindows -O2 -fomit-frame-pointer -pipe -D_FORTIFY_SOURCE=2
W32_LIBS     ?= -ljthread -lIrrlicht -lsqlite3 -lzlibstatic -lws2_32
W32_LDFLAGS  ?= -L$(PWD)/w32libs/lib/ $(W32_LIBS) -O2 -fomit-frame-pointer -pipe -D_FORTIFY_SOURCE=2
W32_CPPFLAGS ?= -I$(PWD)/src -I$(PWD)/w32libs/include/ $(DEFS) -DWIN32 -D_FORTIFY_SOURCE=2

LASTARCH=$(shell touch lastarch && cat lastarch)
ifeq "$(LASTARCH)" ""
	LASTARCH=$(ARCH)
endif

ifeq "$(ARCH)" "POSIX"
	CXX = $(POSIX_CXX)
	CXXFLAGS = $(POSIX_CFLAGS)
	LDFLAGS = $(POSIX_LDFLAGS)
	CPPFLAGS = $(POSIX_CPPFLAGS)
else
	CXX = $(W32_CXX)
	CXXFLAGS = $(W32_CFLAGS)
	LDFLAGS = $(W32_LDFLAGS)
	CPPFLAGS = $(W32_CPPFLAGS)
	EXT = .exe
endif

_SOURCES_COMMON = log.cpp content_sao.cpp mapgen.cpp content_inventory.cpp content_nodemeta.cpp content_craft.cpp content_craftitem.cpp content_toolitem.cpp content_mapnode.cpp content_list.cpp content_nodebox.cpp auth.cpp collision.cpp nodemetadata.cpp serverobject.cpp noise.cpp mineral.cpp porting.cpp defaultsettings.cpp mapnode.cpp voxel.cpp inventory.cpp debug.cpp serialization.cpp light.cpp filesys.cpp connection.cpp environment.cpp server.cpp servercommand.cpp socket.cpp mapblock.cpp mapsector.cpp map.cpp mesh.cpp player.cpp utility.cpp test.cpp sha1.cpp base64.cpp ban.cpp http.cpp path.cpp

_SOURCES_CLIENT = $(_SOURCES_COMMON) content_mapblock.cpp content_cao.cpp mapblock_mesh.cpp farmesh.cpp keycode.cpp camera.cpp clouds.cpp particles.cpp clientobject.cpp guiMainMenu.cpp guiKeyChangeMenu.cpp guiMessageMenu.cpp guiTextInputMenu.cpp guiFormSpecMenu.cpp guiPauseMenu.cpp guiPasswordChange.cpp guiDeathScreen.cpp client.cpp tile.cpp game.cpp main.cpp
SOURCES_CLIENT  = $(_SOURCES_CLIENT:%=src/%)
OBJECTS_CLIENT  = $(_SOURCES_CLIENT:%.cpp=clientobj/%.o)

_SOURCES_SERVER = $(_SOURCES_COMMON) servermain.cpp
SOURCES_SERVER  = $(_SOURCES_SERVER:%=src/%)
OBJECTS_SERVER  = $(_SOURCES_SERVER:%.cpp=serverobj/%.o)

DISTFILES=src doc data misc po util locale minetest.conf.example Makefile README.txt $(PACKAGE).desktop $(PACKAGE).ico $(PACKAGE).svg $(PACKAGE).png

all: default
default: client server
client: prepare bin/$(TARGET)$(EXT)
server: prepare bin/$(TARGET)-server$(EXT)

ifeq "$(ARCH)" "W32"
prepare:
	$(info $(MINGW))
	$(info $(W32_CXX))
ifeq "(shell which $(W32_CXX) 2>/dev/null)" ""
	$(info =====)
	$(info  mingw (i686-pc-mingw32-g++) is not in your PATH)
	$(info  you probably want to set MINGW to the directory containing it)
	$(info  eg. make w32 MINGW=/usr/mingw/bin/)
	$(info =====)
	$(error no crosscompiler)
endif
else
prepare:
endif
ifneq "$(ARCH)" "$(LASTARCH)"
	@$(MAKE) clean
endif
	@echo $(ARCH) > lastarch
	$(MKDIR) clientobj
	$(MKDIR) serverobj
	$(MKDIR) bin

bin/$(TARGET)$(EXT) : $(OBJECTS_CLIENT)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS_CLIENT) -o bin/$(TARGET)$(EXT)

bin/$(TARGET)-server$(EXT) : $(OBJECTS_SERVER)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS_SERVER) -o bin/$(TARGET)-server$(EXT)

jthread.o: src/jthread/pthread/jmutex.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o jmutex.o src/jthread/pthread/jmutex.cpp

jmutex.o: src/jthread/pthread/jthread.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o jthread.o src/jthread/pthread/jthread.cpp

clientobj/%.o: src/%.cpp src/config.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

serverobj/%.o: src/%.cpp src/config.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -DSERVER -c -o $@ $<

dist-base:
	mkdir -p $(PACKAGE)-$(VERSION)
	cp -Rt $(PACKAGE)-$(VERSION) $(DISTFILES)

dist-gz: dist-base
	tar czf $(PACKAGE)-$(VERSION).tar.gz $(PACKAGE)-$(VERSION)
	$(RM) -r $(PACKAGE)-$(VERSION)

dist-bz2: dist-base
	tar cjf $(PACKAGE)-$(VERSION).tar.bz2 $(PACKAGE)-$(VERSION)
	$(RM) -r $(PACKAGE)-$(VERSION)

dist-zip: dist-base
	zip -r $(PACKAGE)-$(VERSION).zip $(PACKAGE)-$(VERSION)
	$(RM) -r $(PACKAGE)-$(VERSION)

dist: distclean dist-bz2

distclean:
	$(RM) lastarch
	$(RM) jthread.o
	$(RM) jmutex.o
	$(RM) serverobj/*
	$(RM) clientobj/*

clean: distclean
	$(RM) bin/*

fresh: clean all

inplace:
	@$(MAKE) $(MAKEOPTS) BUILD_FLAGS=-DRUN_IN_PLACE=1 all

inplace-client:
	@$(MAKE) $(MAKEOPTS) BUILD_FLAGS=-DRUN_IN_PLACE=1 client

inplace-server:
	@$(MAKE) $(MAKEOPTS) BUILD_FLAGS=-DRUN_IN_PLACE=1 server

w32:
	@$(MAKE) ARCH="W32" MINGW=$(MINGW) all

w32-fresh: clean w32

.PHONY: default client server all fresh prepare inplace inplace-client inplace-server
