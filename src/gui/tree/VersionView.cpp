/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "VersionView.h"
#include "vtwindow.hpp"
#include "Theme.h"
#include <QGraphicsItem>
#include <QColor>
#include <QMimeData>
#include <QDrag>

const static double ZOOM_THRESH = 350.0;

QGraphicsLinkItem::QGraphicsLinkItem(QGraphicsItem *parent, 
                                     QVersionTreeScene *scene)
                                     : QGraphicsPolygonItem(parent)
{
  this->setFlags(QGraphicsItem::ItemIsSelectable);
  this->setZValue(0);
  this->linkPen = ThemeHolder::getCurrentTheme()->LINK_PEN;
  this->ghosted = false;

  // cache link endpoints to improve performance on scene updates
  this->c1 = QPoint();
  this->c2 = QPoint();
  this->expand = false;
  this->collapse = false;

  this->startVersion = vt::VERSION_INVALID;
  this->endVersion = vt::VERSION_INVALID;

  this->treeScene = scene;
}

void QGraphicsLinkItem::mousePressEvent(QGraphicsSceneMouseEvent *e)
{
  this->setSelected(true);
}

void QGraphicsLinkItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
  QGraphicsPolygonItem::mouseReleaseEvent(e);

  if (this->expand)
  {
    this->treeScene->controller->expandVersions(this->startVersion, 
      this->endVersion);
  }
  else if(this->collapse)
  {
    this->treeScene->controller->collapseVersions(this->endVersion);
  }
  this->setSelected(false);
  this->treeScene->setupScene();
}

void QGraphicsLinkItem::setGhosted(bool ghosted)
{
  if(this->ghosted != ghosted)
    this->ghosted = ghosted;
  if(ghosted)
    this->linkPen = ThemeHolder::getCurrentTheme()->GHOSTED_LINK_PEN;
  else
    this->linkPen = ThemeHolder::getCurrentTheme()->LINK_PEN;
}

void QGraphicsLinkItem::setupLink(QGraphicsVersionItem *v1, 
                                  QGraphicsVersionItem *v2,
                                  bool expand, 
                                  bool collapse)
{
  this->startVersion = v1->id;
  this->endVersion = v2->id;

  QPointF _c1 = v1->sceneBoundingRect().center();
  QPointF _c2 = v2->sceneBoundingRect().center();

  // check if it is the same geometry 
  // improves performance on updates
  if(this->c1 != QPointF() && 
    this->c2 != QPointF() && 
    this->c1 == _c1 && 
    this->c2 == _c2 && 
    this->expand == expand && 
    this->collapse == collapse)
  {
    return;
  }

  // update current state
  this->c1 = _c1;
  this->c2 = _c2;
  this->collapse = collapse;
  this->expand = expand;

  // Compute the line of the link and its normal line through
  // the midpoint
  QLineF mainLine(c1, c2);
  QLineF normalLine = mainLine.normalVector();
  normalLine.setLength(ThemeHolder::getCurrentTheme()->LINK_SEGMENT_LENGTH);
  QPointF dis = (mainLine.pointAt(0.5)-mainLine.p1()+
    normalLine.p1()-normalLine.pointAt(0.5));
  normalLine.translate(dis.x(), dis.y());

  // Generate 2 segments along the main line and 3 segments along
  // the normal line
  if(!this->collapse && !this->expand)
  {
    this->lines.clear();
    this->lines.append(mainLine);
    QPolygonF poly = QPolygonF();
    poly.append(this->lines[0].p1());
    poly.append(this->lines[0].p2());
    poly.append(this->lines[0].p1());
    this->setPolygon(poly);
  }
  else
  {
    this->lines.clear();

    normalLine = mainLine.normalVector();
    normalLine.setLength(ThemeHolder::getCurrentTheme()->LINK_SEGMENT_SQUARE_LENGTH);
    dis = (mainLine.pointAt(0.5)-mainLine.p1()+
      normalLine.p1()-normalLine.pointAt(0.5));
    normalLine.translate(dis.x(), dis.y());

    QLineF gapLine(mainLine);
    gapLine.setLength(ThemeHolder::getCurrentTheme()->LINK_SEGMENT_SQUARE_LENGTH/2);
    QPointF gapVector = gapLine.p2()-gapLine.p1();

    // First segment along the main line
    QLineF line(mainLine);
    line.setLength(line.length()/2-ThemeHolder::getCurrentTheme()->LINK_SEGMENT_SQUARE_LENGTH/2);
    this->lines.append(QLineF(line));

    // Second segment along the main line
    line.translate(line.p2()-line.p1()+gapVector*2);
    this->lines.append(QLineF(line));

    // First normal segment in front
    QLineF line_t(normalLine);
    line_t.translate(gapVector*(-1.0));
    this->lines.append(QLineF(line_t));

    // Second normal segment in back
    QLineF line_b(normalLine);
    line_b.translate(gapVector);
    this->lines.append(QLineF(line_b));

    // Left box
    line = QLineF(line_t.p1(),line_b.p1());
    this->lines.append(QLineF(line));

    // Right box
    line = QLineF(line_t.p2(),line_b.p2());
    this->lines.append(QLineF(line));

    // Horizontal plus
    QLineF line_h(normalLine.pointAt(0.2),normalLine.pointAt(0.8));
    this->lines.append(QLineF(line_h));

    if(this->expand)
    {
      // Vertical plus
      line = QLineF(mainLine);
      line.translate((line.p2()-line.p1())/2-gapVector);
      line.setLength(
        ThemeHolder::getCurrentTheme()->LINK_SEGMENT_SQUARE_LENGTH);
      QLineF line_v(line.pointAt(0.2), line.pointAt(0.8));
      this->lines.append(QLineF(line_v));
    }

    // Create the poly line for selection and redraw
    QPolygonF poly = QPolygonF();
    poly.append(this->lines[0].p1());
    poly.append(this->lines[2].p1());
    poly.append(this->lines[3].p1());
    poly.append(this->lines[1].p2());
    poly.append(this->lines[3].p2());
    poly.append(this->lines[2].p2());
    poly.append(this->lines[0].p1());
    this->setPolygon(poly);
  }

  this->setGhosted(v1->ghosted && v2->ghosted);

}

