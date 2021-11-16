/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Theme.h"

DefaultTheme *ThemeHolder::theme = NULL;

inline QPolygonF _create_configure_shape(int w, int h)
{
  QPolygon poly = QPolygon(3);
  poly.setPoint(0, 0, 0);
  poly.setPoint(1, 0, h);
  poly.setPoint(2, w, h/2);
  return QPolygonF(poly);
}


// Instantiates the theme according to the current platform
inline DefaultTheme *get_os_theme()
{
#ifdef __APPLE__
  return new MacTheme();
#else
  return new DefaultTheme();
#endif
}
    
inline void initializeCurrentTheme()
{

}

DefaultTheme::DefaultTheme()
{
  this->VERSION_LABEL_MARGIN[0] = 60;
  this->VERSION_LABEL_MARGIN[1] = 35;
  
  this->MODULE_LABEL_MARGIN[0] = 20;
  this->MODULE_LABEL_MARGIN[1] = 20;
  this->MODULE_LABEL_MARGIN[2] = 20;
  this->MODULE_LABEL_MARGIN[3] = 15;
  
  this->MODULE_LABEL_MARGIN[0] = 4;
  this->MODULE_LABEL_MARGIN[1] = 4;
  this->MODULE_LABEL_MARGIN[2] = 4;
  this->MODULE_LABEL_MARGIN[3] = 4;
  
  this->MODULE_PORT_SPACE = 4;
  this->MODULE_PORT_PADDED_SPACE = 20;
  this->PORT_WIDTH = 10;
  this->PORT_HEIGHT = 10;
  this->PORT_RECT = QRectF(0, 0, this->PORT_WIDTH, this->PORT_HEIGHT);

  this->CONFIGURE_WIDTH = 6;
  this->CONFIGURE_HEIGHT = 10;
  this->CONFIGURE_SHAPE = _create_configure_shape(this->CONFIGURE_WIDTH,
                                                 this->CONFIGURE_HEIGHT);

  this->CONNECTION_CONTROL_POINTS = 20;

  this->LINK_SEGMENT_LENGTH = 15;
  this->LINK_SEGMENT_GAP = 5;
  this->LINK_SEGMENT_SQUARE_LENGTH = 12;

  this->PIP_IN_FRAME_WIDTH = 5;
  this->PIP_OUT_FRAME_WIDTH = 1;
  
  this->PIP_DEFAULT_SIZE[0] = 128;
  this->PIP_DEFAULT_SIZE[1] = 128;

  this->BOUNDING_RECT_MINIMUM = 64;

  this->VISUAL_DIFF_PARAMETER_WINDOW_SIZE[0] = 348;
  this->VISUAL_DIFF_PARAMETER_WINDOW_SIZE[1] = 256;

  this->VISUAL_DIFF_LEGEND_SIZE[0] = 16;
  this->VISUAL_DIFF_LEGEND_SIZE[1] = 16;

  this->VIRTUAL_CELL_LABEL_SIZE[0] = 40;
  this->VIRTUAL_CELL_LABEL_SIZE[1] = 40;

  this->QUERY_PREVIEW_SIZE[0] = 256;
  this->QUERY_PREVIEW_SIZE[0] = 256;

  //////// BRUSH & PEN ////////
  this->PIPELINE_VIEW_BACKGROUND_BRUSH = QBrush(
      QImage(":/images/pipeline_bg.png"));
  this->VERSION_TREE_BACKGROUND_BRUSH = QBrush(
      QImage(":/images/version_bg.png"));
  this->QUERY_BACKGROUND_BRUSH = QBrush(
      QImage(":/images/query_bg.png"));
      
  this->MODULE_PEN = QPen(QBrush(ColorByName::get("black")), 2);
  this->MODULE_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("goldenrod_medium")), 3);
  this->MODULE_LABEL_PEN = QPen(QBrush(ColorByName::get("black")), 2);
  this->MODULE_LABEL_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("black")), 2);
  this->GHOSTED_MODULE_LABEL_PEN = QPen(QBrush(
      ColorByName::get("dark_dim_grey")), 2);
  this->MODULE_BRUSH = QBrush(ColorByName::get("light_grey"));
  this->ERROR_MODULE_BRUSH = QBrush(ColorByName::get("salmon"));
  this->SUCCESS_MODULE_BRUSH = QBrush(ColorByName::get("mint"));
  this->ACTIVE_MODULE_BRUSH = QBrush(ColorByName::get("navajo_white"));
  this->COMPUTING_MODULE_BRUSH = QBrush(ColorByName::get("yellow"));
  this->NOT_EXECUTED_MODULE_BRUSH = QBrush(
      ColorByName::get("light_goldenrod"));

  // Pen and brush for un-matched queried modules
  this->GHOSTED_MODULE_PEN = QPen(QBrush(
      ColorByName::get("dark_dim_grey")), 2);
  this->GHOSTED_MODULE_BRUSH = QBrush(
      ColorByName::get("light_dim_grey"));

  // Brush and pen to draw a port shape at regular state
  this->PORT_PEN = QPen(QBrush(ColorByName::get("black")), 1);
  this->PORT_BRUSH = QBrush(ColorByName::get("light_grey"));
  this->PORT_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("goldenrod_medium")), 3);
  this->PORT_OPTIONAL_PEN = QPen(QBrush(
      ColorByName::get("lamp_black")), 1);
  this->PORT_OPTIONAL_BRUSH = QBrush(
      ColorByName::get("titanium_white"));
  
  // Pen and brush for drawing ports of ghosted modules
  this->GHOSTED_PORT_PEN = QPen(QBrush(
      ColorByName::get("dark_dim_grey")), 2);
  this->GHOSTED_PORT_BRUSH = QBrush(
      ColorByName::get("light_dim_grey"));

  // Pen and brush for drawing the configure button
  this->CONFIGURE_PEN = QPen(QBrush(ColorByName::get("black")), 1);
  this->CONFIGURE_BRUSH = QBrush(ColorByName::get("black"));

  // Pen and brush for drawing the ghosted configure button
  this->GHOSTED_CONFIGURE_PEN = QPen(QBrush(
      ColorByName::get("dark_dim_grey")), 2);
  this->GHOSTED_CONFIGURE_BRUSH = QBrush(
      ColorByName::get("dark_dim_grey"));

  // Brush and pen to draw connections
  this->CONNECTION_PEN = QPen(QBrush(ColorByName::get("black")), 2);
  this->CONNECTION_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("goldenrod_medium")), 3);
  this->CONNECTION_BRUSH = QBrush(ColorByName::get("black"));

  // Pen for drawing while connecting any ghosted modules
  this->GHOSTED_CONNECTION_PEN = QPen(QBrush(
      ColorByName::get("dark_dim_grey")), 2);

  // Pen to draw version tree node
  this->VERSION_PEN = QPen(QBrush(
      ColorByName::get("black")), 2) ;   
  this->GHOSTED_VERSION_PEN = QPen(QBrush(
      ColorByName::get("light_grey")), 2); 
  this->VERSION_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("goldenrod_medium")), 4);
  this->VERSION_FILTER_PEN = QPen(QBrush(
      ColorByName::get("green")), 4);
  this->VERSION_SELECTED_FILTER_PEN = QPen(QBrush(
      ColorByName::get("yellow_green")), 4);
  this->VERSION_PLAYBACK_START_PEN = QPen(QBrush(
      ColorByName::get("permanent_green")), 2);
  this->VERSION_PLAYBACK_END_PEN = QPen(QBrush(
      ColorByName::get("cadmium_red_deep")), 2);

  // Brush and pen to draw a version label
  this->VERSION_LABEL_PEN = QPen(QBrush(
      ColorByName::get("black")), 2);
  this->GHOSTED_VERSION_LABEL_PEN = QPen(QBrush(
      ColorByName::get("light_grey")), 2);
  this->VERSION_LABEL_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("black")), 2);

  // Brush to draw version belongs to the current user
  this->VERSION_USER_BRUSH = QBrush(
      ColorByName::get("desatcornflower"));
  this->GHOSTED_VERSION_USER_BRUSH = QBrush(
      ColorByName::get("very_light_grey"));

  // Brush to draw version belongs to the other users
  this->VERSION_OTHER_BRUSH = QBrush(
      ColorByName::get("melon"));

  // Brush and pen to draw a link between two versions
  this->LINK_PEN = QPen(QBrush(
      ColorByName::get("black")), 1.5);
  this->LINK_SELECTED_PEN = QPen(QBrush(
      ColorByName::get("goldenrod_medium")), 3);
  this->GHOSTED_LINK_PEN = QPen(QBrush(
      ColorByName::get("light_grey")), 2);

  // Selection box color
  this->SELECTION_BOX_BRUSH = QBrush(
      ColorByName::get("light_grey"));
  this->SELECTION_BOX_PEN = QPen(QBrush(
      ColorByName::get("lamp_black")), 1);
  
  // Color of the version is being diff from in
  this->VISUAL_DIFF_FROM_VERSION_BRUSH = QBrush(
      ColorByName::get("melon"));
  
  // Color of the version is being diff to in
  this->VISUAL_DIFF_TO_VERSION_BRUSH = QBrush(
      ColorByName::get("steel_blue_light"));
  
  // Color of the paramter changed modules in Visual Diff
  this->VISUAL_DIFF_PARAMETER_CHANGED_BRUSH = QBrush(
      ColorByName::get("light_grey"));
  
  // Color of the shared modules in Visual Diff
  this->VISUAL_DIFF_SHARED_BRUSH = QBrush(QColor(155, 155, 155, 255));

  // Pen & Brush of the circled id on the right corner of the
  // virtual cell label
  this->ANNOTATED_ID_PEN = QPen(Qt::white);
  this->ANNOTATED_ID_BRUSH = QBrush(QColor(157, 0, 0, 255));

  //////// FONTS ////////        
  // Font for module text
  this->MODULE_FONT = QFont("Arial", 14, QFont::Bold);
  this->MODULE_DESC_FONT = QFont("Arial", 12);

  // Font for version text
  this->VERSION_FONT = QFont("Arial", 15, QFont::Bold);
  this->VERSION_FONT_METRICS = new QFontMetrics(this->VERSION_FONT);
  this->VERSION_DESCRIPTION_FONT = QFont("Arial", 15, QFont::Normal, true);
  this->VERSION_DESCRIPTION_FONT_METRICS = new QFontMetrics(this->VERSION_DESCRIPTION_FONT);
  this->VERSION_PROPERTIES_FONT = QFont("Arial", 12);
  this->VERSION_PROPERTIES_PEN = QBrush(
      QColor(20, 100, 20, 255));
      

  // Font showing on the Parameter Inspector window of Visual Diff
  this->VISUAL_DIFF_PARAMETER_FONT = QFont("Arial", 10);
  
  // Font showing on the Legend window of Visual Diff
  this->VISUAL_DIFF_LEGEND_FONT = QFont("Arial", 9);


  //////// ICONS & IMAGES ////////
  //The application disclaimer image
  this->DISCLAIMER_IMAGE = QPixmap(":/images/disclaimer.png");
  //The application icon
  // this->APPLICATION_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/vistrails_icon_small.png')

  // //The application pixmap
  // this->APPLICATION_PIXMAP = QPixmap(
       // core.system.vistrails_root_directory() +
      // '/gui/resources/images/vistrails_icon_small.png')

  // // The execute icons in the first spot of vistrail view toolbar
  // this->EXECUTE_PIPELINE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/execute.png')
  // this->EXECUTE_EXPLORE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/execute_explore.png')

  // // The undo icons for the vistrail view toolbar
  // this->UNDO_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/undo.png')

  // // The redo icons for the vistrail view toolbar
  // this->REDO_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/redo.png')

  // // Icon to select the tabbed view
  // this->TABBED_VIEW_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/tabbed_view.png')

  // // Icon to select the horizontal split view
  // this->HORIZONTAL_VIEW_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/hor_view.png')

  // // Icon to select the vertical split view
  // this->VERTICAL_VIEW_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/ver_view.png')

  // // Icon to select the docking-style view
  // this->DOCK_VIEW_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/dock_view.png')

  // // Toolbar icon for creating a Vistrail
  // this->NEW_VISTRAIL_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/new_vistrail.png')
  
  // // Toolbar icon for opening a vistrail
  // this->OPEN_VISTRAIL_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/open_vistrail.png')

  // //Toolbar icon for opening a vistrail from a database
  // this->OPEN_VISTRAIL_DB_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/openfromdb.png')

  // //Icon for database connections
  // this->DB_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/database.png')

  // //Icon for vistrails files
  // this->FILE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/file.png')
  
  // // Toolbar icon for save the current Vistrail
  // this->SAVE_VISTRAIL_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/save_vistrail.png')

  // // Toolbar icon for toggling console mode window
  // this->CONSOLE_MODE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/console.png')

  // // Toolbar icon for toggling bookmarks window
  // this->BOOKMARKS_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/bookmarks.png')
  
  // // BOOKMARKS WINDOW
  // //Toolbar icon for removing bookmark
  // this->BOOKMARKS_REMOVE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/edittrash.png')

  // //Toolbar icon for reloading a bookmark
  // this->BOOKMARKS_RELOAD_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/reload.png')

  // Background image of the Visual Diff pipeline view
  this->VISUAL_DIFF_BACKGROUND_IMAGE = 
    QImage(":/images/pipeline_bg.png");
  
  // Toolbar icon for showing the Parameter Inspector window
  // this->VISUAL_DIFF_SHOW_PARAM_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/show_params.png')
  
  // // Toolbar icon for showing the Legend window
  // this->VISUAL_DIFF_SHOW_LEGEND_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/show_legends.png')

  // // Toolbar icon for creating an analogy
  // this->VISUAL_DIFF_CREATE_ANALOGY_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/analogy.png')

  // // Toolbar icon for close button on the vistrail tabbar
  // this->VIEW_MANAGER_CLOSE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/closeview.png')

  // // Toolbar icon for the dock toolbutton on the splitted window
  // this->DOCK_BACK_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/dockback.png')

  // // Icon for adding string in the parameter exploration widget
  // this->ADD_STRING_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/edit_add.png')

  // // Icon for moving string up in the parameter exploration widget
  // this->UP_STRING_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/up.png')

  // // Icon for moving string up in the parameter exploration widget
  // this->DOWN_STRING_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/down.png')

  // // Toolbar icons for views
  // this->PIPELINE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/pipeline.png')
  // this->HISTORY_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/history.png')
  // this->QUERY_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/full_tree.png')
  // this->EXPLORE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/explore.png')

  // // Toolbar icon for visual query on a vistrail
  // this->VISUAL_QUERY_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/visual_query.png')
  
  // // Toolbar icon for viewing the whole version tree
  // this->VIEW_FULL_TREE_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/full_tree.png')

  // // Toolbar icon for viewing the whole version tree
  // this->PERFORM_PARAMETER_EXPLORATION_ICON = QIcon(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/perform_pe.png')

  // // Toolbar icon for dragging pixmap of VisDiff
  // this->VERSION_DRAG_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/dragging.png')

  // // Parameter Exploration Pixmaps
  // this->EXPLORE_COLUMN_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/column.png')
  // this->EXPLORE_ROW_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/row.png')
  // this->EXPLORE_SHEET_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/sheet.png')
  // this->EXPLORE_TIME_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/time.png')        
  // this->EXPLORE_SKIP_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/skip.png')        
  // this->REMOVE_PARAM_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/remove_param.png')        
  // this->RIGHT_ARROW_PIXMAP = QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/right.png')        
  
  // // Cursor for zoom in/out graphics views
  // this->SELECT_CURSOR = QCursor(QtCore.Qt.ArrowCursor)
  // this->OPEN_HAND_CURSOR = QCursor(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/open_hand.png'))
  // this->CLOSE_HAND_CURSOR = QCursor(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/close_hand.png'))
  // this->ZOOM_CURSOR = QCursor(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/zoom.png'))
          
  // // Cursor icon for zoom in/out graphics views
  // this->SELECT_ICON = QIcon(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/select_icon.png'))
  // this->PAN_ICON = QIcon(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/pan_icon.png'))
  // this->ZOOM_ICON = QIcon(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/zoom_icon.png'))

  // // Saved Queries icons
  // this->QUERY_VIEW_ICON = this->ZOOM_ICON
  // this->QUERY_ARROW_ICON = QIcon(QPixmap(
          // core.system.vistrails_root_directory() +
          // '/gui/resources/images/zoom_arrow_icon.png'))
  // this->QUERY_EDIT_ICON = QIcon(QPixmap(
      // core.system.vistrails_root_directory() +
      // '/gui/resources/images/edit.png'))

  //////// COLORS ////////
  // Color for the PIP frame
  this->PIP_FRAME_COLOR = ColorByName::get("yellow_light");

  // Color of selected methods in the modules method area
  this->METHOD_SELECT_COLOR = ColorByName::get("yellow_light");

  // Color of the hover/unhover alias labels
  this->HOVER_DEFAULT_COLOR = ColorByName::get("black");
  this->HOVER_SELECT_COLOR = ColorByName::get("blue");
}

MacTheme::MacTheme()
{
  this->APPLICATION_ICON = QIcon(":/images/vistrails_icon.png");
  this->APPLICATION_PIXMAP = QPixmap(":/images/vistrails_icon.png");
  this->VIEW_MANAGER_CLOSE_ICON = QIcon(":/images/mac/closetab.png");
}

void ThemeHolder::setTheme(DefaultTheme *theme)
{
  ThemeHolder::theme = theme;
}

DefaultTheme *ThemeHolder::getCurrentTheme()
{
  if(ThemeHolder::theme == NULL)
  {
    ThemeHolder::theme = get_os_theme();
    initializeCurrentTheme();
  }
  return ThemeHolder::theme;
}
