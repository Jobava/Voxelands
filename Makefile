PACKAGE=minetest-classic
TARGET=$(PACKAGE)
VERSION=1404.00
VERSION_EXTRA=:next

CXX      ?= g++
CXXFLAGS ?= -march=native -O2 -fomit-frame-pointer -fwrapv -fvisibility=hidden
LIBS     ?= -ljthread -lIrrlicht -lsqlite3 -lpthread -lz
LDFLAGS  ?= $(LIBS) -Wl,-O1,--discard-all,--no-undefined,--sort-common,--as-needed,--hash-style=gnu,-z,now,-z,relro
CPPFLAGS ?= -I./src -I/usr/include/irrlicht -I/usr/include/jthread -DNDEBUG -DVERSION=\"$(VERSION)$(VERSION_EXTRA)\" -DPACKAGE=\"$(PACKAGE)\"
MKDIR    ?= mkdir -p
RM       ?= rm -f

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
client: prepare bin/$(TARGET)
server: prepare bin/$(TARGET)-server

prepare:
	$(MKDIR) clientobj
	$(MKDIR) serverobj
	$(MKDIR) bin

bin/$(TARGET) : $(OBJECTS_CLIENT)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS_CLIENT) -o bin/$(TARGET)

bin/$(TARGET)-server : $(OBJECTS_SERVER)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS_SERVER) -o bin/$(TARGET)-server

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
	$(RM) jthread.o
	$(RM) jmutex.o
	$(RM) serverobj/*
	$(RM) clientobj/*

clean: distclean
	$(RM) bin/*

fresh: clean all

.PHONY: default client server all fresh prepare
