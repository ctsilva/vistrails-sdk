/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "GraphicsView.h"
#include "Theme.h"
#include <math.h>

//const static double log2 = log(2.0);

void QGraphicsRubberBandItem::paint(QPainter *painter, 
  const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  painter->setOpacity(0.5);
  painter->fillRect(this->rect(), 
    ThemeHolder::getCurrentTheme()->SELECTION_BOX_BRUSH);
  painter->setOpacity(1.0);
  painter->setPen(ThemeHolder::getCurrentTheme()->SELECTION_BOX_PEN);
  painter->drawRect(this->rect());
}

void QGraphicsRubberBandItem::prepareGeoChange()
{
  this->prepareGeometryChange();
}


QInteractiveGraphicsScene::QInteractiveGraphicsScene(QWidget *parent)
  : QGraphicsScene(parent)
{
  this->sceneBoundingRect = QRectF();
  this->multiSelecting = false;
}


void QInteractiveGraphicsScene::updateSceneBoundingRect()
{
  this->sceneBoundingRect = QRectF();
  Q_FOREACH(QGraphicsItem *item, this->items())
  {
    QRectF rect = item->sceneBoundingRect();
    this->sceneBoundingRect = this->sceneBoundingRect.united(rect);
  }

  // Keep a minimum size
  qreal minWDiff = 0;
  qreal minHDiff = 0;
  qreal min = ThemeHolder::getCurrentTheme()->BOUNDING_RECT_MINIMUM;
  if (this->sceneBoundingRect.width() < min)
    minWDiff = min - this->sceneBoundingRect.width();
  if (this->sceneBoundingRect.height() < min)
    minHDiff = min - this->sceneBoundingRect.height();
  this->sceneBoundingRect.adjust(-minWDiff/2, minHDiff/2, 
                   minWDiff/2, -minHDiff/2);

  qreal diff = abs(this->sceneBoundingRect.width()-
         this->sceneBoundingRect.height());

  if (this->sceneBoundingRect.width() < this->sceneBoundingRect.height())
    this->sceneBoundingRect.adjust(-diff/2, 0, diff/2, 0);
  else
    this->sceneBoundingRect.adjust(0, -diff/2, 0, diff/2);

  QRectF panRect = this->sceneBoundingRect.adjusted(
    -this->sceneBoundingRect.width()*100,
    -this->sceneBoundingRect.height()*100,
    this->sceneBoundingRect.width()*100,
    this->sceneBoundingRect.height()*100);
  if (panRect.width() < 0.000001 && panRect.height() < 0.000001)
    panRect = QRectF(-1000,-1000,2000,2000);
  this->setSceneRect(panRect);

  // Reset cache
  Q_FOREACH(QGraphicsView *view, this->views())
    view->resetCachedContent();
}

void QInteractiveGraphicsScene::fitToView(QGraphicsView *view, 
                                          bool recompute_bounding_rect)
{
  if (recompute_bounding_rect)
    this->updateSceneBoundingRect();
  view->centerOn(this->sceneBoundingRect.center());
  view->fitInView(this->sceneBoundingRect, Qt::KeepAspectRatio);

  // Set minimum scale to 0.5
  if (view->matrix().m11() > 0.5 && view->matrix().m22() > 0.5)
    view->scale(0.5/view->matrix().m11(), 0.5/view->matrix().m22());
}

void QInteractiveGraphicsScene::fitToAllViews(bool recompute_bounding_rect)
{
  if (recompute_bounding_rect)
    this->updateSceneBoundingRect();
  Q_FOREACH(QGraphicsView *view, this->views())
    this->fitToView(view, false);
}

void QInteractiveGraphicsScene::clearItems()
{
  this->removeItems(this->items());
}

void QInteractiveGraphicsScene::removeItems(QList<QGraphicsItem*> itemList)
{
  Q_FOREACH(QGraphicsItem *item, itemList)
    if (item->scene())
      this->removeItem(item);
}

