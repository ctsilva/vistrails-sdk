/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

/*
This is a QGraphicsView for pipeline view, it also holds different
types of graphics items that are only available in the pipeline
view. It only handles GUI-related actions, the rest of the
functionalities are implemented at somewhere else,
e.g. core.vistrails

QGraphicsLinkItem
QGraphicsVersionTextItem
QGraphicsVersionItem
QVersionTreeScene
QVersionTreeView
*/

#ifndef __VERSIONVIEW_H__
#define __VERSIONVIEW_H__

#include "GraphicsView.h"
#include <QMouseEvent>
#include "VistrailController.h"
#include "VersionProp.h"

class QGraphicsVersionItem;
class QVersionTreeScene;

//from core.dotty import DotLayout
//from core.vistrails_tree_layout_lw import VistrailsTreeLayoutLW
//from core.system import systemType
//from gui.graphics_view import (QInteractiveGraphicsScene,
//                               QInteractiveGraphicsView,
//                               QGraphicsItemInterface,
//                               QGraphicsRubberBandItem)
//from gui.version_prop import QVersionPropOverlay
//from gui.theme import CurrentTheme
//from gui.vis_diff import QVisualDiff
//from gui.qt import qt_super
//import gui.utils
//import math


// QGraphicsLinkItem is a connection shape connecting two versions
class QGraphicsLinkItem: public QGraphicsPolygonItem
{
public:
  // Create the shape, initialize its pen and brush accordingly
  QGraphicsLinkItem(QGraphicsItem *parent = 0, QVersionTreeScene *scene = 0);

  void mousePressEvent(QGraphicsSceneMouseEvent *e);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

  // Set this link to be ghosted or notRayann
  void setGhosted(bool ghosted);

  // Setup a line connecting v1 and v2 items
  void setupLink(QGraphicsVersionItem *v1, QGraphicsVersionItem *v2, 
    bool expand = false, bool collapse = false);

  // Peform actual painting of the link
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
    QWidget *widget = NULL);

  // Do not allow link to be selected with version shape
  QVariant itemChange(QGraphicsItem::GraphicsItemChange change, 
    QVariant value);

private:
  QPen linkPen;
  QPointF c1, c2;
  bool ghosted, expand, collapse;
  QList<QLineF> lines;
  vt::VersionId startVersion, endVersion;
  QVersionTreeScene *treeScene;
};

// QGraphicsVersionTextItem is an editable text item that appears on top of;
// a QGraphicsVersionItem to allow the tag to be changed;
class QGraphicsVersionTextItem: public QGraphicsTextItem
{
  Q_OBJECT

public:
  QGraphicsVersionTextItem();

  // Enter and Return keys signal a change in the label.  Other keys
  // update the position and size of the parent ellipse during key entry.
  void keyPressEvent(QKeyEvent *e);

  // Update the tag if the text has changed
  void focusOutEvent(QFocusEvent *e);

  void activate(QGraphicsVersionItem* item);

  void updatePos();

  bool isActivated();

  QGraphicsVersionItem *getActiveItem();

private:
  QGraphicsVersionItem *activeItem;
  bool activated;
  float centerX;
  float centerY;
};

// QGraphicsVersionItem is the version shape holding version id and label
class QGraphicsVersionItem: public QObject, 
  public QGraphicsEllipseItem  
{
  Q_OBJECT

public:
  // Create the shape, initialize its pen and brush accordingly
  QGraphicsVersionItem(QGraphicsItem *parent = NULL, 
    QVersionTreeScene *scene = NULL);

  ~QGraphicsVersionItem();

  // Set this version to be ghosted or not
  void setGhosted(bool ghosted = true);

  // If necessary, update the colors of this version node based on
  // who owns the node and new ranks.
  // NOTE: if username changes during execution, this might break.
  void update_color(bool isThisUs, int new_rank, int new_max_rank, 
    bool new_ghosted);

  // Set the color of this version depending on whose is the user
  // and its saturation
  void setSaturation(bool isThisUser, float sat);
  
  // Change width of ellipse based on text
  void updateWidthFromLabel();
  void updateWidthFromLabel(const QString &lbl);
  
  // Update the version dimensions and id
  void setupVersion(vt::Version *version);

  // Add a padded space to avoid un-updated area
  QRectF boundingRect();

  // Peform actual painting of the version shape
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
    QWidget *widget = 0);
  
  // Do not allow links to be selected with version
  QVariant itemChange(QGraphicsItem::GraphicsItemChange change, 
    QVariant value);

  // Start dragging a version to someplaces...
  void mousePressEvent(QGraphicsSceneMouseEvent * e);

  // Now set the timer preparing for dragging. Must use a timer in
  // junction with QDrag in order to avoid problem updates stall of
  // QGraphicsView, especially on Linux
  void mouseMoveEvent(QGraphicsSceneMouseEvent * e);

  // Cancel the drag
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *e);

  // Capture version-to-version drag-and-drop
  void dragEnterEvent(QDragEnterEvent *e);

  void dropEvent(QDropEvent *e);

  void perform_analogy();

  // Captures context menu event.
  void contextMenuEvent(QGraphicsSceneContextMenuEvent *e);

  // Create actions related to context menu
  void createActions();

  // update color based on new max rank
  void updateColor();

  void setLabel(QString lbl);

  static void resetRank();

  vt::VersionId id;
  bool ghosted;
  //QGraphicsVersionTextItem *text;
  int width, height;

