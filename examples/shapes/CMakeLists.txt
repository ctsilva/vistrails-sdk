cmake_minimum_required(VERSION 3.16)
project(VisTrailsShapesExample CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)
find_package(Boost 1.42 REQUIRED)

set(VT_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../..)
set(VT_LIB ${VT_ROOT}/build/libVisTrails.dylib)
set(VT_GUI_LIB ${VT_ROOT}/build/gui/libVisTrailsGUI.dylib)

add_definitions(-DTIXML_USE_STL -D_FILE_OFSET_BITS=64)

set(SHAPES_SRCS
    actions.cpp
    diagramitem.cpp
    diagramscene.cpp
    main.cpp
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
    ${VT_ROOT}/src/tinyxml/tinystr.cpp
    ${VT_ROOT}/src/tinyxml/tinyxml.cpp
    ${VT_ROOT}/src/tinyxml/tinyxmlerror.cpp
    ${VT_ROOT}/src/tinyxml/tinyxmlparser.cpp
)

add_executable(Shapes ${SHAPES_SRCS})

target_include_directories(Shapes PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${VT_ROOT}/src
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(Shapes PRIVATE
    Qt6::Widgets
    ${VT_GUI_LIB}
    ${VT_LIB}
)
