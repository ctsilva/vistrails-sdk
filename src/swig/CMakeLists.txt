#[[
Copyright (c) 2009-2011 VisTrails, Inc..

This file is part of VisTrails SDK
(see https://github.com/vistrails/sdk).

License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
]]
set(SRC_SWIG_PYTHON_CXX 
  # pyAction.cxx 
  # py_global.cxx
)

set(SRC_SWIG_INTERFACE
	Vistrails.i
)

set(SRC_SWIG_INTERFACES	
	Action.i
	ActionFactory.i
	HPropertyIO.i
	Handler.i
	Property.i
	Session.i
	SessionFocus.i
	Storage.i
	Version.i
	VersionTree.i
	VersionTreeFilter.i
	VersionTreeLayout.i
	VersionTreeTraversal.i
	Vistrail.i
	XmlStorage.i
)

add_definitions(-DTIXML_USE_STL -D_FILE_OFSET_BITS=64)

# Set CMake policies for modern SWIG usage
cmake_policy(SET CMP0078 NEW)  # UseSWIG generates standard target names
cmake_policy(SET CMP0086 NEW)  # UseSWIG honors SWIG_MODULE_NAME

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})
set_source_files_properties(${SRC_SWIG_INTERFACE} PROPERTIES CPLUSPLUS ON)
SET_SOURCE_FILES_PROPERTIES(${SRC_SWIG_INTERFACE} PROPERTIES SWIG_FLAGS "-includeall")
set(CMAKE_SWIG_FLAGS "-c++")

if (VisTrails_PYTHON_WRAPPING)
   find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
   include_directories(${Python3_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

   set_property(SOURCE ${SRC_SWIG_INTERFACE} PROPERTY USE_TARGET_INCLUDE_DIRECTORIES TRUE)
   swig_add_library(PyVisTrails
      TYPE MODULE
      LANGUAGE python
      SOURCES ${SRC_SWIG_INTERFACE} ${SRC_SWIG_PYTHON_CXX})
   target_link_libraries(PyVisTrails PRIVATE ${Python3_LIBRARIES} VisTrails)
endif (VisTrails_PYTHON_WRAPPING)

if (VisTrails_JAVA_WRAPPING)
   # project(JavaVisTrails)
   find_package(JNI)
   find_package(Java)
   include_directories(${JAVA_INCLUDE_PATH} ${JNI_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})
   set(JAVA_PACKAGE_NAME "com.vistrails.sdk")
   set(JAVA_PACKAGE_DIR "com/vistrails/sdk")
   message(STATUS "JavaVisTrails_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR})
   
   set(CMAKE_SWIG_FLAGS ${CMAKE_SWIG_FLAGS} -package ${JAVA_PACKAGE_NAME})
   swig_add_module(JavaVisTrails java ${SRC_SWIG_INTERFACE})
   swig_link_libraries(JavaVisTrails ${JNI_LIBRARIES} VisTrails)
   
   ADD_CUSTOM_COMMAND(TARGET JavaVisTrails
                     POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E echo "Compiling Java files..."
                     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/${JAVA_PACKAGE_DIR}
                     COMMAND ${Java_JAVAC_EXECUTABLE} *.java -d ${CMAKE_CURRENT_BINARY_DIR}
                     COMMAND ${CMAKE_COMMAND} -E echo "Creating jar file..."
                     COMMAND ${Java_JAR_EXECUTABLE} cvf vistrails.jar com
   )
endif (VisTrails_JAVA_WRAPPING)
