/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef DIRDELEGATE_HPP
#define DIRDELEGATE_HPP

#include <QtGui>
#include "VisTrails.h"

class ListDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit ListDelegate(QObject *parent=0);
  virtual ~ListDelegate();

  enum datarole 
  {
    idHeader = Qt::EditRole,
    tagHeader = Qt::EditRole+100,
    actionHeader = Qt::UserRole+101,
    dateHeader = Qt::UserRole+102,
    userHeader = Qt::UserRole+103,
    IconRole = Qt::UserRole+104,
    parentIDHeader = Qt::UserRole+105
  };

  bool editorEvent(QEvent *event,
    QAbstractItemModel *model, const QStyleOptionViewItem &option,
    const QModelIndex &index);

  QSize sizeHint(const QStyleOptionViewItem &option,
    const QModelIndex &index ) const;

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
    const QModelIndex &index) const;

  quint16 setColor(const QModelIndex &index) const;
};

class TableDelegate : public QStyledItemDelegate
{
  Q_OBJECT
public:
  explicit TableDelegate(QObject *parent=0);
  virtual ~TableDelegate();

  bool editorEvent(QEvent *event,
    QAbstractItemModel *model, const QStyleOptionViewItem &option,
    const QModelIndex &index);

  QSize sizeHint(const QStyleOptionViewItem &option,
    const QModelIndex &index ) const;

  void paint(QPainter *painter, const QStyleOptionViewItem &option,
    const QModelIndex &index) const;
};

#endif