void QInteractiveGraphicsScene::saveToPDF(const QString &filename)
{
  this->updateSceneBoundingRect();
  QPrinter printer(QPrinterInfo::defaultPrinter());
  printer.setOutputFormat(QPrinter::PdfFormat);
  printer.setOutputFileName(filename);
  QPainter painter(&printer);
  QBrush brush = this->backgroundBrush();
  this->setBackgroundBrush(QBrush(QColor(255,255,255)));
  this->render(&painter, QRectF(), this->sceneBoundingRect);
  painter.end();
  this->setBackgroundBrush(brush);
}

void QInteractiveGraphicsScene::emitTwoVersionsSelected(
  const vt::VersionId &id1, const vt::VersionId &id2)
{
  emit twoVersionsSelected(id1, id2);
}

//--------------------------------------------------------------------------
// QInteractiveGraphicsView methods

QInteractiveGraphicsView::QInteractiveGraphicsView(QWidget *parent)
: QGraphicsView(parent)
{
  // FIXME 
  // Workaround fo Qt/PyQt weirdness on Ubuntu 7.04 as of 2008/05/21
  // if scroll bar is not visible, panning does not work.
  this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
  this->setInteractive(true);
  //this->setCacheMode(QGraphicsView::CacheBackground)
  this->setResizeAnchor(QGraphicsView::AnchorViewCenter);
  this->setRenderHints(QPainter::Antialiasing |
    QPainter::TextAntialiasing |
    QPainter::SmoothPixmapTransform);
  this->selectionBox = new QGraphicsRubberBandItem();
  this->scaleMax = 2000;
  this->scaleRatio = this->scaleMax/10;
  this->currentScale = this->scaleMax/2;
  this->startScroll = QPoint(0,0);
  this->lastPos = new QPoint(0,0);
  this->pipScene = NULL;
  this->pipFrame = NULL;
  this->resetButton = NULL;
  this->startSelectingPos = NULL;
  this->setProperty("captureModifiers", QVariant(1));
  this->defaultCursorState = 0;
  this->setCursorState(this->defaultCursorState);
  this->canSelectBackground = true;
  this->canSelectRectangle = true;
}

QInteractiveGraphicsView::~QInteractiveGraphicsView()
{
  delete this->selectionBox;
  this->selectionBox = NULL;
  
  if(this->lastPos)
  {
    delete this->lastPos;
    this->lastPos = NULL;
  }
  
  if(this->startSelectingPos)
  {
    delete this->startSelectingPos;
    this->startSelectingPos = NULL;
  }
}

void QInteractiveGraphicsView::modifiersPressed(Qt::KeyboardModifiers modifiers)
{
  this->validateCursorState(modifiers);
}

void QInteractiveGraphicsView::modifiersReleased()
{
  this->validateCursorState();
}

int QInteractiveGraphicsView::findCursorState(Qt::KeyboardModifiers modifiers)
{
  if (!this->isActiveWindow())
    return this->defaultCursorState;
  if (modifiers == Qt::NoModifier)
    modifiers = QApplication::keyboardModifiers();
  bool shift = modifiers & Qt::ShiftModifier;
  bool alt = modifiers & Qt::AltModifier;
  bool meta = (modifiers & Qt::MetaModifier) || (alt && shift);
  bool ctrl = modifiers & Qt::ControlModifier;
  if (shift && !alt && !ctrl && !meta)
    return 1;
  else if (meta && !ctrl)
    return 2;
  else
    return this->defaultCursorState;
}

void QInteractiveGraphicsView::validateCursorState(
  Qt::KeyboardModifiers modifiers)
{
  this->setCursorState(this->findCursorState(modifiers));
}

void QInteractiveGraphicsView::enterEvent(QEvent *e)
{
  this->validateCursorState();
  QGraphicsView::enterEvent(e);
}

void QInteractiveGraphicsView::setCursorState(int state)
{
  if (state==0)
    this->viewport()->setCursor(ThemeHolder::getCurrentTheme()->SELECT_CURSOR);
  else if (state==1)
    this->viewport()->setCursor(ThemeHolder::getCurrentTheme()->OPEN_HAND_CURSOR);
  else if (state==2)
    this->viewport()->setCursor(ThemeHolder::getCurrentTheme()->ZOOM_CURSOR);
  else if (state==3)
    this->viewport()->setCursor(ThemeHolder::getCurrentTheme()->CLOSE_HAND_CURSOR);
}

