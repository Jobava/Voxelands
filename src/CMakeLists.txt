project(voxelands)
cmake_minimum_required( VERSION 2.6 )

if(RUN_IN_PLACE)
	add_definitions ( -DRUN_IN_PLACE )
endif(RUN_IN_PLACE)

set(USE_GETTEXT 1)

find_package(GettextLib)

if(GETTEXT_FOUND)
	message(STATUS "gettext msgfmt path: ${GETTEXT_MSGFMT}")
	message(STATUS "GetText enabled; locales found: ${GETTEXT_AVAILABLE_LOCALES}")
endif(GETTEXT_FOUND)

# user visible option to enable/disable audio
OPTION(ENABLE_AUDIO "Enable audio" ON)

## this is only set to 1 if audio is enabled _and_ available
set(USE_AUDIO 0)
set(SOUND_PROBLEM 0)

if(ENABLE_AUDIO AND BUILD_CLIENT)
	# Sound libraries
	find_package(OpenAL)
	find_package(Vorbis)
	if(NOT OPENAL_FOUND)
		message(STATUS "Sound enabled, but OpenAL not found!")
		set(SOUND_PROBLEM 1)
		MARK_AS_ADVANCED(CLEAR OPENAL_LIBRARY OPENAL_INCLUDE_DIR)
	endif()
	if(NOT VORBIS_FOUND)
		message(STATUS "Sound enabled, but Vorbis libraries not found!")
		set(SOUND_PROBLEM 1)
		MARK_AS_ADVANCED(CLEAR OGG_INCLUDE_DIR VORBIS_INCLUDE_DIR OGG_LIBRARY VORBIS_LIBRARY VORBISFILE_LIBRARY)
	endif()
	if(OPENAL_FOUND AND VORBIS_FOUND)
		set(USE_AUDIO 1)
		message(STATUS "Sound enabled")
	endif()
endif(ENABLE_AUDIO AND BUILD_CLIENT)

if(SOUND_PROBLEM)
	message(FATAL_ERROR "Sound enabled, but cannot be used.\n"
		"To continue, either fill in the required paths or disable sound. (-DENABLE_AUDIO=0)")
endif()
if(USE_AUDIO)
	set(audio_SRCS sound.cpp sound_openal.cpp)
	set(AUDIO_INCLUDE_DIRS
		${OPENAL_INCLUDE_DIR}
		${VORBIS_INCLUDE_DIR}
		${OGG_INCLUDE_DIR}
		)
	set(AUDIO_LIBRARIES
		${OPENAL_LIBRARY}
		${VORBIS_LIBRARIES}
		)
else()
	set(audio_SRCS sound.cpp)
endif()

option(ENABLE_FREETYPE "Enable freetype2 (truetype fonts and basic unicode support)" ON)
set(USE_FREETYPE 0)

if(ENABLE_FREETYPE)
	find_package(Freetype REQUIRED)
	if (NOT FREETYPE_FOUND)
		MARK_AS_ADVANCED(CLEAR FREETYPE_INCLUDE_DIR_ft2build FREETYPE_INCLUDE_DIR_freetype2 FREETYPE_LIBRARY)
	endif()
	set(USE_FREETYPE 1)
endif(ENABLE_FREETYPE)

if(NOT MSVC)
	set(USE_GPROF 0 CACHE BOOL "Use -pg flag for g++")
endif()

# Use cmake_config.h
add_definitions ( -DUSE_CMAKE_CONFIG_H )

