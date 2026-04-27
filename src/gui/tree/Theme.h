/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

// This module describes a theme structure for VisTrails GUI. It
// specifies colors, background images and other measurements

#ifndef __THEME_H___
#define __THEME_H___

#include <QColor>
#include <QCursor>
#include <QFont>
#include <QFontMetrics>
#include <QIcon>
#include <QPen>
#include <QBrush>
#include <QLinearGradient>
#include "Color.h"

// This is the default theme which contains color, images,
// measurements, etc. for Vistrail. Other themes should derive from
// this class and change appropriate values
class DefaultTheme
{
public:
  // This is for initializing all Qt objects
  DefaultTheme();

  //////////////////////////////
  //////// MEASUREMENTS ////////


   // Padded space of Version shape and its label
  int VERSION_LABEL_MARGIN[2];

  // Padded space of Module shape into its label
  int MODULE_LABEL_MARGIN[4];

  // Space between ports inside a module
  int MODULE_PORT_SPACE;

  // The space added to the end of port shapes before it reaches the
  // margin of the module
  int MODULE_PORT_PADDED_SPACE;

  // Width and Height of Port shape
  int PORT_WIDTH;
  int PORT_HEIGHT;
  QRectF PORT_RECT;

  // Width and Height of Configure button shape
  int CONFIGURE_WIDTH;
  int CONFIGURE_HEIGHT;

  QPolygonF CONFIGURE_SHAPE;

  // The number of control points when drawing connection curve
  int CONNECTION_CONTROL_POINTS;

  // Control the size and gap for the 3 little segments when
  // draw connections between versions
  int LINK_SEGMENT_LENGTH;
  int LINK_SEGMENT_GAP;
  int LINK_SEGMENT_SQUARE_LENGTH;

  // The size of the frame containing the PIP graphics view
  int PIP_IN_FRAME_WIDTH;
  int PIP_OUT_FRAME_WIDTH;

  // The size of the frame containing the PIP graphics view
  int PIP_DEFAULT_SIZE[2];

  // The default minimum size of the graphics views
  int BOUNDING_RECT_MINIMUM;

  // Default Paramter Inspector Window dimension
  int VISUAL_DIFF_PARAMETER_WINDOW_SIZE[2];

  // Default legend size (small rectangular shape)
  int VISUAL_DIFF_LEGEND_SIZE[2];

  // Virtual Cell Label default  size
  int VIRTUAL_CELL_LABEL_SIZE[2];

  // Query Preview Size
  int QUERY_PREVIEW_SIZE[2];

  /////////////////////////////
  //////// BRUSH & PEN ////////

  // Background brush of the pipeline view
  QBrush PIPELINE_VIEW_BACKGROUND_BRUSH;
  
  // Background brush of the version tree
  QBrush VERSION_TREE_BACKGROUND_BRUSH;
  
  // Background brush of the query pipeline view
  QBrush QUERY_BACKGROUND_BRUSH;

  // Pen to draw a module shape at regular state
  QPen MODULE_PEN;
  
  // Pen to draw a module shape when selected
  QPen MODULE_SELECTED_PEN;
  
  // Brush and pen to draw a module label
  QPen MODULE_LABEL_PEN;
  QPen MODULE_LABEL_SELECTED_PEN;
  
  // Pen to draw module label when it is unmatched due to a query
  QPen GHOSTED_MODULE_LABEL_PEN;
  
  // Brush to draw a module shape at different states
  QBrush MODULE_BRUSH;
  QBrush ERROR_MODULE_BRUSH;
  QBrush SUCCESS_MODULE_BRUSH;
  QBrush ACTIVE_MODULE_BRUSH;
  QBrush COMPUTING_MODULE_BRUSH;
  QBrush NOT_EXECUTED_MODULE_BRUSH;

  // Pen and brush for un-matched queried modules
  QPen GHOSTED_MODULE_PEN;
  QBrush GHOSTED_MODULE_BRUSH;

  // Brush and pen to draw a port shape at regular state
  QPen PORT_PEN;
  QBrush PORT_BRUSH;
  QPen PORT_SELECTED_PEN;
  QPen PORT_OPTIONAL_PEN;
  QBrush PORT_OPTIONAL_BRUSH;
  
  // Pen and brush for drawing ports of ghosted modules
  QPen GHOSTED_PORT_PEN;
  QBrush GHOSTED_PORT_BRUSH;