void QGraphicsLinkItem::paint(QPainter *painter, 
                              const QStyleOptionGraphicsItem *option, 
                              QWidget *widget)
{
  QInteractiveGraphicsView* view = 
    static_cast<QInteractiveGraphicsView*>(this->scene()->views()[0]);
									  
  if(view->getCurrentScale() < ZOOM_THRESH)
    return;

  painter->setRenderHint(QPainter::Antialiasing);
  if(this->isSelected())
    painter->setPen(ThemeHolder::getCurrentTheme()->LINK_SELECTED_PEN);
  else
    painter->setPen(this->linkPen);
  
  Q_FOREACH(QLineF line, this->lines)
    painter->drawLine(line);
}

QVariant QGraphicsLinkItem::itemChange(
                                       QGraphicsItem::GraphicsItemChange change, QVariant value)
{
  if(change==QGraphicsItem::ItemSelectedChange && value.toBool())
  {
    int qGraphicsVersionItemType = QGraphicsVersionItem().type();
    Q_FOREACH(QGraphicsItem *item, this->scene()->selectedItems())
    {
      if(item->type() == qGraphicsVersionItemType)
        return QVariant(false);
    }
  }
  return QGraphicsPolygonItem::itemChange(change, value);
}

//--------------------------------------------------------------------------
// QGraphicsVersionTextItem Methods


QGraphicsVersionTextItem::QGraphicsVersionTextItem() : QGraphicsTextItem()
{
  this->setTextInteractionFlags(Qt::TextEditorInteraction);
  this->setFont(ThemeHolder::getCurrentTheme()->VERSION_FONT);
  this->setZValue(2);
  //this->setTextWidth(ThemeHolder::getCurrentTheme()->VERSION_LABEL_MARGIN[0]);
  this->activated = false;
  this->setVisible(false);
}

void QGraphicsVersionTextItem::activate(QGraphicsVersionItem* item)
{
  if(this->activated)
    return;

  this->activeItem = item;
  QPointF center = item->rect().center();
  this->centerX = center.x();
  this->centerY = center.y();
  this->activated = true;

  vt::Version *version = VisTrailsWindow::getInstance()->getSessionFocus()
    ->session()->vistrail()->getVersion(item->id);
  vt::Property *pTag = version->annotations()->get("tag");
  if(pTag && strlen(pTag->asChar()) > 0)
    this->setPlainText(QString(pTag->asChar()));
  item->updateWidthFromLabel(this->toPlainText());
  this->setTextWidth(-1);
  this->updatePos();
  this->setVisible(true);
  this->setFocus(Qt::OtherFocusReason);
}

void QGraphicsVersionTextItem::updatePos()
{
  this->setPos(this->centerX - this->boundingRect().width()/2.0,
               this->centerY - this->boundingRect().height()/2.0);
}

bool QGraphicsVersionTextItem::isActivated()
{
  return this->activated;
}

void QGraphicsVersionTextItem::keyPressEvent(QKeyEvent *e)
{
  if(!this->activated)
    return;

  if(e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return || 
    e->key() == Qt::Key_Escape)
  {
    e->ignore();
    this->clearFocus(); //focus out event handles the update
    return;
  }
  QGraphicsTextItem::keyPressEvent(e);
  this->setTextWidth(-1);
  this->updatePos();
  this->activeItem->updateWidthFromLabel(this->toPlainText());
}

QGraphicsVersionItem *QGraphicsVersionTextItem::getActiveItem()
{
  return this->activeItem;
}