if(WIN32)
	# Windows
	if(MSVC) # MSVC Specifics
		# Surpress some useless warnings
		add_definitions ( /D "_CRT_SECURE_NO_DEPRECATE" /W1 )
	else() # Probably MinGW = GCC
		set(PLATFORM_LIBS ws2_32.lib)
	endif()
	# Zlib stuff
	set(ZLIB_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/../../zlib/zlib-1.2.5"
			CACHE PATH "Zlib include directory")
	set(ZLIB_LIBRARIES "${PROJECT_SOURCE_DIR}/../../zlib125dll/dll32/zlibwapi.lib"
			CACHE FILEPATH "Path to zlibwapi.lib")
	set(ZLIB_DLL "${PROJECT_SOURCE_DIR}/../../zlib125dll/dll32/zlibwapi.dll"
			CACHE FILEPATH "Path to zlibwapi.dll (for installation)")
	set(IRRLICHT_SOURCE_DIR "${PROJECT_SOURCE_DIR}/../../irrlicht-1.7.2"
			CACHE PATH "irrlicht dir")
	if(USE_FREETYPE)
		set(FREETYPE_INCLUDE_DIR_ft2build "${PROJECT_SOURCE_DIR}/../../freetype2/include/"
				CACHE PATH "freetype include dir")
		set(FREETYPE_INCLUDE_DIR_freetype2 "${PROJECT_SOURCE_DIR}/../../freetype2/include/freetype"
				CACHE PATH "freetype include dir")
		set(FREETYPE_LIBRARY "${PROJECT_SOURCE_DIR}/../../freetype2/objs/win32/vc2005/freetype247.lib"
				CACHE FILEPATH "Path to freetype247.lib")
	endif(USE_FREETYPE)
else()
	# Unix probably
	if(BUILD_CLIENT)
		find_package(X11 REQUIRED)
		find_package(OpenGL REQUIRED)
		find_package(JPEG REQUIRED)
		find_package(BZip2 REQUIRED)
		find_package(PNG REQUIRED)
		if(APPLE)
			FIND_LIBRARY(CARBON_LIB Carbon)
			FIND_LIBRARY(COCOA_LIB Cocoa)
			FIND_LIBRARY(IOKIT_LIB IOKit)
			FIND_LIBRARY(COREFOUNDATION_LIB CoreFoundation)
			mark_as_advanced(
				CARBON_LIB
				COCOA_LIB
				IOKIT_LIB
				COREFOUNDATION_LIB
			)
			SET(CLIENT_PLATFORM_LIBS ${CLIENT_PLATFORM_LIBS} ${CARBON_LIB} ${COCOA_LIB} ${IOKIT_LIB} ${COREFOUNDATION_LIB})
			SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -framework Cocoa")
		endif(APPLE)
	endif(BUILD_CLIENT)
	find_package(ZLIB REQUIRED)
	set(PLATFORM_LIBS -lpthread ${CMAKE_DL_LIBS})
	#set(CLIENT_PLATFORM_LIBS -lXxf86vm)
	# This way Xxf86vm is found on OpenBSD too
	find_library(XXF86VM_LIBRARY Xxf86vm)
	set(CLIENT_PLATFORM_LIBS ${CLIENT_PLATFORM_LIBS} ${XXF86VM_LIBRARY})
endif()

find_package(Jthread REQUIRED)
find_package(Sqlite3 REQUIRED)

