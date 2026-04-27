/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

// This containing a subclassed QGraphicsView that allows
// zoom/pan/select inside the graphics view and supports PIP as well
// 
// QInteractiveGraphicsScene
// QInteractiveGraphicsView
// QPIPGraphicsView

#ifndef __GRAPHICSVIEW_H__
#define __GRAPHICSVIEW_H__

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QEnterEvent>
#include "VisTrails.h"

class QPIPGraphicsView;
class QResetQueryButton;

//QGraphicsItem will override the default QGraphicsItem mouseRelease
//event to let it fires only 1 ItemSelectionChanged event. This is
//due to the clearSelection() in
//qgraphicsitem.cpp::QGrahpicsItem::mouseReleaseEvent
//class QGraphicsItemInterface
//{
//public:
//  // Make sure the current item is not selected before
//  // clearSelection and perform the selected again
//  // NOTE: Old vistrails code used this (for grouping?) to reset the 
//  // selection.  The selection on mouse release causes problems for maya 
//  // when changing to a version fails and we need to set it back.
//  virtual void  mouseReleaseEvent(QGraphicsSceneMouseEvent *e){}
//};

// QGraphicsRubberBandItem try to replace QRubberBand to have a
// unified look and feel on all platform. In the end, it mimics the
// Windows look. It is just a rectangle with a 50% transparency
class QGraphicsRubberBandItem: public QGraphicsRectItem
{
public:
  // Peform actual painting of the rubber band
  void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, 
    QWidget *widget = NULL);

  // calls protected pepareGeometryChange()
  void prepareGeoChange();
};

// QInteractiveGraphicsScene expands QGraphicsScene to allow panning
// freely over the view area
class QInteractiveGraphicsScene: public QGraphicsScene
{
  Q_OBJECT

public:
  // Initialize the actual scene bounding rect
  QInteractiveGraphicsScene(QWidget *parent = NULL);

  // Compute the actual bounding rect of all shapes, then update
  // the scene rect to be much wider for panning
  void updateSceneBoundingRect();

  // Adjust view to fit and center the whole scene. If recompute_bounding_rect is
  // False, does not recompute bounds, and instead uses previous one.
  void fitToView(QGraphicsView *view, bool recompute_bounding_rect = false);

  // Adjust all views using this scene to fit and center the whole scene
  // if recompute_bounding_rect is False, uses previous bounding rect.
  void fitToAllViews(bool recompute_bounding_rect = false);

  // Remove and delete all items belonging to this scene
  void clearItems();

  // Remove all items in itemList
  void removeItems(QList<QGraphicsItem*> itemList);

  void saveToPDF(const QString &filename);

  bool multiSelecting;

  void emitTwoVersionsSelected(const vt::VersionId &id1, 
    const vt::VersionId &id2);

signals:
  void twoVersionsSelected(const vt::VersionId &id1, 
    const vt::VersionId &id2);

private:
  QRectF sceneBoundingRect;
};

//QInteractiveGraphicsView is QGraphicsView with abilities to
//zoom/span with right/mid click
class QInteractiveGraphicsView: public QGraphicsView
{
  Q_OBJECT
public:
  //Initialize the graphics view with interactive options
  QInteractiveGraphicsView(QWidget *parent = NULL);

  //Destroy objects on the heap
  ~QInteractiveGraphicsView();

  //Notification when one of the modifier keys has been pressed
  void modifiersPressed(Qt::KeyboardModifiers modifiers);

  //Notification when one of the modifier keys has been released
  void modifiersReleased();
  
  //Check the keyboard modifiers and return the cursor state
  int findCursorState(Qt::KeyboardModifiers modifiers = Qt::NoModifier);
  
  //Check the keyboard modifiers to change the cursor shape correspondingly
  void validateCursorState(Qt::KeyboardModifiers modifiers = Qt::NoModifier);

  //Check the modifiers state when the mouse enter the
  //canvas. Then update the mouse functionality appropriately
  void enterEvent(QEnterEvent *e);
  