void QGraphicsVersionTextItem::focusOutEvent(QFocusEvent *e)
{
  QGraphicsTextItem::focusOutEvent(e);
  if(!this->activeItem)
    return;

  vt::VersionId id = this->activeItem->id;  
  QString text = this->toPlainText();
  this->setPlainText("");
  this->setVisible(false);
  this->activated = false;
  this->activeItem = NULL;
  if(id != vt::VERSION_ROOT && id != vt::VERSION_INVALID)
  {
    vt::SessionFocus *focus = VisTrailsWindow::getInstance()->getSessionFocus();
    vt::Version *version = focus->session()->vistrail()->getVersion(id);
    vt::Property *pTag = version->annotations()->get("tag");
    if(pTag)
    {
      if(text.isEmpty())
        version->annotations()->remove("tag");
      else if( text != QString(pTag->asChar()))
        version->annotations()->setString("tag", text.toUtf8().constData());
      else
        return;
    }
    else if(!text.isEmpty())
      version->annotations()->setString("tag", text.toUtf8().constData());
    else
      return;
    focus->versionChanged(id);
  }
}

//--------------------------------------------------------------------------
// QGraphicsVersionItem methods
int QGraphicsVersionItem::max_rank = 0;

QGraphicsVersionItem::QGraphicsVersionItem(QGraphicsItem *parent, 
                                           QVersionTreeScene *scene)
                                           : QGraphicsEllipseItem(parent, scene)
{
  this->mimeData = new QMimeData();
  this->treeScene = scene;
  this->setZValue(1);
  this->setAcceptDrops(true);
  this->versionPen = ThemeHolder::getCurrentTheme()->VERSION_PEN;
  this->versionLabelPen = ThemeHolder::getCurrentTheme()->VERSION_LABEL_PEN;
  this->versionBrush = ThemeHolder::getCurrentTheme()->VERSION_USER_BRUSH;
  this->setFlags(QGraphicsItem::ItemIsSelectable);
  this->id = vt::VERSION_INVALID;
  this->dragging = false;
  this->ghosted = false;
  this->createActions();

  // this->rank is a positive number that determines the
  // saturation of the node. Two version nodes might have the
  // same rank if(they were created by different users
  // this->max_rank is the maximum rank Q_FOREACH(that version class
  this->rank = QGraphicsVersionItem::max_rank++;

  // Need a timer to start a drag to avoid stalls on QGraphicsView
  this->dragTimer = new QTimer();
  this->dragTimer->setSingleShot(true);
  this->connect(this->dragTimer,
    SIGNAL(timeout()),
    SLOT(startDrag()));

  this->dragPos = QPoint();
  this->addToBookmarksAct = NULL;
}

QGraphicsVersionItem::~QGraphicsVersionItem()
{
  //delete this->text;
  //this->text = NULL;

  this->mimeData = NULL;

  if(this->addToBookmarksAct != NULL)
  {
    delete this->addToBookmarksAct;
    this->addToBookmarksAct = NULL;
  }
}

void QGraphicsVersionItem::setLabel(QString lbl)
{
  this->label = lbl;
  this->treeScene->update(this->rect());
}

void QGraphicsVersionItem::setGhosted(bool ghost)
{
  if(this->ghosted != ghost)
  {
    DefaultTheme *theme = ThemeHolder::getCurrentTheme();
    this->ghosted = ghost;
    if(ghost)
    {
      this->versionPen = theme->GHOSTED_VERSION_PEN;
      this->versionLabelPen = theme->GHOSTED_VERSION_LABEL_PEN;
      this->versionBrush = theme->GHOSTED_VERSION_USER_BRUSH;
    }
    else
    {
      this->versionPen = theme->VERSION_PEN;
      this->versionLabelPen = theme->VERSION_LABEL_PEN;
      this->versionBrush = theme->VERSION_USER_BRUSH;
    }
  }
}

void QGraphicsVersionItem::update_color(bool isThisUs, int new_rank, 
                                        int new_max_rank, bool new_ghosted)
{
  if((new_rank == this->rank && new_max_rank == this->max_rank &&
    new_ghosted == this->ghosted))
    // nothing changed
    return;

  this->setGhosted(new_ghosted);
  this->rank = new_rank;
  this->max_rank = new_max_rank;
  if(!this->ghosted)
  {
    if(isThisUs)
      setBrush(ThemeHolder::getCurrentTheme()->VERSION_USER_BRUSH);
    else
      setBrush(ThemeHolder::getCurrentTheme()->VERSION_OTHER_BRUSH);
    float sat = (float)(new_rank+1) / new_max_rank;
    qreal h,s,v,a;
    brush().color().getHsvF(&h, &s, &v, &a);
    this->versionBrush = 
      QBrush(QColor::fromHsvF(h, s*sat, v+(1.0-v)*(1-sat), a));
  }
}

void QGraphicsVersionItem::updateColor()
{
  setBrush(ThemeHolder::getCurrentTheme()->VERSION_USER_BRUSH);
  float sat = (float)this->rank / QGraphicsVersionItem::max_rank;
  qreal h,s,v,a;
  brush().color().getHsvF(&h, &s, &v, &a);
  this->versionBrush = 
    QBrush(QColor::fromHsvF(h, s*sat, v+(1.0-v)*(1-sat), a));
}

