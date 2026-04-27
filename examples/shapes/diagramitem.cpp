/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QBrush>
#include <QRandomGenerator>

#include "diagramitem.h"
#include "diagramscene.h"

DiagramItem::DiagramItem(DiagramType diagramType, QGraphicsItem *item,
                         QGraphicsScene *scene, QColor color)
    : QGraphicsPolygonItem(item)
{
    (void)scene;
    if (diagramType == Box) {
        boxPolygon << QPointF(0, 0) 
		   << QPointF(0, DIAGRAM_ITEM_SCALE) 
		   << QPointF(DIAGRAM_ITEM_SCALE, DIAGRAM_ITEM_SCALE)
                   << QPointF(DIAGRAM_ITEM_SCALE, 0) 
		   << QPointF(0, 0);
        setPolygon(boxPolygon);
    } else {
        trianglePolygon << QPointF(DIAGRAM_ITEM_SCALE/2.0, 0) 
			<< QPointF(DIAGRAM_ITEM_SCALE, DIAGRAM_ITEM_SCALE) 
			<< QPointF(0, DIAGRAM_ITEM_SCALE)
                        << QPointF(DIAGRAM_ITEM_SCALE/2.0, 0);
        setPolygon(trianglePolygon);
    }

    if (color==Qt::transparent) {
      auto *rng = QRandomGenerator::global();
      color = QColor(rng->bounded(256), rng->bounded(256), rng->bounded(256));
    }
    QBrush brush(color);
    setBrush(brush);
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable);
}
