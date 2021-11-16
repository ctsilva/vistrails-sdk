/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QtGui>
#include <QByteArray>
#include <QDataStream>

#include "commands.h"
#include "diagramitem.h"
#include "diagramscene.h"

//! [0]
MoveCommand::MoveCommand(DiagramItem *diagramItem, const QPointF &oldPos,
                         QUndoCommand *parent)
    : QUndoCommand(parent){

  myDiagramItem = diagramItem;
  newPos = diagramItem->pos();
  myOldPos = oldPos;
}
//! [0]

//! [1]
bool MoveCommand::mergeWith(const QUndoCommand *command)
{
  const MoveCommand *moveCommand = static_cast<const MoveCommand *>(command);
  DiagramItem *item = moveCommand->myDiagramItem;

  if (myDiagramItem != item)
    return false;

  newPos = item->pos();
  setText(QObject::tr("Move %1")
          .arg(createCommandString(myDiagramItem, newPos)));

  return true;
}
//! [1]

//! [2]
void MoveCommand::undo()
{
  myDiagramItem->setPos(myOldPos);
  myDiagramItem->scene()->update();
  setText(QObject::tr("Move %1")
          .arg(createCommandString(myDiagramItem, newPos)));
}
//! [2]

//! [3]
void MoveCommand::redo()
{
  myDiagramItem->setPos(newPos);
  setText(QObject::tr("Move %1")
          .arg(createCommandString(myDiagramItem, newPos)));
}
//! [3]

//! [4]
DeleteCommand::DeleteCommand(DiagramScene *scene, int id, QUndoCommand *parent)
    : QUndoCommand(parent)
{
  myGraphicsScene = scene;
  myDiagramItem = myGraphicsScene->getDiagramItem(id);
  setText(QObject::tr("Delete %1")
          .arg(createCommandString(myDiagramItem, myDiagramItem->pos())));
}
//! [4]

//! [5]
void DeleteCommand::undo()
{
  myGraphicsScene->addDiagramItem(myDiagramItem);
  myGraphicsScene->update();
}
//! [5]

//! [6]
void DeleteCommand::redo()
{
  myGraphicsScene->removeDiagramItem(myDiagramItem->id());
}
//! [6]

//! [7]
AddCommand::AddCommand(DiagramItem::DiagramType addType, QPointF pos,
                       DiagramScene *scene, QUndoCommand *parent, int id,
                       QColor color)
    : QUndoCommand(parent)
{

  myGraphicsScene = scene;
  myDiagramItem = new DiagramItem(addType, 0, 0, color);
  myDiagramItem->setId(id);
  initialPosition = pos;
  setText(QObject::tr("Add %1")
          .arg(createCommandString(myDiagramItem, pos)));
}
//! [7]

//! [8]
void AddCommand::undo()
{
  myGraphicsScene->removeDiagramItem(myDiagramItem->id());
  myGraphicsScene->update();
}
//! [8]

//! [9]
void AddCommand::redo()
{
  myGraphicsScene->addDiagramItem(myDiagramItem);
  myDiagramItem->setPos(initialPosition);
  myGraphicsScene->clearSelection();
  myGraphicsScene->update();
}
//! [9]

QString createCommandString(DiagramItem *item, const QPointF &pos)
{
  return QObject::tr("%1 at (%2, %3)")
    .arg(item->diagramType() == DiagramItem::Box ? "Box" : "Triangle")
    .arg(pos.x()).arg(pos.y());
}