if(USE_FREETYPE)
	if(UNIX)
		include(FindPkgConfig)
		if(PKG_CONFIG_FOUND)
			pkg_check_modules(FREETYPE QUIET freetype2)
			if(FREETYPE_FOUND)
				SET(FREETYPE_PKGCONFIG_FOUND TRUE)
				SET(FREETYPE_LIBRARY ${FREETYPE_LIBRARIES})
				# because cmake is idiotic
				string(REPLACE ";" " " FREETYPE_CFLAGS_STR ${FREETYPE_CFLAGS})
				string(REPLACE ";" " " FREETYPE_LDFLAGS_STR ${FREETYPE_LDFLAGS})
			endif(FREETYPE_FOUND)
		endif(PKG_CONFIG_FOUND)
	endif(UNIX)
	if(NOT FREETYPE_FOUND)
		find_package(Freetype REQUIRED)
	endif(NOT FREETYPE_FOUND)
	set(CGUITTFONT_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cguittfont")
	set(CGUITTFONT_LIBRARY cguittfont)
endif(USE_FREETYPE)

configure_file(
	"${PROJECT_SOURCE_DIR}/cmake_config.h.in"
	"${PROJECT_BINARY_DIR}/cmake_config.h"
)

set(common_SRCS
	log.cpp
	content_sao.cpp
	mapgen.cpp
	content_nodemeta.cpp
	content_craft.cpp
	content_craftitem.cpp
	content_clothesitem.cpp
	content_toolitem.cpp
	content_mob.cpp
	content_mapnode.cpp
	content_mapnode_door.cpp
	content_mapnode_farm.cpp
	content_mapnode_circuit.cpp
	content_mapnode_furniture.cpp
	content_mapnode_stair.cpp
	content_mapnode_slab.cpp
	content_mapnode_special.cpp
	content_mapnode_plants.cpp
	content_mapnode_util.cpp
	content_list.cpp
	content_nodebox.cpp
	intl.cpp
	auth.cpp
	collision.cpp
	nodemetadata.cpp
	serverobject.cpp
	noise.cpp
	mineral.cpp
	enchantment.cpp
	porting.cpp
	defaultsettings.cpp
	mapnode.cpp
	voxel.cpp
	inventory.cpp
	debug.cpp
	serialization.cpp
	light.cpp
	filesys.cpp
	connection.cpp
	environment.cpp
	plantgrowth.cpp
	server.cpp
	servercommand.cpp
	socket.cpp
	mapblock.cpp
	mapsector.cpp
	map.cpp
	player.cpp
	utility.cpp
	test.cpp
	sha1.cpp
	base64.cpp
	ban.cpp
	http.cpp
	path.cpp
)

# This gives us the icon
if(WIN32)
	if(MINGW)
		ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/winresource_rc.o
			COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR}
			-i${CMAKE_CURRENT_SOURCE_DIR}/winresource.rc
			-o ${CMAKE_CURRENT_BINARY_DIR}/winresource_rc.o)
		SET(common_SRCS ${common_SRCS} ${CMAKE_CURRENT_BINARY_DIR}/winresource_rc.o)
	else(MINGW) # Probably MSVC
		set(common_SRCS ${common_SRCS} winresource.rc)
	endif(MINGW)
endif()

set(voxelands_SRCS
	${common_SRCS}
	${audio_SRCS}
	sky.cpp
	hud.cpp
	content_mapblock.cpp
	content_cao.cpp
	mapblock_mesh.cpp
	selection_mesh.cpp
	keycode.cpp
	camera.cpp
	clouds.cpp
	particles.cpp
	clientobject.cpp
	guiMainMenu.cpp
	guiSettingsMenu.cpp
	guiMessageMenu.cpp
	guiTextInputMenu.cpp
	guiFormSpecMenu.cpp
	guiPauseMenu.cpp
	guiPasswordChange.cpp
	guiDeathScreen.cpp
	character_creator.cpp
	mesh.cpp
	client.cpp
	tile.cpp
	game.cpp
	main.cpp
)

# Server sources
set(voxelandsserver_SRCS
	${common_SRCS}
	${audio_SRCS}
	servermain.cpp
)

include_directories(
	${CMAKE_BINARY_DIR}/src/jthread
	${PROJECT_BINARY_DIR}
	${IRRLICHT_INCLUDE_DIR}
	${ZLIB_INCLUDE_DIR}
	${CMAKE_BUILD_TYPE}
	${PNG_INCLUDE_DIR}
	${AUDIO_INCLUDE_DIRS}
	${JTHREAD_INCLUDE_DIR}
	${SQLITE3_INCLUDE_DIR}
)

if(USE_FREETYPE)
	set(voxelands_SRCS
		${voxelands_SRCS}
		intlGUIEditBox.cpp
	)
	include_directories(
		${FREETYPE_INCLUDE_DIRS}
		${CGUITTFONT_INCLUDE_DIR}
	)
endif(USE_FREETYPE)

set(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}/bin")

if(BUILD_CLIENT)
	add_executable(${PROJECT_NAME} ${voxelands_SRCS})
	target_link_libraries(
		${PROJECT_NAME}
		${ZLIB_LIBRARIES}
		${IRRLICHT_LIBRARY}
		${OPENGL_LIBRARIES}
		${JPEG_LIBRARIES}
		${BZIP2_LIBRARIES}
		${PNG_LIBRARIES}
		${X11_LIBRARIES}
		${AUDIO_LIBRARIES}
		${JTHREAD_LIBRARY}
		${SQLITE3_LIBRARY}
		${PLATFORM_LIBS}
		${CLIENT_PLATFORM_LIBS}
	)
	if(USE_FREETYPE)
		if(FREETYPE_PKGCONFIG_FOUND)
			set_target_properties(${PROJECT_NAME}
				PROPERTIES
				COMPILE_FLAGS "${FREETYPE_CFLAGS_STR}"
			)
		endif(FREETYPE_PKGCONFIG_FOUND)
		target_link_libraries(
			${PROJECT_NAME}
			${FREETYPE_LIBRARY}
			${CGUITTFONT_LIBRARY}
		)
	endif(USE_FREETYPE)