void QInteractiveGraphicsView::setDefaultCursorState(int state)
{
  this->defaultCursorState = state;
  this->validateCursorState();
}

Qt::MouseButtons QInteractiveGraphicsView::translateButton(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
  {
    int state = this->findCursorState(e->modifiers());
    switch(state)
    {
    case 0:
      return Qt::LeftButton;
    case 1:
      return Qt::MidButton;
    case 2:
      return Qt::RightButton;
    }
  }
  return e->buttons();
}

void QInteractiveGraphicsView::mousePressEvent(QMouseEvent *e)
{
  QPointF scenePos = this->mapToScene(e->pos());
  QGraphicsItem *item = this->scene()->itemAt(scenePos);
  Qt::MouseButtons buttons = this->translateButton(e);
  if (buttons == Qt::LeftButton)
  {
    if (item==NULL)
    {
      QInteractiveGraphicsScene *scene = 
        dynamic_cast<QInteractiveGraphicsScene*>(this->scene());
      if (scene)
      {
        scene->multiSelecting = true;
        scene->addItem(this->selectionBox);
        this->selectionBox->setZValue(1000);
        
        if(this->startSelectingPos)
          delete this->startSelectingPos;
        this->startSelectingPos = new QPointF(this->mapToScene(e->pos()));
        QRectF rect(*this->startSelectingPos, QSizeF(0,0));
        this->selectionBox->setRect(rect);
        this->selectionBox->setVisible(this->canSelectRectangle);
      }
    }
    else
      QGraphicsView::mousePressEvent(e);
  }
  else
  {
    if (buttons & Qt::RightButton)
    {
      this->setCursorState(2);
      this->computeScale();
    }
    else if (buttons & Qt::MidButton)
    {
      this->setCursorState(3);
      this->startScroll = QPoint(this->horizontalScrollBar()->value(),
                this->verticalScrollBar()->value());
    }
    delete this->lastPos;
    this->lastPos = new QPoint(QCursor::pos());
    this->setDragMode(QGraphicsView::NoDrag);
  }
}

void QInteractiveGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
  this->setUpdatesEnabled(false);
  Qt::MouseButtons buttons = this->translateButton(e);
  if (buttons == Qt::LeftButton)
  {
    if (this->startSelectingPos)
    {
      QPointF dis = this->mapToScene(e->pos()) - *this->startSelectingPos;
      QRectF rect(*this->startSelectingPos, QSizeF(dis.x(), dis.y()));
      this->selectionBox->prepareGeoChange();
      this->selectionBox->setRect(rect);
      this->selectModules();
    }
    else
      QGraphicsView::mouseMoveEvent(e);
  }
  else if (this->lastPos)
  {
    if (buttons == Qt::RightButton)
    {
      QPoint globalPos = QCursor::pos();
    
      // Set up new scale based on pixels moved
      int newScale = this->currentScale + (globalPos.y() - this->lastPos->y()) +
        (globalPos.x() - this->lastPos->x());

      // Clamp the scale
      if (newScale<0) newScale = 0;
      if (newScale>this->scaleMax) newScale = this->scaleMax;

      // Update the scale and transformation matrix
      this->currentScale = newScale;
      this->updateMatrix();

      // Need to update last position
      delete this->lastPos;
      this->lastPos = new QPoint(globalPos.y(), globalPos.x());
    }
      
    else if (buttons == Qt::MidButton)
    {
      QPoint globalPos = QCursor::pos();
      
       // Just need to pan the scroll bar
      this->horizontalScrollBar()->setValue(this->startScroll.x() -
                        globalPos.x() +
                        this->lastPos->x());
      this->verticalScrollBar()->setValue(this->startScroll.y() -
                        globalPos.y() +
                        this->lastPos->y());
    }
  }
  else
    this->validateCursorState(e->modifiers());
  this->setUpdatesEnabled(true);
}

void QInteractiveGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
  if (this->startSelectingPos)
  {

    this->selectModules();
    delete this->startSelectingPos;
    this->startSelectingPos = NULL;
    this->selectionBox->setVisible(false);

    
    QInteractiveGraphicsScene *scene = 
      dynamic_cast<QInteractiveGraphicsScene*>(this->scene());
    if(scene)
    {
      scene->removeItem(this->selectionBox);
      scene->multiSelecting = false;
    }
  }
  delete this->lastPos;
  this->lastPos = NULL;
  this->validateCursorState(e->modifiers());
  this->setUpdatesEnabled(true);
  QGraphicsView::mouseReleaseEvent(e);
}

void QInteractiveGraphicsView::mouseDoubleClickEvent(QMouseEvent *e)
{
  if (this->canSelectBackground)
    QGraphicsView::mouseDoubleClickEvent(e);
}

void QInteractiveGraphicsView::selectModules()
{
  QRectF br;
  if (this->canSelectRectangle)
    br = this->selectionBox->sceneBoundingRect();
  else
    br = QRectF(*this->startSelectingPos, *this->startSelectingPos);
  if (!this->canSelectBackground)
  {
    QList<QGraphicsItem*> items = this->scene()->items(br); 
    if (items.isEmpty() || 
      (items.size() == 1 && items[0] == this->selectionBox))
      return;
  }

  QPainterPath path = QPainterPath();
  path.addRect(br);
  this->scene()->setSelectionArea(path);
}

void QInteractiveGraphicsView::updateMatrix()
{
  QMatrix matrix = QMatrix();
  float power = ((float) this->currentScale - this->scaleMax/2) 
    / this->scaleRatio;
  float scale = pow(2.0f, power);
  matrix.scale(scale, scale);
  this->setMatrix(matrix);
}

inline double mylog2(double n)
{
  return log(n) / log(2.0);
}

void QInteractiveGraphicsView::computeScale()
{
  this->currentScale = mylog2(this->matrix().m11()) * 
    this->scaleRatio + this->scaleMax/2;
}

void QInteractiveGraphicsView::setPIPScene(QGraphicsScene *scene)
{
  this->pipScene = scene;
}

void QInteractiveGraphicsView::setPIPEnabled(bool enabled)
{
  if (this->pipScene)
  {
    if (enabled)
    {
      if (this->pipFrame==NULL)
      {
        this->pipFrame = new QPIPGraphicsView(this);
        this->pipFrame->graphicsView->setScene(this->pipScene);
        this->pipFrame->move(this->width(), 0);
      }
      this->pipFrame->show();
      this->pipFrame->updateGeometry();
    }
    else if (this->pipFrame!=NULL)
      this->pipFrame->hide();
  }
}

void QInteractiveGraphicsView::setQueryEnabled(bool enabled)
{
  if (enabled)
  {
    if (!this->resetButton)
    {
      this->resetButton = new QResetQueryButton(this);
      this->connect(this->resetButton,
             SIGNAL(resetQuery()),
             SLOT(resetQuery()), Qt::UniqueConnection);
    }
    this->resetButton->show();
    this->resetButton->updateGeometry();
  }
  else
    this->resetButton->hide();
}

void QInteractiveGraphicsView::resetQuery()
{
  emit resetQuery();
}

void QInteractiveGraphicsView::resizeEvent(QResizeEvent *e)
{
  if (this->pipFrame)
    this->pipFrame->updateGeometry();
  if (this->resetButton)
    this->resetButton->updateGeometry();
  QGraphicsView::resizeEvent(e);
}

QSize QInteractiveGraphicsView::sizeHint()
{
  return QSize(512,512);
}

void QInteractiveGraphicsView::zoomIn()
{
  QWheelEvent e(QPoint(), 120, Qt::NoButton, Qt::NoModifier);
  this->wheelEvent(&e);
}

void QInteractiveGraphicsView::zoomOut()
{
  QWheelEvent e(QPoint(), -120, Qt::NoButton, Qt::NoModifier);
  this->wheelEvent(&e);
}

