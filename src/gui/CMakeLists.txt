
#[[
Copyright (c) 2009-2011 VisTrails, Inc..

This file is part of VisTrails SDK
(see https://github.com/vistrails/sdk).

License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
]]


cmake_minimum_required(VERSION 3.5)
project(VisTrailsGUI)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set( SRC_GUI_SRCS
#appmodel.cpp
delegate.cpp
galleryimageholder.cpp
gallerylabel.cpp
galleryproxy.cpp
galleryview.cpp
imageviewer.cpp
preferencesdialog.cpp
imagevisualizer.cpp
vtmodel.cpp
vtnode.cpp
vtsortfilterproxy.cpp
vttableproxy.cpp
vtwindow.cpp
versionedit.cpp
)

set( SRC_GUI_HDRS
info.hpp
delegate.hpp
galleryimageholder.hpp
gallerylabel.hpp
galleryproxy.hpp
galleryview.hpp
imageviewer.hpp
preferencesdialog.hpp
imagevisualizer.hpp
aspectpreservinglabel.hpp
vtmodel.hpp
vtnode.hpp
vtsortfilterproxy.hpp
vttableproxy.hpp
vtwindow.hpp
versionedit.hpp
)

set( SRC_UI
gallerymainview.ui
imagecontainer.ui
imageviewer.ui
preferencesdialog.ui
vistrailsui.ui
imagevisualizer.ui
about.ui
)

set( SRC_TREE_HDRS
tree/Color.h
tree/GraphicsView.h
tree/Theme.h
tree/VersionView.h
tree/VistrailController.h
tree/CommonWidgets.h
tree/VersionProp.h
)

set( SRC_TREE
tree/Color.cpp
tree/GraphicsView.cpp
tree/Theme.cpp
tree/VersionView.cpp
tree/VistrailController.cpp
tree/CommonWidgets.cpp
tree/VersionProp.cpp
)

# create Visual Studio sub folder
SOURCE_GROUP(Tree FILES ${SRC_TREE} ${SRC_TREE_HDRS})

set( SRC_RC
vistrailsui.qrc
)

#add_definitions (-Wall)

find_package(Qt6 COMPONENTS Widgets PrintSupport Concurrent REQUIRED)

include_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${VT_SRC_DIR}
  ${Boost_INCLUDE_DIR}
)

add_library(VisTrailsGUI SHARED
  ${SRC_GUI_SRCS}
  ${SRC_GUI_HDRS}
  ${SRC_UI}
  ${SRC_RC}
  ${SRC_TREE}
  ${SRC_TREE_HDRS}
)
add_dependencies(VisTrailsGUI VisTrails)

target_link_libraries(VisTrailsGUI Qt6::Widgets Qt6::PrintSupport Qt6::Concurrent VisTrails)
# set_target_properties(VisTrailsGUI PROPERTIES INSTALL_NAME_DIR "@loader_path")

INSTALL(FILES ${SRC_GUI_HDRS} DESTINATION include/vistrails/gui)
INSTALL(FILES ${SRC_TREE_HDRS} DESTINATION include/vistrails/gui/tree)
INSTALL(TARGETS VisTrailsGUI 
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