void QGraphicsVersionItem::setSaturation(bool isThisUser, float sat)
{
  if(!this->ghosted)
  {
    if(isThisUser)
      setBrush(ThemeHolder::getCurrentTheme()->VERSION_USER_BRUSH);
    else
      setBrush(ThemeHolder::getCurrentTheme()->VERSION_OTHER_BRUSH);
    qreal h,s,v,a;
    brush().color().getHsvF(&h, &s, &v, &a);
    this->versionBrush = 
      QBrush(QColor::fromHsvF(h, s*sat, v+(1.0-v)*(1-sat), a));
  }
}

void QGraphicsVersionItem::updateWidthFromLabel()
{
  this->updateWidthFromLabel(label);
}

void QGraphicsVersionItem::updateWidthFromLabel(const QString &lbl)
{
  this->width = this->versionFontMetrics->width(lbl)+40;
  this->height = this->versionFontMetrics->height()+25;

  if(this->width < 80)
    this->width = 80;

  QRectF r = this->rect();
  r.setX(r.center().x() - this->width / 2.0);
  r.setY(r.center().y() - this->height / 2.0);
  r.setWidth(this->width);
  r.setHeight(this->height);
  this->setRect(r);
  this->update(r);
}

void QGraphicsVersionItem::setupVersion(vt::Version *version)
{
  if(this->id == vt::VERSION_ROOT)
  {
    this->rank = 0;
    this->setGhosted();
  }

  DefaultTheme *theme = ThemeHolder::getCurrentTheme();
  this->versionFont = theme->VERSION_FONT;
  this->versionFontMetrics = theme->VERSION_FONT_METRICS;

  vt::Version *versoin = this->treeScene->getSessionFocus()
    ->session()->vistrail()->getVersion(this->id);
  if(version)
  {
    vt::Property *p = version->annotations()->get("tag");
    if(!p)
    {
      this->versionFont = theme->VERSION_DESCRIPTION_FONT;
      this->versionFontMetrics = theme->VERSION_DESCRIPTION_FONT_METRICS;
      p = version->annotations()->get("label");
    }

    if(p)
      this->label = QString(p->asChar());
  }

  this->updateWidthFromLabel();
}

QRectF QGraphicsVersionItem::boundingRect()
{
  return this->rect().adjusted(-2, -2, 2, 2);
}

void QGraphicsVersionItem::paint(QPainter *painter, 
                                 const QStyleOptionGraphicsItem *option,
                                 QWidget *widget)
{
  DefaultTheme *theme = ThemeHolder::getCurrentTheme();
  if(this->isSelected())
    if(this->treeScene->controller->inFilter(this->id))
      painter->setPen(theme->VERSION_SELECTED_FILTER_PEN);
    else
      painter->setPen(theme->VERSION_SELECTED_PEN);
  else
    if(this->treeScene->controller->inFilter(this->id))
      painter->setPen(theme->VERSION_FILTER_PEN);
    else
      painter->setPen(this->versionPen);

  painter->setRenderHint(QPainter::Antialiasing);
  painter->setBrush(this->versionBrush);
  painter->drawEllipse(this->rect());

  if(this->isSelected() && !this->ghosted)
    painter->setPen(theme->VERSION_LABEL_SELECTED_PEN);
  else
    painter->setPen(this->versionLabelPen);

  QInteractiveGraphicsView* view = 
    static_cast<QInteractiveGraphicsView*>(this->scene()->views()[0]);
									  
  if(view->getCurrentScale() > ZOOM_THRESH && 
     this != this->treeScene->text->getActiveItem())
    {
      painter->setFont(this->versionFont);
      painter->drawText(this->rect(), Qt::AlignCenter, this->label);
    }

  if(this->treeScene->controller->playback)
    if(this->treeScene->controller->playbackStart == this)
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(theme->VERSION_PLAYBACK_START_PEN);
      painter->drawEllipse(this->rect().adjusted(-4,-4,4,4));
    }
    else if(this->treeScene->controller->playbackEnd == this)
    {
      painter->setBrush(Qt::NoBrush);
      painter->setPen(theme->VERSION_PLAYBACK_END_PEN);
      painter->drawEllipse(this->rect().adjusted(-4,-4,4,4));
    }
}