void QInteractiveGraphicsView::wheelEvent(QWheelEvent *e) 
{
  this->computeScale();
  this->currentScale += e->delta()/5.0;
  if (this->currentScale < 0)
    this->currentScale = 0;
  if (this->currentScale > this->scaleMax)
    this->currentScale = this->scaleMax;

  this->updateMatrix();
}

// FIXME:
// Workaround for border aliasing on OSX
// However, it breaks things on Linux, because it
// makes zooming _extremely_ slow, so we check it
// before we run.
//if (core.system.systemType == 'Darwin')
//  def setScene(self, scene))
//    """ setScene(scene) QGraphicsScene) -> None
//    Make sure the viewport background brush the same as the scene
//    one. This is only neccessary on the Mac to work around Qt/Mac
//    bug. We can remove this if (any future release of Qt fixes
//    this.
//
//    """
//    QGraphicsView.setScene(self, scene)
//    if (this->scene())
//      palette = QPalette(this->viewport().palette())
//      palette.setBrush(QPalette.Base, 
//               this->scene().backgroundBrush())
//      this->viewport().setPalette(palette)

//--------------------------------------------------------------------------
// QPIPGraphicsView Methods

QPIPGraphicsView::QPIPGraphicsView(QWidget *parent)
: QWidget(parent)
{
  this->setMouseTracking(true);
  this->setAutoFillBackground(true);
  QPalette pallete = QPalette(this->palette());
  pallete.setColor(QPalette::Base,
    ThemeHolder::getCurrentTheme()->PIP_FRAME_COLOR);
  this->setPalette(pallete);
  this->hBoxLayout = new QHBoxLayout(this);
  this->setLayout(this->hBoxLayout);
  this->layout()->setMargin(
    ThemeHolder::getCurrentTheme()->PIP_OUT_FRAME_WIDTH);
  this->graphicsView = new QInteractiveGraphicsView();
  this->layout()->addWidget(this->graphicsView);
  this->firstShow = true;
  this->anchorCorner[0] = Qt::AlignRight;
  this->anchorCorner[1] = Qt::AlignTop;
}

QPIPGraphicsView::~QPIPGraphicsView()
{
  delete this->graphicsView;
  this->graphicsView = NULL;

  delete this->hBoxLayout;
  this->hBoxLayout = NULL;
}

QSize QPIPGraphicsView::sizeHint()
{
  int *size = ThemeHolder::getCurrentTheme()->PIP_DEFAULT_SIZE;
  return QSize(size[0], size[1]);
}
      
void QPIPGraphicsView::mousePressEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
  {
    this->resizing = false;
    this->lastPos = QPoint(e->globalX(), e->globalY());
    const QRect gvRect = this->graphicsView->geometry();
    if (((e->x() < gvRect.left() || e->x() > gvRect.right()) &&
      (e->y() < gvRect.top() || e->y() > gvRect.bottom())))
    {
      this->direction[0] = 1;
      this->direction[1] = 1;
      if (e->x() < gvRect.left())
        this->direction[0] = -1;
      if (e->y() < gvRect.top())
        this->direction[1] = -1;
      this->resizing = true;
    }
  }
}
      
void QPIPGraphicsView::mouseMoveEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
  {
    int dx = e->globalX() - this->lastPos.x();
    int dy = e->globalY() - this->lastPos.y();
    const QRect parentGeometry = this->parentWidget()->geometry();
    QRect newGeometry = QRect(this->geometry());

    if (this->resizing)
    {
      if (this->direction[0]==1)
      {
        if (newGeometry.right() + dx > parentGeometry.width() - 1)
          dx = parentGeometry.width() - 1 - newGeometry.right();
        newGeometry.adjust(0, 0, dx, 0);
      }
      else
      {
        if (newGeometry.left()+dx<0)
          dx = -newGeometry.left();
        newGeometry.adjust(dx, 0, 0, 0);
      }
      if (this->direction[1]==1)
      {
        if (newGeometry.bottom()+dy>parentGeometry.height()-1)
          dy = parentGeometry.height()-1-newGeometry.bottom();
        newGeometry.adjust(0, 0, 0, dy);
      }
      else
      {
        if (newGeometry.top()+dy<0)
          dy = -newGeometry.top();
        newGeometry.adjust(0, dy, 0, 0);
      }
    }
    else
      newGeometry.translate(dx, dy);

    if ((newGeometry.left()>0 &&
      newGeometry.right()<parentGeometry.width()-1))
      this->anchorCorner[0] = (Qt::AlignmentFlag)0;
    if ((newGeometry.top()>0 &&
      newGeometry.bottom()<parentGeometry.height()-1))
      this->anchorCorner[1] = (Qt::AlignmentFlag)0;
    this->updateGeometry(newGeometry);
    this->lastPos = QPoint(e->globalX(), e->globalY());
  }
}

