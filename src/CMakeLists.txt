
#[[
Copyright (c) 2009-2011 VisTrails, Inc..

This file is part of VisTrails SDK
(see https://github.com/vistrails/sdk).

License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
]]


cmake_minimum_required(VERSION 3.5)
project(VisTrails)
set(VisTrails_VERSION_MAJOR 0)
set(VisTrails_VERSION_MAJOR 1)

if(APPLE)
  find_package(Boost 1.42) 
  # MacOS X doesn't seem to need the compiled stuff
else()
  find_package(Boost 1.42 COMPONENTS date_time REQUIRED)
  set(Boost_USE_STATIC_LIBS ON)
endif()

add_definitions(-DTIXML_USE_STL -D_FILE_OFSET_BITS=64)

set(SRC_TINYXML
tinyxml/tinystr.cpp
tinyxml/tinyxml.cpp
tinyxml/tinyxmlerror.cpp
tinyxml/tinyxmlparser.cpp
tinyxml/tinystr.h
tinyxml/tinyxml.h
)

set(SRC_CORE
core/Action.cpp
core/ActionFactory.cpp
core/Handler.cpp
core/Property.cpp
core/Session.cpp
core/SessionFocus.cpp
core/Storage.cpp
core/Version.cpp
core/VersionTree.cpp
core/VersionTreeFilter.cpp
core/VersionTreeLayout.cpp
core/VersionTreeTraversal.cpp
core/Vistrail.cpp
core/XmlStorage.cpp
core/global.cpp
)

set(SRC_CORE_HDRS
core/Action.h
core/ActionFactory.h
core/Handler.h
core/Property.h
core/Session.h
core/SessionFocus.h
core/Storage.h
core/VersionTreeFilter.h
core/Vistrail.h
core/Version.h
core/VersionTreeLayout.h
core/VersionTree.h
core/VersionTreeTraversal.h
core/XmlStorage.h
core/global.h
core/global_private.h
)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
  add_library(VisTrails SHARED ${SRC_TINYXML} ${SRC_DB} ${SRC_CORE} ${SRC_CORE_HDRS})
  target_link_libraries(VisTrails ${Boost_LIBRARIES})
  # set_target_properties(VisTrails PROPERTIES INSTALL_NAME_DIR "@loader_path")

  #only provide these options if building outside of paraview
  if(NOT DEFINED PARAVIEW_BUILD_QT_GUI)
    set(VisTrails_PYTHON_WRAPPING OFF CACHE BOOL "Enable building Python wrapping of the library using SWIG")
    set(VisTrails_JAVA_WRAPPING OFF CACHE BOOL "Enable building Java wrapping of the library using SWIG")
    if (VisTrails_PYTHON_WRAPPING OR VisTrails_JAVA_WRAPPING)
      add_subdirectory(swig)
    endif()
  endif()

  set(VisTrails_BUILD_GUI OFF CACHE BOOL "Enable building gui components")
  if (VisTrails_BUILD_GUI)
    add_subdirectory(gui)
    set(VisTrails_LIBRARIES VisTrails VisTrailsGUI CACHE INTERNAL "libraries" FORCE)
  else()
    set(VisTrails_LIBRARIES VisTrails CACHE INTERNAL "libraries" FORCE) 
  endif()

  set(VisTrails_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE INTERNAL "include directories" FORCE)
 
  INSTALL(FILES VisTrails.h DESTINATION include/vistrails)
  INSTALL(FILES ${SRC_CORE_HDRS} DESTINATION include/vistrails/core)
  INSTALL(TARGETS VisTrails
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
  )
endif()
