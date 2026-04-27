/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QGraphicsSceneMouseEvent>

#include "diagramscene.h"
#include "diagramitem.h"

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
  movingItem = 0;
}

void DiagramScene::addDiagramItem(DiagramItem *item)
{
  this->itemMap[item->id()] = item;
  this->addItem(item);
}

void DiagramScene::removeDiagramItem(int id)
{
  if (this->itemMap.contains(id)) {
    DiagramItem *item = this->itemMap[id];
    this->itemMap.remove(id);
    this->removeItem(item);
  }
}

DiagramItem* DiagramScene::getDiagramItem(int id)
{
  return this->itemMap[id];
}

void DiagramScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
  QPointF mousePos = event->buttonDownScenePos(Qt::LeftButton);
  movingItem = itemAt(mousePos, QTransform());

  if (movingItem != 0 && event->button() == Qt::LeftButton) {
    oldPos = movingItem->pos();
  }
    
  clearSelection();    
  QGraphicsScene::mousePressEvent(event);
}

void DiagramScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
  if (movingItem != 0 && event->button() == Qt::LeftButton) {
    if (oldPos != movingItem->pos())
      emit itemMoved(qgraphicsitem_cast<DiagramItem *>(movingItem),
                     oldPos);
    movingItem = 0;
  }
  QGraphicsScene::mouseReleaseEvent(event);
}