QVariant QGraphicsVersionItem::itemChange(
  QGraphicsItem::GraphicsItemChange change, QVariant value)
{
  //if(change==QGraphicsItem::ItemSelectedChange && !value.toBool())
  //  this->text->hide();

  QInteractiveGraphicsScene *scene = 
    dynamic_cast<QInteractiveGraphicsScene*>(this->scene());
  if(((change==QGraphicsItem::ItemSelectedChange && value.toBool()) ||
    (change==QGraphicsItem::ItemSelectedChange &&
    (!value.toBool() &&
    this->scene()->selectedItems().size() == 1))) && scene != NULL)
  {
    QList<QGraphicsItem*> selectedItems = this->scene()->selectedItems();
    vt::VersionId selectedId = vt::VERSION_INVALID;
    bool selectByClick = !scene->multiSelecting;
    if(value.toBool())
    {
      Q_FOREACH(QGraphicsItem *item, selectedItems)
      {
        QGraphicsLinkItem *link = dynamic_cast<QGraphicsLinkItem*>(item);
        if(link)
        {
          link->setSelected(false);
          link->update();
        }
      }
      selectedItems = this->scene()->selectedItems();
      if(selectedItems.isEmpty())
        selectedId = this->id;
    }
    else if(selectedItems.size()==2)
    {
      if(selectedItems[0]==this)
      {
        QGraphicsVersionItem* temp = 
          dynamic_cast<QGraphicsVersionItem*>(selectedItems[1]);
        if(temp)
          selectedId = temp->id;
      }
      else
      {
        QGraphicsVersionItem* temp = 
          dynamic_cast<QGraphicsVersionItem*>(selectedItems[0]);
        if(temp)
          selectedId = temp->id;
      }
    }
    selectByClick = this->scene()->mouseGrabberItem() == this;
    if(!selectByClick)
    {
      Q_FOREACH(QGraphicsItem *item, this->scene()->items())
      {
        QGraphicsRubberBandItem *band = 
          dynamic_cast<QGraphicsRubberBandItem*>(item);
        if(band)
        {
          selectByClick = true;
          break;
        }
      }
    }

    // Update the selected items list to include only versions && 
    // check if(two versions selected
    QList<QGraphicsVersionItem*> selectedVersions;
    Q_FOREACH(QGraphicsItem *item, this->scene()->selectedItems())
    {
      QGraphicsVersionItem* versionItem = 
        dynamic_cast<QGraphicsVersionItem*>(item);
      if(versionItem)
        selectedVersions.append(versionItem);
    }

    // if(adding a version, the ids are self && other selected version
    if(selectedVersions.size() == 1 && value.toBool()) 
    {
      scene->emitTwoVersionsSelected(selectedVersions[0]->id, this->id);
    }
    // if(deleting a version, the ids are the two selected versions that
    // are !self
    else if(selectedVersions.size() == 3 && !value.toBool())
    {
      if(selectedVersions[0] == this)
      {
        scene->emitTwoVersionsSelected(selectedVersions[1]->id, 
          selectedVersions[2]->id);
      }
      else if(selectedVersions[1] == this)
      {
        scene->emitTwoVersionsSelected(selectedVersions[0]->id, 
          selectedVersions[2]->id);
      }
      else
      {
        scene->emitTwoVersionsSelected(selectedVersions[0]->id, 
          selectedVersions[1]->id);
      }
    }
  }

  return QGraphicsItem::itemChange(change, value);
}

void QGraphicsVersionItem::mousePressEvent(QGraphicsSceneMouseEvent * e)
{
  if(e->button()==Qt::LeftButton)
  {
    this->dragging = true;
    this->dragPos = QPoint(e->screenPos());

    if(this->isSelected() && id != vt::VERSION_ROOT && id != vt::VERSION_INVALID)
    {
      e->ignore();
      this->treeScene->text->activate(this);
      return;
    }
    else if(this->treeScene->text->isActivated())
      this->treeScene->text->clearFocus();
  }
  QGraphicsEllipseItem::mousePressEvent(e);
}

void QGraphicsVersionItem::mouseMoveEvent(QGraphicsSceneMouseEvent * e)
{
//  if(this->dragging)
//  {
//    QPoint dragVector = e->screenPos()-this->dragPos;
//    if (dragVector.manhattanLength() > 2)
//    {
//      this->dragging = false;
//      // the timer has undesirable effects on Windows
//#if defined (_WIN32)
//      this->startDrag();
//#else
//      // this->dragTimer.start(1);
//#endif
//    }
//  }
//  QGraphicsEllipseItem::mouseMoveEvent(e);
}

void QGraphicsVersionItem::startDrag()
{
  mimeData->setText(QString(this->id.toString().asChar()));
  QDrag _drag(this->scene()->views()[0]);
  _drag.setMimeData(mimeData);
  _drag.setPixmap(ThemeHolder::getCurrentTheme()->VERSION_DRAG_PIXMAP);
  _drag.start();
}

void QGraphicsVersionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *e)
{
  this->dragging = false;
  if(this->id == vt::VERSION_INVALID)
    return;

  VisTrailsWindow *mw = VisTrailsWindow::getInstance();
  PlaybackSelectionMode mode = mw->getPlaybackSelectionMode();    
  if(mode != SelectPlaybackEnd)
  {
    this->treeScene->getSessionFocus()->selectVersion(this->id);
    this->treeScene->setupScene();
    if(mode == SelectPlaybackStart)
      mw->setPlaybackStart(this->id);
  }
  else
  {
    mw->setPlaybackEnd(this->id);
    this->setSelected(false);
    this->treeScene->setupScene();
    return;
  }
  if(this->treeScene->text->isActivated())
  {
    e->ignore();
    return;
  }
  QGraphicsEllipseItem::mouseReleaseEvent(e);
}

void QGraphicsVersionItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *e)
{
  // VisTrailsWindow *mw = VisTrailsWindow::getInstance();
  // if(mw->getPlaybackSelectionMode() == NotSelectingPlayback)
  // {
  //   mw->getVersionContextMenu()->setVersion(this->id);
  //   mw->getVersionContextMenu()->getVersionEdit()->show();
  // }
}