endif(BUILD_CLIENT)

if(BUILD_SERVER)
	add_executable(${PROJECT_NAME}-server ${voxelandsserver_SRCS})
	target_link_libraries(
		${PROJECT_NAME}-server
		${ZLIB_LIBRARIES}
		${JTHREAD_LIBRARY}
		${SQLITE3_LIBRARY}
		${PLATFORM_LIBS}
	)
endif(BUILD_SERVER)

#
# Set some optimizations and tweaks
#

include(CheckCXXCompilerFlag)

if(MSVC)
	# Visual Studio

	# EHa enables SEH exceptions (used for catching segfaults)
	set(CMAKE_CXX_FLAGS_RELEASE "/EHa /O2 /Ob2 /Oi /Ot /Oy /GL /FD /MT /GS- /arch:SSE /fp:fast /D NDEBUG /D _HAS_ITERATOR_DEBUGGING=0 /TP")
	#set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /NODEFAULTLIB:\"libcmtd.lib\" /NODEFAULTLIB:\"libcmt.lib\"")
	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "/LTCG /SAFESEH:NO")

	# Debug build doesn't catch exceptions by itself
	# Add some optimizations because otherwise it's VERY slow
	set(CMAKE_CXX_FLAGS_DEBUG "/MDd /Zi /Ob0 /Od /RTC1")

	# Flags for C files (sqlite)
	# /MT = Link statically with standard library stuff
	set(CMAKE_C_FLAGS_RELEASE "/O2 /Ob2 /MT")

	if(BUILD_SERVER)
		set_target_properties(${PROJECT_NAME}-server PROPERTIES
				COMPILE_DEFINITIONS "SERVER")
	endif(BUILD_SERVER)

