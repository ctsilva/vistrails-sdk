
#[[
Copyright (c) 2009-2011 VisTrails, Inc..

This file is part of VisTrails SDK
(see https://github.com/vistrails/sdk).

License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
]]


cmake_minimum_required(VERSION 2.6)
project(VisTrailsSimpleExample)

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
    MARK_AS_ADVANCED(VisTrails_LIBRARY)
    find_library(VisTrails_LIBRARY NAMES VisTrails libVisTrails
                 PATHS "${VisTrails_DIR}/lib")
    set(VisTrails_INCLUDE_DIR ${VisTrails_DIR}/include/vistrails)
    set(VisTrails_LIBRARIES ${VisTrails_LIBRARY})
  endif()
else()
  set(VisTrails_BUILD_GUI OFF)
  add_subdirectory(../../src)
endif()

set(SIMPLE_SRCS
	simple.cpp
)

#This command lists all defined cmake variables 
# get_cmake_property(allvars VARIABLES)
# foreach(v ${allvars})
	# message(${v})
# endforeach(v)

message("VisTrails INCLUDE: ${VisTrails_INCLUDE_DIR}")
message("Boost INCLUDE: ${Boost_INCLUDE_DIR}")
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${VisTrails_INCLUDE_DIR}
	${Boost_INCLUDE_DIR}
)

add_executable(Simple ${SIMPLE_SRCS})

if (VisTrails_SHOULD_BUILD_SDK)
  add_dependencies(Simple VisTrails)
endif()

target_link_libraries( Simple ${VisTrails_LIBRARIES} )

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/test_vt.xml
    COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/test_vt.xml ${CMAKE_CURRENT_BINARY_DIR}/test_vt.xml
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test_vt.xml)
add_custom_target(CopyXML ALL DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/test_vt.xml)

INSTALL(TARGETS Simple
  RUNTIME DESTINATION bin
)
INSTALL(FILES test_vt.xml DESTINATION bin)
