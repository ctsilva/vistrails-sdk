/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsPolygonItem>
#include <QColor>

QT_BEGIN_NAMESPACE
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsSceneMouseEvent;
class QPointF;
QT_END_NAMESPACE

#define DIAGRAM_ITEM_SCALE 80

class DiagramItem : public QGraphicsPolygonItem
{
public:
  enum { Type = UserType + 1 };
  enum DiagramType { Box, Triangle };

  DiagramItem(DiagramType diagramType, QGraphicsItem *item = 0,
              QGraphicsScene *scene = 0, QColor color=Qt::transparent);

  int type() const {
    return Type;
  }
  
  DiagramType diagramType() const {
    return polygon() == boxPolygon ? Box : Triangle;
  }

  int id() { return this->itemId; }
  void setId(int ii) { this->itemId = ii; }

private:
  QPolygonF boxPolygon;
  QPolygonF trianglePolygon;
  int itemId;
};

#endif
