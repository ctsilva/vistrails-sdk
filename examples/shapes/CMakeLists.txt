
#[[
Copyright (c) 2009-2011 VisTrails, Inc..

This file is part of VisTrails SDK
(see https://github.com/vistrails/sdk).

License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
]]


cmake_minimum_required(VERSION 2.6)
project(VisTrailsShapesExample)

if(APPLE)
  find_package(Boost 1.42) 
  # MacOS X doesn't seem to need the compiled stuff
else()
  find_package(Boost 1.42 COMPONENTS date_time REQUIRED)
  set(Boost_USE_STATIC_LIBS ON)
endif()

set(VisTrails_SHOULD_BUILD_SDK ON CACHE BOOL "Build the VisTrails SDK")
if (NOT VisTrails_SHOULD_BUILD_SDK)
  set(VisTrails_DIR "/vistrailsinc/src/sdk/install" CACHE DIRECTORY "Path to VisTrails SDK installation")
  if (EXISTS "${VisTrails_DIR}" AND IS_DIRECTORY "${VisTrails_DIR}")
    MARK_AS_ADVANCED(VisTrails_LIBRARY VisTrailsGUI_LIBRARY)
    find_library(VisTrails_LIBRARY NAMES VisTrails libVisTrails
                 PATHS "${VisTrails_DIR}/lib")
    find_library(VisTrailsGUI_LIBRARY NAMES VisTrailsGUI libVisTrailsGUI
                 PATHS "${VisTrails_DIR}/lib")
    set(VisTrails_INCLUDE_DIR ${VisTrails_DIR}/include/vistrails)
    set(VisTrails_LIBRARIES ${VisTrails_LIBRARY} ${VisTrailsGUI_LIBRARY})
  endif()
else()
  set(VisTrails_BUILD_GUI ON)
  add_subdirectory(../../src)
endif()

add_definitions(-DTIXML_USE_STL -D_FILE_OFSET_BITS=64)

set(TINYXML_SRCS
	../../src/tinyxml/tinystr.cpp
	../../src/tinyxml/tinyxml.cpp
	../../src/tinyxml/tinyxmlerror.cpp
	../../src/tinyxml/tinyxmlparser.cpp
)

set(TINYXML_HDRS
	../../src/tinyxml/tinystr.h
	../../src/tinyxml/tinyxml.h
)

set(TINYXML_INCLUDE_DIR ../../src)

set(SHAPES_SRCS
	actions.cpp
	commands.cpp
	diagramitem.cpp 
	diagramscene.cpp
	main.cpp
	# versionscene.cpp
	# versionitem.cpp
	mainwindow.cpp
	ufdb/domainfactory.cpp
	ufdb/xmlfactory.cpp
	ufdb/add_action.cpp
	ufdb/add_action_xml.cpp
	ufdb/move_action.cpp
	ufdb/move_action_xml.cpp
	ufdb/delete_action.cpp
	ufdb/delete_action_xml.cpp
	ufdb/scene_action.cpp
	ufdb/scene_action_xml.cpp
	ufdb/shape.cpp
	ufdb/shape_xml.cpp
)

set(SHAPES_HDRS
	actions.h
	commands.h
	diagramitem.h
	diagramscene.h
	# versionscene.h
	# versionitem.h
	mainwindow.h
	ufdb/domainfactory.h
	ufdb/xmlfactory.h
	ufdb/add_action.h
	ufdb/add_action_xml.h
	ufdb/move_action.h
	ufdb/move_action_xml.h
	ufdb/delete_action.h
	ufdb/delete_action_xml.h
	ufdb/scene_action.h
	ufdb/scene_action_xml.h
	ufdb/shape.h
	ufdb/shape_xml.h
)

# add_subdirectory(quazip)
# get_directory_property(QUAZIP_SRC_DIR DIRECTORY quazip DEFINITION CMAKE_CURRENT_SOURCE_DIR)

#This command lists all defined cmake variables 
# get_cmake_property(allvars VARIABLES)
# foreach(v ${allvars})
	# message(${v})
# endforeach(v)

message("VisTrails INCLUDE: ${VisTrails_INCLUDE_DIR}")
message("Boost INCLUDE: ${Boost_INCLUDE_DIR}")
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${TINYXML_INCLUDE_DIR}
	${VisTrails_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
	# ${VT_SRC_DIR} 
	# ${QUAZIP_SRC_DIR}
	# ${ZLIB_INCLUDE_DIR}
)

find_package( Qt4 REQUIRED )
include( ${QT_USE_FILE} )

QT4_WRAP_CPP(MOC_SRCS
	actions.h
	commands.h
	diagramitem.h
	diagramscene.h
	# versionscene.h
	# versionitem.h
	mainwindow.h)

add_executable(Shapes 
		${SHAPES_SRCS}
		${SHAPES_HDRS}
		${MOC_SRCS}
		${TINYXML_HDRS}
		${TINYXML_SRCS})

if (VisTrails_SHOULD_BUILD_SDK)
  add_dependencies(Shapes VisTrails VisTrailsGUI)
endif()

target_link_libraries( Shapes
		       ${VisTrails_LIBRARIES}
		       ${QT_LIBRARIES}
		       # vtquazip
		       )


INSTALL(TARGETS Shapes
  RUNTIME DESTINATION bin
)