void QGraphicsVersionItem::dragEnterEvent(QDragEnterEvent *e)
{
  if(e->mimeData()->hasText())
    e->accept();
  else
    e->ignore();
}

void QGraphicsVersionItem::dropEvent(QDropEvent *e)
{
  const QMimeData *mimeData = e->mimeData();
  if(mimeData->hasText())
  {
    e->accept();
    // FIXME: convert QVisualDiff
    //visDiff = QVisualDiff(this->scene().controller.vistrail,
    //            data.versionId,
    //            this->id,
    //            this->scene().controller,
    //            this->scene().views()[0]);
    //visDiff.show();
  }
  else
    e->ignore();
}

void QGraphicsVersionItem::perform_analogy()
{
  // FIXME: what does this do?

  //QObject *sender = this->scene()->sender();
  //QString analogy_name = sender->text();
  //QList<QGraphicsItem*> selectedItems = this->scene()->selectedItems();
  //controller = this->scene().controller
  //Q_FOREACH(item in selectedItems)
  //  controller.perform_analogy(analogy_name, item.id)
}

void QGraphicsVersionItem::contextMenuEvent(QGraphicsSceneContextMenuEvent *e)
{
  if(this->id != vt::VERSION_ROOT && this->id != vt::VERSION_INVALID)
  {
    VisTrailsWindow::getInstance()->getVersionContextMenu()->setVersion(this->id);
    VisTrailsWindow::getInstance()->getVersionContextMenu()->popup(e->screenPos());
    VisTrailsWindow::getInstance()->getVersionContextMenu()->show();
  }
}

void QGraphicsVersionItem::createActions()
{
  this->addToBookmarksAct = new QAction("Add To Bookmarks", this->scene());
  this->addToBookmarksAct->setStatusTip("Add this pipeline to bookmarks");
}


void QGraphicsVersionItem::resetRank()
{
  QGraphicsVersionItem::max_rank = 0;
}

//--------------------------------------------------------------------------
// QVersionTreeScene methods

QVersionTreeScene::QVersionTreeScene(QWidget *parent)
: QInteractiveGraphicsScene(parent)
{
  this->setBackgroundBrush(
    ThemeHolder::getCurrentTheme()->VERSION_TREE_BACKGROUND_BRUSH);
  this->setSceneRect(QRectF(-5000, -5000, 10000, 10000));
  this->controller = new VistrailController(this);
  this->timer = new QTimer();
  this->connect(this->timer,
    SIGNAL(timeout()),
    SLOT(updateAnimation()));
  this->connect(this, 
    SIGNAL(selectionChanged()), 
    SLOT(selectionChangedSlot()));
  this->animation_step = 1;
  this->num_animation_steps = 10;
  this->ignoreSelectionChanged = false;

  // Editable text item that remains hidden unless a version is selected
  this->text = new QGraphicsVersionTextItem();
  this->addItem(this->text);
}

QVersionTreeScene::~QVersionTreeScene()
{
  delete this->controller;
  this->controller = NULL;

  delete this->timer;
  this->timer = NULL;

  delete this->text;
  this->text = NULL;
}

void QVersionTreeScene::selectionChangedSlot()
{
  if(!this->ignoreSelectionChanged && this->selectedItems().isEmpty())
  {
    this->controller->setSelected(
      this->controller->getSessionFocus()->currentVersion());
  }
}

void QVersionTreeScene::setSessionFocus(vt::SessionFocus *focus)
{
  this->controller->setSessionFocus(focus);
}

vt::SessionFocus * QVersionTreeScene::getSessionFocus()
{
  return this->controller->getSessionFocus();
}

void QVersionTreeScene::addVersion(vt::Version* version)
{
  QGraphicsVersionItem *versionShape = 
    this->controller->findNode(version->id());
  if(!versionShape)
  {
    versionShape = new QGraphicsVersionItem(NULL, this);
    this->controller->addNode(version->id(), versionShape);
  }
  versionShape->setupVersion(version);
}

void QVersionTreeScene::setSelected(const vt::VersionId &id)
{
  this->controller->setSelected(id);
}

void QVersionTreeScene::removeVersion(const vt::VersionId &id)
{
  this->controller->removeVersion(id);
}


void QVersionTreeScene::removeLink(const vt::VersionId &childId)
{
  this->controller->removeLink(childId);
}

void QVersionTreeScene::clear()
{
  QGraphicsVersionItem::resetRank();
  this->controller->clear();
  this->clearItems();
}

void QVersionTreeScene::adjust_version_colors()
{
  // currently not used
}