else()
	if(APPLE)
		set(CMAKE_OSX_ARCHITECTURES x86_64 FORCE)
		set(ARCH x86_64)
	endif()

	CHECK_CXX_COMPILER_FLAG("-fstack-protector-strong" HAS_FSTACK_STRONG)
	if(HAS_FSTACK_STRONG)
		set(SAFETY_FLAGS "-fno-strict-aliasing -fwrapv -fvisibility=hidden -fstack-protector-strong --param=ssp-buffer-size=4")
	else()
		set(SAFETY_FLAGS "-fno-strict-aliasing -fwrapv -fvisibility=hidden -fstack-protector --param=ssp-buffer-size=4")
	endif()

	if(CPU_OPTS)
		set(OPT_FLAGS "-march=native -mtune=native -O2 -fomit-frame-pointer")
	else()
		set(OPT_FLAGS "-O2 -fomit-frame-pointer")
	endif()

	set(CMAKE_CXX_FLAGS_RELEASE "${OPT_FLAGS} ${SAFETY_FLAGS} -Wall -DNDEBUG -pipe")
	set(CMAKE_CXX_FLAGS_DEBUG "${SAFETY_FLAGS} -Wall -O0 -g2 -ggdb")
	set(CMAKE_C_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
	set(CMAKE_C_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

	set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-Wl,-O1,--as-needed,--discard-all,--no-undefined,-s,--sort-common,-z,relro,-z,now")
	set(CMAKE_MODULE_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS_RELEASE})
	set(CMAKE_SHARED_LINKER_FLAGS_RELEASE ${CMAKE_EXE_LINKER_FLAGS_RELEASE})

	if(USE_GPROF)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -pg")
	endif()

	if(BUILD_SERVER)
		set_target_properties(${PROJECT_NAME}-server PROPERTIES
				COMPILE_DEFINITIONS "SERVER")
	endif(BUILD_SERVER)

	if(CMAKE_BUILD_TYPE STREQUAL Release)
		MESSAGE(STATUS "C/CXX Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
		MESSAGE(STATUS "Linker Flags: ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
	endif()
   	if(CMAKE_BUILD_TYPE STREQUAL Debug)
		MESSAGE(STATUS "C/CXX Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
		MESSAGE(STATUS "Linker Flags: ${CMAKE_EXE_LINKER_FLAGS} ${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
	endif()
endif()
#
# Installation
#

# Example configuration file
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/../voxelands.conf.example" DESTINATION ${EXAMPLE_CONF_DIR})

if(BUILD_CLIENT)
	install(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})

	file(GLOB images "${CMAKE_CURRENT_SOURCE_DIR}/../data/textures/*.png")
	install(FILES ${images} DESTINATION ${SHAREDIR}/textures)
	file(GLOB skins "${CMAKE_CURRENT_SOURCE_DIR}/../data/textures/skins/*.png")
	install(FILES ${skins} DESTINATION ${SHAREDIR}/textures/skins)
	file(GLOB models "${CMAKE_CURRENT_SOURCE_DIR}/../data/models/*.b3d")
	install(FILES ${models} DESTINATION ${SHAREDIR}/models)
	file(GLOB modelsx "${CMAKE_CURRENT_SOURCE_DIR}/../data/models/*.x")
	install(FILES ${modelsx} DESTINATION ${SHAREDIR}/models)
	file(GLOB sounds "${CMAKE_CURRENT_SOURCE_DIR}/../data/sounds/*.ogg")
	install(FILES ${sounds} DESTINATION ${SHAREDIR}/sounds)
	file(GLOB fonts "${CMAKE_CURRENT_SOURCE_DIR}/../data/fonts/*.ttf")
	install(FILES ${fonts} DESTINATION ${SHAREDIR}/fonts)

	if(USE_GETTEXT)
		foreach(LOCALE ${GETTEXT_AVAILABLE_LOCALES})
			set_mo_paths(MO_BUILD_PATH MO_DEST_PATH ${LOCALE})
			set(MO_BUILD_PATH "${MO_BUILD_PATH}/${PROJECT_NAME}.mo")
			install(FILES ${MO_BUILD_PATH} DESTINATION ${MO_DEST_PATH})
		endforeach(LOCALE ${GETTEXT_AVAILABLE_LOCALES})
	endif()

	if(WIN32)
		if(DEFINED IRRLICHT_DLL)
			install(FILES ${IRRLICHT_DLL} DESTINATION ${BINDIR})
		endif()
		if(DEFINED ZLIB_DLL)
			install(FILES ${ZLIB_DLL} DESTINATION ${BINDIR})
		endif()
		if(FREETYPE_DLL)
			install(FILES ${FREETYPE_DLL} DESTINATION ${BINDIR})
		endif()
		if(USE_AUDIO)
			if(DEFINED VORBIS_DLL)
				install(FILES ${VORBIS_DLL} DESTINATION ${BINDIR})
			endif()
			if(DEFINED VORBISFILE_DLL)
				install(FILES ${VORBISFILE_DLL} DESTINATION ${BINDIR})
			endif()
			if(DEFINED OGG_DLL)
				install(FILES ${OGG_DLL} DESTINATION ${BINDIR})
			endif()
			if(DEFINED OPENAL_DLL)
				install(FILES ${OPENAL_DLL} DESTINATION ${BINDIR})
			endif()
		endif(USE_AUDIO)
	endif()
	if(APPLE)
		# TODO: install Irrlicht.framework into app bundle!
		# it should be in Contents/Resources/Frameworks/Irrlicht.framework

		# Compile .xib files
		# should be in Contents/Resources/MainMenu.nib at the end
		set (VoxelandsMac_XIBS
			MainMenu
		)
		# Make sure the 'Resources' Directory is correctly created before we build
		add_custom_command (TARGET ${PROJECT_NAME} PRE_BUILD
			COMMAND mkdir -p ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/Contents/Resources)
		find_program(IBTOOL ibtool HINTS "/usr/bin" "${OSX_DEVELOPER_ROOT}/usr/bin")
        # Make sure the 'Libs' Directory is correctly created before we build
        add_custom_command (TARGET ${PROJECT_NAME} PRE_BUILD
            COMMAND mkdir -p ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/Contents/Resources/libs)
		if (${IBTOOL} STREQUAL "IBTOOL-NOTFOUND")
			message(SEND_ERROR "ibtool can not be found and is needed to compile the .xib files. It should have been installed with the Apple developer tools. The default system paths were searched in addition to ${OSX_DEVELOPER_ROOT}/usr/bin")
		endif()
		# Compile the .xib files using the 'ibtool' program with the destination being the app package
		foreach(xib ${VoxelandsMac_XIBS})
			add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
				COMMAND ${IBTOOL} --errors --warnings --notices --output-format human-readable-text
				--compile ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}/Contents/Resources/${xib}.nib
				${CMAKE_SOURCE_DIR}/misc/mac/${xib}.xib
				COMMENT "Compiling ${CMAKE_SOURCE_DIR}/${xib}.xib")
		endforeach()
        # Make icns from svg, just in case it was updated
        add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_SOURCE_DIR}/misc/mac/makeicns.sh
                COMMENT "making ${PROJECT_NAME}.icns file from ${PROJECT_NAME}.svg")
        # Change to proper version and mac osx deployment version
        add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/misc/mac/makeplist.sh ${VERSION_STRING}
                COMMENT "Change Info.plist according to provided CMAKE variables")
        # Include the linked '.dylib' to bundle inside the app
        add_custom_command (TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_SOURCE_DIR}/misc/mac/makelibbundle.sh ${EXECUTABLE_OUTPUT_PATH}/\${CONFIGURATION}
            COMMENT "Bundling dylibs with the app bundle")
	endif(APPLE)