  // Pen and brush for drawing the configure button
  QPen CONFIGURE_PEN;
  QBrush CONFIGURE_BRUSH;

  // Pen and brush for drawing the ghosted configure button
  QPen GHOSTED_CONFIGURE_PEN;
  QBrush GHOSTED_CONFIGURE_BRUSH;

  // Brush and pen to draw connections
  QPen CONNECTION_PEN;
  QPen CONNECTION_SELECTED_PEN;
  QBrush CONNECTION_BRUSH;

  // Pen for drawing while connecting any ghosted modules
  QPen GHOSTED_CONNECTION_PEN;

  // Pen to draw version tree node
  QPen VERSION_PEN;
  QPen GHOSTED_VERSION_PEN;
  QPen VERSION_SELECTED_PEN;
  QPen VERSION_FILTER_PEN;
  QPen VERSION_SELECTED_FILTER_PEN;
  QPen VERSION_PLAYBACK_START_PEN;
  QPen VERSION_PLAYBACK_END_PEN;

  // Brush and pen to draw a version label
  QPen VERSION_LABEL_PEN;
  QPen GHOSTED_VERSION_LABEL_PEN;
  QPen VERSION_LABEL_SELECTED_PEN;

  // Brush to draw version belongs to the current user
  QBrush VERSION_USER_BRUSH;
  QBrush GHOSTED_VERSION_USER_BRUSH;

  // Brush to draw version belongs to the other users
  QBrush VERSION_OTHER_BRUSH;

  // Brush and pen to draw a link between two versions
  QPen LINK_PEN;
  QPen LINK_SELECTED_PEN;
  QPen GHOSTED_LINK_PEN;

  // Selection box color
  QBrush SELECTION_BOX_BRUSH;
  QPen SELECTION_BOX_PEN;
  
  // Color of the version is being diff from in
  QBrush VISUAL_DIFF_FROM_VERSION_BRUSH;
  
  // Color of the version is being diff to in
  QBrush VISUAL_DIFF_TO_VERSION_BRUSH;
  
  // Color of the paramter changed modules in Visual Diff
  QBrush VISUAL_DIFF_PARAMETER_CHANGED_BRUSH;
  
  // Color of the shared modules in Visual Diff
  QBrush VISUAL_DIFF_SHARED_BRUSH;

  // Pen & Brush of the circled id on the right corner of the
  // virtual cell label
  QPen ANNOTATED_ID_PEN;
  QBrush ANNOTATED_ID_BRUSH;

  //////// FONTS ////////        
  // Font for module text
  QFont MODULE_FONT;
  QFont MODULE_DESC_FONT;

  // Font for version text
  QFont VERSION_FONT;
  QFont VERSION_DESCRIPTION_FONT;
  QFont VERSION_PROPERTIES_FONT;
  QFontMetrics *VERSION_FONT_METRICS;
  QFontMetrics *VERSION_DESCRIPTION_FONT_METRICS;
  QBrush VERSION_PROPERTIES_PEN;

  // Font showing on the Parameter Inspector window of Visual Diff
  QFont VISUAL_DIFF_PARAMETER_FONT;
  
  // Font showing on the Legend window of Visual Diff
  QFont VISUAL_DIFF_LEGEND_FONT;


  //////// ICONS & IMAGES ////////
  //The application disclaimer image
  QPixmap DISCLAIMER_IMAGE;
  
  //The application icon
  QIcon APPLICATION_ICON;

  //The application pixmap
  QPixmap APPLICATION_PIXMAP;

  // The execute icons in the first spot of vistrail view toolbar
  QIcon EXECUTE_PIPELINE_ICON;
  QIcon EXECUTE_EXPLORE_ICON;

  // The undo icons for the vistrail view toolbar
  QIcon UNDO_ICON;

  // The redo icons for the vistrail view toolbar
  QIcon REDO_ICON;

  // Icon to select the tabbed view
  QIcon TABBED_VIEW_ICON;

  // Icon to select the horizontal split view
  QIcon HORIZONTAL_VIEW_ICON;

  // Icon to select the vertical split view
  QIcon VERTICAL_VIEW_ICON;

  // Icon to select the docking-style view
  QIcon DOCK_VIEW_ICON;

  // Toolbar icon for creating a new Vistrail
  QIcon NEW_VISTRAIL_ICON;
  
  // Toolbar icon for opening a vistrail
  QIcon OPEN_VISTRAIL_ICON;