void QPIPGraphicsView::updateGeometry(QRect newGeometry)
{
  const QRect parentGeometry = this->parentWidget()->geometry();
  if (newGeometry==QRect())
    newGeometry = QRect(this->geometry());
    
  if (this->anchorCorner[0]==0)
  {
    if (newGeometry.left()<0)
      newGeometry.moveLeft(0);
    if (newGeometry.right()>=parentGeometry.width())
      newGeometry.moveRight(parentGeometry.width()-1);
    if (newGeometry.left()==0)
      this->anchorCorner[0] = Qt::AlignLeft;
    if (newGeometry.right()==parentGeometry.width()-1)
      this->anchorCorner[0] = Qt::AlignRight;
  }
  else
  {
    if (this->anchorCorner[0]==Qt::AlignLeft)
      newGeometry.moveLeft(0);
    if (this->anchorCorner[0]==Qt::AlignRight)
      newGeometry.moveRight(parentGeometry.width()-1);
  }

  if (this->anchorCorner[1]==0)
  {
    if (newGeometry.top()<0)
      newGeometry.moveTop(0);
    if (newGeometry.bottom()>=parentGeometry.height())
      newGeometry.moveBottom(parentGeometry.height()-1);
    if (newGeometry.top()==0)
      this->anchorCorner[1] = Qt::AlignTop;
    if (newGeometry.bottom()==parentGeometry.height()-1)
      this->anchorCorner[1] = Qt::AlignBottom;
  }
  else
  {
    if (this->anchorCorner[1]==Qt::AlignTop)
      newGeometry.moveTop(0);
    if (this->anchorCorner[1]==Qt::AlignBottom)
      newGeometry.moveBottom(parentGeometry.height()-1);
  }
      
  this->setGeometry(newGeometry);
}

void QPIPGraphicsView::showEvent(QShowEvent *e)
{
  if (this->firstShow)
  {
    this->firstShow = false;

    QInteractiveGraphicsScene *scene = 
      dynamic_cast<QInteractiveGraphicsScene*>(this->graphicsView->scene());
    if(scene)
      scene->fitToView(this->graphicsView, true);
  }
  QWidget::showEvent(e);
}

void QPIPGraphicsView::enterEvent(QEvent *e)
{
  this->layout()->setMargin(
    ThemeHolder::getCurrentTheme()->PIP_IN_FRAME_WIDTH);
}

void QPIPGraphicsView::leaveEvent(QEvent *e)
{
  this->layout()->setMargin(ThemeHolder::getCurrentTheme()->PIP_OUT_FRAME_WIDTH);
}

//--------------------------------------------------------------------------
// QResetQueryButton methods

QResetQueryButton::QResetQueryButton(QWidget *parent)
: QLabel(parent)
{
  this->setText("Reset Query");
  this->setFrameStyle(QFrame::StyledPanel);
  this->setFrameShadow(QFrame::Raised);
  this->marginPad = 10;
}

void QResetQueryButton::mousePressEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
    this->setFrameShadow(QFrame::Sunken);
}
  
void QResetQueryButton::mouseReleaseEvent(QMouseEvent *e)
{
  this->setFrameShadow(QFrame::Raised);
  emit resetQuery();
}

void QResetQueryButton::updateGeometry()
{
  const QRect parentGeometry = this->parentWidget()->geometry();
  this->move(this->marginPad, 
        parentGeometry.height() - this->height() - this->marginPad);
}