endif(BUILD_CLIENT)

if(BUILD_SERVER)
	install(TARGETS ${PROJECT_NAME}-server DESTINATION ${BINDIR})
	file(GLOB html "${CMAKE_CURRENT_SOURCE_DIR}/../data/html/*.html")
	install(FILES ${html} DESTINATION ${SHAREDIR}/html)
endif(BUILD_SERVER)

if (USE_GETTEXT)
	set(MO_FILES)

	foreach(LOCALE ${GETTEXT_AVAILABLE_LOCALES})
		set(PO_FILE_PATH "${GETTEXT_PO_PATH}/${LOCALE}/voxelands.po")
		set_mo_paths(MO_BUILD_PATH MO_DEST_PATH ${LOCALE})
		set(MO_FILE_PATH "${MO_BUILD_PATH}/${PROJECT_NAME}.mo")

		add_custom_command(OUTPUT ${MO_BUILD_PATH}
			COMMAND ${CMAKE_COMMAND} -E make_directory ${MO_BUILD_PATH}
			COMMENT "mo-update [${LOCALE}]: Creating locale directory.")

		add_custom_command(
			OUTPUT ${MO_FILE_PATH}
			COMMAND ${GETTEXT_MSGFMT} -o ${MO_FILE_PATH} ${PO_FILE_PATH}
			DEPENDS ${MO_BUILD_PATH} ${PO_FILE_PATH}
			WORKING_DIRECTORY "${GETTEXT_PO_PATH}/${LOCALE}"
			COMMENT "mo-update [${LOCALE}]: Creating mo file."
			)

		set(MO_FILES ${MO_FILES} ${MO_FILE_PATH})
	endforeach(LOCALE ${GETTEXT_AVAILABLE_LOCALES})

	add_custom_target(translations ALL COMMENT "mo update" DEPENDS ${MO_FILES})
endif(USE_GETTEXT)

# Subdirectories

if (JTHREAD_FOUND)
else (JTHREAD_FOUND)
	add_subdirectory(jthread)
endif (JTHREAD_FOUND)

if (BUILD_CLIENT AND USE_FREETYPE)
	add_subdirectory(cguittfont)
endif (BUILD_CLIENT AND USE_FREETYPE)

if (SQLITE3_FOUND)
else (SQLITE3_FOUND)
add_subdirectory(sqlite)
endif (SQLITE3_FOUND)

#end