  //Toolbar icon for opening a vistrail from a database
  QIcon OPEN_VISTRAIL_DB_ICON;

  //Icon for database connections
  QIcon DB_ICON;

  //Icon for vistrails files
  QIcon FILE_ICON;
  
  // Toolbar icon for save the current Vistrail
  QIcon SAVE_VISTRAIL_ICON;

  // Toolbar icon for toggling console mode window
  QIcon CONSOLE_MODE_ICON;

  // Toolbar icon for toggling bookmarks window
  QIcon BOOKMARKS_ICON;
  
  // BOOKMARKS WINDOW
  //Toolbar icon for removing bookmark
  QIcon BOOKMARKS_REMOVE_ICON;

  //Toolbar icon for reloading a bookmark
  QIcon BOOKMARKS_RELOAD_ICON;

  // Background image of the Visual Diff pipeline view
  QImage VISUAL_DIFF_BACKGROUND_IMAGE;
  
  // Toolbar icon for showing the Parameter Inspector window
  QIcon VISUAL_DIFF_SHOW_PARAM_ICON;
  
  // Toolbar icon for showing the Legend window
  QIcon VISUAL_DIFF_SHOW_LEGEND_ICON;

  // Toolbar icon for creating an analogy
  QIcon VISUAL_DIFF_CREATE_ANALOGY_ICON;

  // Toolbar icon for close button on the vistrail tabbar
  QIcon VIEW_MANAGER_CLOSE_ICON;

  // Toolbar icon for the dock toolbutton on the splitted window
  QIcon DOCK_BACK_ICON;

  // Icon for adding string in the parameter exploration widget
  QIcon ADD_STRING_ICON;

  // Icon for moving string up in the parameter exploration widget
  QIcon UP_STRING_ICON;

  // Icon for moving string up in the parameter exploration widget
  QIcon DOWN_STRING_ICON;

  // Toolbar icons for views
  QIcon PIPELINE_ICON;
  QIcon HISTORY_ICON;
  QIcon QUERY_ICON;
  QIcon EXPLORE_ICON;

  // Toolbar icon for visual query on a vistrail
  QIcon VISUAL_QUERY_ICON;
  
  // Toolbar icon for viewing the whole version tree
  QIcon VIEW_FULL_TREE_ICON;

  // Toolbar icon for viewing the whole version tree
  QIcon PERFORM_PARAMETER_EXPLORATION_ICON;

  // Toolbar icon for dragging pixmap of VisDiff
  QPixmap VERSION_DRAG_PIXMAP;

  // Parameter Exploration Pixmaps
  QPixmap EXPLORE_COLUMN_PIXMAP;
  QPixmap EXPLORE_ROW_PIXMAP;
  QPixmap EXPLORE_SHEET_PIXMAP;
  QPixmap EXPLORE_TIME_PIXMAP;
  QPixmap EXPLORE_SKIP_PIXMAP;
  QPixmap REMOVE_PARAM_PIXMAP;
  QPixmap RIGHT_ARROW_PIXMAP;
  
  // Cursor for zoom in/out graphics views
  QCursor SELECT_CURSOR;
  QCursor OPEN_HAND_CURSOR;
  QCursor CLOSE_HAND_CURSOR;
  QCursor ZOOM_CURSOR;
          
  // Cursor icon for zoom in/out graphics views
  QIcon SELECT_ICON;
  QIcon PAN_ICON;
  QIcon ZOOM_ICON;

  // Saved Queries icons
  QIcon QUERY_VIEW_ICON;
  QIcon QUERY_ARROW_ICON;
  QIcon QUERY_EDIT_ICON;

  //////// COLORS ////////
  // Color for the PIP frame
  QColor PIP_FRAME_COLOR;

  // Color of selected methods in the modules method area
  QColor METHOD_SELECT_COLOR;

  // Color of the hover/unhover alias labels
  QColor HOVER_DEFAULT_COLOR;
  QColor HOVER_SELECT_COLOR;
};

class MacTheme: public DefaultTheme
{
public:
  MacTheme();
};
        
// ThemeHolder is a class holding a theme and exposed that theme
// atrributes as attributes of itself. This is useful for global
// import of the CurrentTheme variable
class ThemeHolder
{
public:
  // Set the current theme to theme
  static void setTheme(DefaultTheme *theme);
  static DefaultTheme *getCurrentTheme();

private:
  static DefaultTheme *theme;
};

#endif