protected:
  void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e);

public slots:
  // Start the drag of QDrag
  void startDrag();

private:
  QPen versionPen;
  QPen versionLabelPen;
  QFont versionFont;
  QFontMetrics *versionFontMetrics;
  QBrush versionBrush;
  bool dragging;
  int rank;
  static int max_rank;
  QTimer *dragTimer;
  QPoint dragPos;
  QAction *addToBookmarksAct;
  QVersionTreeScene *treeScene;
  QMimeData *mimeData;
  QString label;
};

// QVersionTree inherits from QInteractiveGraphicsScene to keep track
// of the version scenes, i.e. versions, connections, etc.
class QVersionTreeScene
  : public QInteractiveGraphicsScene, public vt::HNotify
{
  Q_OBJECT

public:
  // Initialize the graphics scene with no shapes
  QVersionTreeScene(QWidget *parent = NULL);

  // delete objects on heap
  ~QVersionTreeScene();

  void setSessionFocus(vt::SessionFocus *focus);
  vt::SessionFocus * getSessionFocus();

  // Add a module to the scene.
  void addVersion(vt::Version* version);

  void setSelected(const vt::VersionId &id);

  // Remove version from scene and mapping
  void removeVersion(const vt::VersionId &id);
  
  // Add a link to the scene
  void addLink(QGraphicsVersionItem *guiSource, 
    QGraphicsVersionItem *guiTarget, bool expand, bool collapse);

  // Remove link from scene and mapping
  void removeLink(const vt::VersionId &childId);

  // Clear the whole scene
  void clear();

  // Based on the controller to set version colors
  void adjust_version_colors();

  // Faster alternative to setup_scene when a single version is
  // changed. When this is called, we know that both old_version
  // and new_version don't have tags associated, so no layout
  // changes happen
  void update_scene_single_node_change(vt::Session *session, 
    const vt::VersionId &old_version, const vt::VersionId &new_version);

  // Setup all the versions on the curent vistail
  void setupVersions();
  
  // Capture 'Del', 'Backspace' for pruning versions when not editing a tag
  void keyPressEvent(QKeyEvent *e);

  VistrailController *controller;

  void versionAdded(vt::Version* version);
  void versionSelected(const vt::VersionId &previous, 
    const vt::VersionId &current);
  void versionChanged(const vt::VersionId &id);
  void vistrailChanged();

public slots:
  // Start up a timer for animating tree drawing events
  void updateAnimation();

  // Construct the scene to view a version tree
  void setupScene();

  // Ensure current item remains selected
  void selectionChangedSlot();

public:
  QGraphicsVersionTextItem *text;

private:
  QTimer *timer;
  int animation_step, num_animation_steps;
  bool ignoreSelectionChanged;
};

// QVersionTreeView inherits from QInteractiveGraphicsView that will
// handle drawing of versions layout output from Dotty
class QVersionTreeView: public QInteractiveGraphicsView
{
public:
  // Initialize the graphics view and its properties
  QVersionTreeView(QWidget *parent = NULL);
  ~QVersionTreeView();

  void setSessionFocus(vt::SessionFocus *focus);
  // Overrides parent class to disable text items if you click on background
  void selectModules();
  QVersionTreeScene *getTreeScene();
  QVersionPropOverlay *getVersionProp();

private:
  QVersionTreeScene *treeScene;
  QVersionPropOverlay *versionProp;
};

#endif