void QVersionTreeScene::update_scene_single_node_change(
  vt::Session *session, const vt::VersionId &old_version, 
  const vt::VersionId &new_version)
{
  // FIXME
  //this->setupScene(controller);

  //// we need to call this every time because version ranks might
  //// change
  //this->adjust_version_colors(controller);

  //// update version item
  ////vt::VersionId oldId(old_version.toString().asChar());
  ////vt::VersionId newId(new_version.toString().asChar());
  //QGraphicsVersionItem *v = this->versions[old_version];
  //this->versions[new_version] = v;
  //this->versions.remove(old_version);
  //v->id = new_version;

  //tm = controller.vistrail.tagMap

  // update link items


  // FIXME: controller
  //dst = controller._current_terse_graph.edges_from(new_version)
  //Q_FOREACH(eto, _ in dst)
  //  edge = this->edges[(old_version, eto)]
  //  edge.setupLink(this->versions[new_version],
  //           this->versions[eto],
  //           this->fullGraph.parent(eto) != new_version,
  //           false) // We shouldn't ever need a collapse here
  //  this->edges[(new_version, eto)] = edge
  //  del this->edges[(old_version, eto)]

  //src = controller._current_terse_graph.edges_to(new_version)
  //Q_FOREACH(efrom, _ in src)
  //  edge = this->edges[(efrom, old_version)]
  //  edge.setupLink(this->versions[efrom],
  //           this->versions[new_version],
  //           this->fullGraph.parent(new_version) != efrom,
  //           false) // We shouldn't ever need a collapse here
  //  this->edges[(efrom, new_version)] = edge
  //  del this->edges[(efrom, old_version)]
}

void QVersionTreeScene::setupVersions()
{  
  // setup versions
  vt::SessionFocus *focus = this->getSessionFocus();
  if (focus) {
    vt::VersionId currentVersion = focus->currentVersion();
    vt::Vistrail *vistrail = focus->session()->vistrail();
    vt::VersionTree *fullTree = this->controller->getFullTree();
    vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(fullTree);
    traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_FAST);
    while(!traversal.done())
      {
	vt::VersionId id = traversal.current();
	vt::Version *version = vistrail->getVersion(id);

	// if the version gui object already exists...
	QGraphicsVersionItem *versionShape = this->controller->findNode(id);
	if(!versionShape)
	  {      
	    versionShape = new QGraphicsVersionItem(NULL, this);
	    this->controller->addNode(id, versionShape);
	  }

	versionShape->id = id;
	versionShape->setupVersion(version);

	// set as selected
	if(id == currentVersion)
	  versionShape->setSelected(true);
	else
	  versionShape->setSelected(false);

	traversal.next();
      }
  }
}

void QVersionTreeScene::setupScene()
{
  // perform graph layout
  this->controller->rebuildAndLayout();

  // adjust the colors
  this->adjust_version_colors();

  // clear previous scene
  this->clearItems();

  this->addItem(this->text);

  // add versions and links to scene
  vt::SessionFocus * focus = this->getSessionFocus();
  if (focus) {
    vt::VersionId currentVersion = focus->currentVersion();
    vt::VersionTree *fullTree = this->controller->getFullTree();
    vt::VersionTreeTraversal traversal = 
      vt::VersionTreeTraversal(this->controller->getTerseTree());
    traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_FAST);
    while(!traversal.done())
    {
      vt::VersionId target = traversal.current();
      QGraphicsVersionItem* targetShape = this->controller->findNode(target);
      if(target == vt::VERSION_ROOT)
      {
        this->addItem(targetShape);
        traversal.next();
        continue;
      }

      targetShape->updateColor();

      vt::Vistrail *vistrail = focus->session()->vistrail();
      vt::VersionId source = traversal.parent();
      QGraphicsVersionItem* sourceShape = this->controller->findNode(source);
      bool tHasTag = vistrail->getVersion(target)->annotations()->get("tag");
      bool sHasTag;
      if(source == vt::VERSION_ROOT)
        sHasTag = true;
      else
        sHasTag = vistrail->getVersion(source)->annotations()->get("tag");

      bool expandable = fullTree->getParent(target) != source;
      bool collapsable = 
        (fullTree->getParent(target) == source && // No in betweens
        this->controller->getChildEnum(target) == ExactlyOneChild && // target is not a leaf || branch
        target != currentVersion && // target is !selected
        !tHasTag && // target has no tag
        !this->controller->inLatest(target) && // not one of the last n modules
        !this->controller->inFilter(target) && // not in search filter
        (
          sHasTag || // source has a tag
          source == vt::VERSION_ROOT || // source is root node
          this->controller->getChildEnum(source) == MoreThanOneChild || //branch
          source == currentVersion || // source is selected
          this->controller->inFilter(source)// source in search filter
        )
        );


      QGraphicsLinkItem *linkShape = this->controller->findEdge(target);
      if(!linkShape)
      {
        linkShape = new QGraphicsLinkItem(NULL, this);
        this->controller->addEdge(target, linkShape);
      }

      linkShape->setupLink(sourceShape, targetShape, expandable, collapsable);

      this->addItem(targetShape);
      this->addItem(linkShape);

      traversal.next();
    }

    this->clearSelection();
    QGraphicsVersionItem *cv = this->controller->findNode(currentVersion);
    if(cv)
      cv->setSelected(true);
  }

  this->updateSceneBoundingRect();
  //if(this->selectedItems().count() > 0)
  //Q_FOREACH(QGraphicsView *view, this->views())
  //view->centerOn(this->selectedItems()[0]);
}

