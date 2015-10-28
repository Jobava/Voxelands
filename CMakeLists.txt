cmake_minimum_required(VERSION 2.6)
if(${CMAKE_VERSION} STREQUAL "2.8.2")
  # bug http://vtk.org/Bug/view.php?id=11020
  message( WARNING "CMake/CPack version 2.8.2 will not create working .deb packages!")
endif(${CMAKE_VERSION} STREQUAL "2.8.2")

# This can be read from ${PROJECT_NAME} after project() is called
project(voxelands)

# Also remember to set PROTOCOL_VERSION in clientserver.h when releasing
set(VERSION_MAJOR 1510)
set(VERSION_MINOR 00)
set(VERSION_EXTRA ":next")
set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}${VERSION_EXTRA}")

MESSAGE(STATUS "*** Will build version ${VERSION_STRING} ***")

# Configuration options

if(WIN32)
	set(RUN_IN_PLACE 1 CACHE BOOL "Run directly in source directory structure")
else()
	set(RUN_IN_PLACE 0 CACHE BOOL "Run directly in source directory structure")
endif()

set(BUILD_CLIENT 1 CACHE BOOL "Build client")
if(WIN32)
	set(BUILD_SERVER 0 CACHE BOOL "Build server")
else()
	set(BUILD_SERVER 1 CACHE BOOL "Build server")
endif()

set(CPU_OPTS 0 CACHE BOOL "Enable CPU optimizations")

if(NOT CMAKE_BUILD_TYPE)
	# Default to release
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: Debug or Release" FORCE)
endif()

# Included stuff
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
include(${CMAKE_SOURCE_DIR}/cmake/Modules/misc.cmake)

# This is done here so that relative search paths are more reasnable
find_package(Irrlicht)

#
# Installation
#

if(WIN32)
    set(SHAREDIR "data")
	set(BINDIR "bin")
	set(DOCDIR "doc")
	set(EXAMPLE_CONF_DIR ".")
elseif(APPLE)
	# random placeholders
    set(SHAREDIR "share/${PROJECT_NAME}")
	set(BINDIR "bin")
	set(DOCDIR "share/doc/${PROJECT_NAME}")
    set(EXAMPLE_CONF_DIR ${DOCDIR})
elseif(UNIX) # Linux, BSD etc
    if(RUN_IN_PLACE)
        set(SHAREDIR "share/${PROJECT_NAME}")
	    set(BINDIR "bin")
	    set(DOCDIR "share/doc/${PROJECT_NAME}")
        set(MANDIR "share/man")
        set(EXAMPLE_CONF_DIR ${DOCDIR})
        set(XDG_APPS_DIR "share/applications")
        set(ICONDIR "share/icons")
        set(LOCALEDIR "share/${PROJECT_NAME}/locale")
    else()
        set(SHAREDIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}")
        set(BINDIR "${CMAKE_INSTALL_PREFIX}/bin")
        set(DOCDIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}")
        set(MANDIR "${CMAKE_INSTALL_PREFIX}/share/man")
        set(EXAMPLE_CONF_DIR ${DOCDIR})
        set(XDG_APPS_DIR "${CMAKE_INSTALL_PREFIX}/share/applications")
        set(ICONDIR "${CMAKE_INSTALL_PREFIX}/share/icons")
        set(LOCALEDIR "${CMAKE_INSTALL_PREFIX}/share/${PROJECT_NAME}/locale")
    endif()
endif()

install(FILES "README.txt" DESTINATION "${DOCDIR}")
install(FILES "doc/changelog.txt" DESTINATION "${DOCDIR}")
install(FILES "voxelands.conf.example" DESTINATION "${DOCDIR}")
if (APPLE)
    install(DIRECTORY "libs/" DESTINATION "libs")
endif()
if (UNIX AND NOT APPLE)
    install(FILES "doc/voxelands.6" "doc/voxelands-server.6" DESTINATION "${MANDIR}/man6")
    install(FILES "misc/freedesktop/voxelands.desktop" DESTINATION "${XDG_APPS_DIR}")
    install(FILES "misc/freedesktop/icons/scalable/apps/voxelands.svg" DESTINATION "${ICONDIR}/hicolor/scalable/apps")
    install(FILES "misc/freedesktop/icons/192x192/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/192x192/apps")
    install(FILES "misc/freedesktop/icons/128x128/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/128x128/apps")
    install(FILES "misc/freedesktop/icons/64x64/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/64x64/apps")
    install(FILES "misc/freedesktop/icons/48x48/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/48x48/apps")
    install(FILES "misc/freedesktop/icons/32x32/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/32x32/apps")
    install(FILES "misc/freedesktop/icons/24x24/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/24x24/apps")
    install(FILES "misc/freedesktop/icons/22x22/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/22x22/apps")
    install(FILES "misc/freedesktop/icons/16x16/apps/voxelands.png" DESTINATION "${ICONDIR}/hicolor/16x16/apps")
endif()
#
# Subdirectories
# Be sure to add all relevant definitions above this
#

add_subdirectory(src)

# CPack

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The Fun-Focused Free Software Voxel World Game")
set(CPACK_PACKAGE_VERSION_MAJOR ${VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${VERSION_PATCH})
set(CPACK_PACKAGE_VENDOR "darkrose")
set(CPACK_PACKAGE_CONTACT "Lisa 'darkrose' Milne <lisa@ltmnet.com>")

if(WIN32)
	# For some reason these aren't copied otherwise
	# NOTE: For some reason now it seems to work without these
	#if(BUILD_CLIENT)
	#	install(FILES bin/voxelands.exe DESTINATION bin)
	#endif()
	#if(BUILD_SERVER)
	#	install(FILES bin/voxelands-server.exe DESTINATION bin)
	#endif()

	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION_STRING}-win32")

	set(CPACK_GENERATOR ZIP)

	# This might be needed for some installer
	#set(CPACK_PACKAGE_EXECUTABLES bin/voxelands.exe "Voxelands" bin/voxelands-server.exe "Voxelands Server")
elseif(APPLE)
	# TODO
	# see http://cmake.org/Wiki/CMake:CPackPackageGenerators#Bundle_.28OSX_only.29
	#
    set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION_STRING}-osx-${CMAKE_OSX_ARCHITECTURES}")
	set(CPACK_PACKAGE_ICON ${CMAKE_CURRENT_SOURCE_DIR}/misc/mac/voxelands.icns)
	set(CPACK_BUNDLE_NAME ${PROJECT_NAME})
	set(CPACK_BUNDLE_ICON ${CPACK_PACKAGE_ICON})
	set(CPACK_BUNDLE_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/misc/mac/Info.plist)
	set(CPACK_BUNDLE_STARTUP_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/bin/Debug/voxelands)
	set(CPACK_GENERATOR "Bundle")
else()
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${VERSION_STRING}-linux")
	set(CPACK_GENERATOR TGZ)
	set(CPACK_SOURCE_GENERATOR TGZ)
	SET ( CPACK_DEBIAN_PACKAGE_SECTION		"other" )
	SET ( CPACK_DEBIAN_PACKAGE_PRIORITY		"optional" )
	SET ( CPACK_DEBIAN_PACKAGE_MAINTAINER	"Lisa Milne <lisa@ltmnet.com>" )
	SET ( CPACK_GENERATOR					"DEB;RPM" )

endif()

include(CPack)