  //Update the cursor shape
  //
  //Keyword arguments:
  //state - 0: selecting (default)
  //        1: pan
  //        2: zoom
  //        3: panning
  void setCursorState(int state);
  
  //Set the default cursor state when no modifier key is pressed
  void setDefaultCursorState(int state);

  //Translate mouse button and modifiers into a virtual mouse button
  Qt::MouseButtons translateButton(QMouseEvent *e);

  //Handle mouse click event, use Qt rubber band for left-click
  //selection and prepare for zoom/pan on right/mid click
  void mousePressEvent(QMouseEvent *e);

  //Handle right click (zoom) and mid click (pan). This function
  //uses QCursor globalPos instead of e.globalX() and e.globalY()
  //because of their flaky values during transformation
  void mouseMoveEvent(QMouseEvent *e);
  
  //Mark box selecting as off
  void mouseReleaseEvent(QMouseEvent *e);
  
  //Try to avoid unselect if double-click on the background
  void mouseDoubleClickEvent(QMouseEvent *e);
  
  //Select all modules inside the this->selectionBox
  void selectModules();

  //Update the view matrix with the current scale
  void updateMatrix();
  
  	//Compute the current scale based on the view matrix
	void computeScale();

  //Set the Picture-In-Picture scene fo the current GraphicsView
  //to 'scene'
  void setPIPScene(QGraphicsScene *scene);

  // Enable/Disable PIP view
  void setPIPEnabled(bool enabled = true);

  // Enable/Disable the query reset button
  void setQueryEnabled(bool enabled = true);

  // pass the signal along
  void resetQuery();

  // Make sure the pip frame is inside the graphics view
  void resizeEvent(QResizeEvent *e);

  // Return recommended size of the widget
  QSize sizeHint();

  // Return current scale
  double getCurrentScale() { return this->currentScale; }

  bool canSelectRectangle;
  QGraphicsRubberBandItem *selectionBox;
  QPointF *startSelectingPos;

public slots:
  // Equivalent to rotating mouse wheel one positive notch
  void zoomIn();

  // Equivalent to rotating mouse wheel one negative notch
  void zoomOut();

protected:
  void wheelEvent(QWheelEvent *e);

private:
  QPoint *lastPos;
  QPoint startScroll;
  double currentScale;
  double scaleMax;
  double scaleRatio;
  int defaultCursorState;
  bool canSelectBackground;
  QGraphicsScene *pipScene;
  QPIPGraphicsView *pipFrame;
  QResetQueryButton *resetButton;
};

// QPIPGraphicsView is a tool window contain a
// QInteractiveGraphicsView for PIP display
class QPIPGraphicsView: public QWidget
{
public:
  // Initialize a layout with some margin and a central widget
  QPIPGraphicsView(QWidget *parent = NULL);
  ~QPIPGraphicsView();

  // Return recommended size of the widget
  QSize sizeHint();
        
  // Capture mouse press event on the frame to move the widget
  void mousePressEvent(QMouseEvent *e);
        
  // Move widget as the mouse moving
  void mouseMoveEvent(QMouseEvent *e);

  // Make sure the widget is inside the parent graphics view
  void updateGeometry(QRect newGeometry = QRect());

  // Fit the scene to view for the show event only
  void showEvent(QShowEvent *e);

  // Show a larger frame when the mouse enter to facilitate
  void enterEvent(QEnterEvent *e);

  // Show a smaller frame when the mouse exit the widget to get
  // more view
  void leaveEvent(QEvent *e);

  QInteractiveGraphicsView *graphicsView;

private:
  QHBoxLayout *hBoxLayout;
  bool firstShow;
  bool resizing;
  Qt::AlignmentFlag anchorCorner[2];
  QPoint lastPos;
  int direction[2];
};

class QResetQueryButton: public QLabel
{
	Q_OBJECT

public:
  QResetQueryButton(QWidget *parent = NULL);

  // Capture mouse press event on the frame to move the widget
  void mousePressEvent(QMouseEvent *e);
    
  void mouseReleaseEvent(QMouseEvent *e);

  void updateGeometry();

signals:
  void resetQuery();

private:
  int marginPad;
};

#endif


