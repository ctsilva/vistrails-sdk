/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QHash>

class DiagramItem;
QT_BEGIN_NAMESPACE
class QGraphicsSceneDragDropEvent;
class QGraphicsViewItem;
QT_END_NAMESPACE

//! [0]
class DiagramScene : public QGraphicsScene
{
  Q_OBJECT

  public:
  DiagramScene(QObject *parent = 0);

  void addDiagramItem(DiagramItem *item);
  void removeDiagramItem(int id);
  DiagramItem* getDiagramItem(int id);

signals:
  void itemMoved(DiagramItem *movedItem, const QPointF &movedFromPosition);

protected:
  void mousePressEvent(QGraphicsSceneMouseEvent *event);
  void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
  QGraphicsItem *movingItem;
  QPointF oldPos;

  QHash<int, DiagramItem*> itemMap;
};
//! [0]

#endif