void QVersionTreeScene::updateAnimation()
{
  this->animation_step += 1;
  if(this->animation_step >= this->num_animation_steps)
  {
    this->animation_step = 1;
    this->timer->stop();
    this->controller->animate_layout = false; 
  }
  this->setupScene();
  this->update();
}

void QVersionTreeScene::keyPressEvent(QKeyEvent *e)
{
  if(e->key() == Qt::Key_Backspace || e->key() == Qt::Key_Delete)
  {
    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    QList<vt::VersionId> selectedVersions;
    Q_FOREACH(QGraphicsItem* item, selectedItems)
    {
      QGraphicsVersionItem* versionItem = 
        dynamic_cast<QGraphicsVersionItem*>(item);
      if(versionItem)
      {
        //if(versionItem->text->hasFocus())
        //  return;
        //else
          selectedVersions.append(versionItem->id);
      }
    }

    //FIXME: 
    //if(this->controller && !selectedVersions.isEmpty())
    //{      
    //  QMessageBox messageBox(QMessageBox::Question, "Prune Versions?", 
    //    "Are you sure that you want to prune the selected version(s)?",
    //    QMessageBox::Yes | QMessageBox::No);
    //  if(messageBox.exec() == QMessageBox::Yes)
    //  {
    //     this->controller.prune_versions(selectedVersions);
    //  }
    //}
  }
  QInteractiveGraphicsScene::keyPressEvent(e);
}

void QVersionTreeScene::versionAdded(vt::Version* version)
{

  // if the version gui object already exists...
  vt::VersionId id = version->id();
  this->controller->addToLatest(id);
  QGraphicsVersionItem *versionShape = this->controller->findNode(id);
  if(!versionShape)
  {      
    versionShape = new QGraphicsVersionItem(NULL, this);
    this->controller->addNode(id, versionShape);
  }

  versionShape->id = id;
  versionShape->setupVersion(version);

  this->versionSelected(version->parent(), version->id());

  // DAK not sure why we need to delay this...
  // if(VisTrailsWindow::getInstance()->getCurrentViewIndex() == 0) // tree view
  this->setupScene();

  this->views()[0]->centerOn(versionShape);
}

void QVersionTreeScene::versionSelected(const vt::VersionId &previous, 
  const vt::VersionId &current)
{
  this->ignoreSelectionChanged = true;
  this->clearSelection();
  QGraphicsVersionItem *node = this->controller->findNode(current);
  if(node)
    node->setSelected(true);
  this->ignoreSelectionChanged = false;
}

void QVersionTreeScene::versionChanged(const vt::VersionId &id)
{
  QGraphicsVersionItem *node = this->controller->findNode(id);
  if(node)
  {
    vt::SessionFocus *focus = this->getSessionFocus();
    vt::Version *version = focus->session()->vistrail()->getVersion(id);
    node->setupVersion(version);
  }
}

void QVersionTreeScene::vistrailChanged()
{
  this->controller->reset();
  this->setupVersions();
  this->setupScene();
}

//--------------------------------------------------------------------------
// QVersionTreeView methods
QVersionTreeView::QVersionTreeView(QWidget *parent)
: QInteractiveGraphicsView(parent)
{
  this->setWindowTitle("Version Tree");

  this->treeScene = new QVersionTreeScene(this);
  this->setScene(treeScene);
  
  this->treeScene->setupVersions();
  //this->treeScene->setupScene();

  this->versionProp = new QVersionPropOverlay(this, this->viewport());
  this->versionProp->updateController(this->treeScene->controller);
  this->versionProp->show(); 
}

QVersionTreeView::~QVersionTreeView()
{
  delete this->treeScene;
  delete this->versionProp;

  this->treeScene = NULL;
  this->versionProp = NULL;
}

void QVersionTreeView::setSessionFocus(vt::SessionFocus *focus)
{
  this->treeScene->setSessionFocus(focus);
  this->treeScene->setupVersions();
  this->treeScene->setupScene();
  this->treeScene->fitToView(this, true);
  if(this->treeScene->selectedItems().count() > 0)
    this->centerOn(this->treeScene->selectedItems()[0]);
}

void QVersionTreeView::selectModules()
{
  QRectF br;
  if(this->canSelectRectangle)
    br = this->selectionBox->sceneBoundingRect();
  else
    br = QRectF(*this->startSelectingPos, *this->startSelectingPos);
  QList<QGraphicsItem*> items = this->scene()->items(br);
  if(items.isEmpty() || (items.size() == 1 && items[0] == this->selectionBox))
  {
    Q_FOREACH(QGraphicsItem *item, this->scene()->selectedItems())
    {
      QGraphicsVersionItem* versionItem = 
        dynamic_cast<QGraphicsVersionItem*>(item);

      //if(versionItem)
      //{
      //  //versionItem->text->clearFocus();
      //  //versionItem->text->hide();
      //}
    }
  }

  QInteractiveGraphicsView::selectModules();
}

QVersionTreeScene *QVersionTreeView::getTreeScene()
{
  return this->treeScene;
}

QVersionPropOverlay *QVersionTreeView::getVersionProp()
{
  return this->versionProp;
}
