/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef COMMANDS_H
#define COMMANDS_H

#include <QUndoCommand>
#include <QPointF>
#include <QString>
#include <QColor>

#include "diagramitem.h"
class DiagramScene;

//! [0]
class MoveCommand : public QUndoCommand
{
public:
  MoveCommand(DiagramItem *diagramItem, const QPointF &oldPos,
              QUndoCommand *parent = 0);

  void undo();
  void redo();
  bool mergeWith(const QUndoCommand *command);

  DiagramItem *myDiagramItem;
  QPointF myOldPos;
  QPointF newPos;
};
//! [0]

//! [1]
class DeleteCommand : public QUndoCommand
{
public:
  DeleteCommand(DiagramScene *graphicsScene, int id, QUndoCommand *parent = 0);

  void undo();
  void redo();

  DiagramItem *myDiagramItem;
  DiagramScene *myGraphicsScene;
};
//! [1]

//! [2]
class AddCommand : public QUndoCommand
{
public:
  AddCommand(DiagramItem::DiagramType addType, QPointF pos, DiagramScene *graphicsScene,
             QUndoCommand *parent = 0, int id=-1, QColor color=Qt::transparent);

  void undo();
  void redo();

  DiagramItem *myDiagramItem;
  DiagramScene *myGraphicsScene;
  QPointF initialPosition;
};
//! [2]

QString createCommandString(DiagramItem *item, const QPointF &point);

#endif
