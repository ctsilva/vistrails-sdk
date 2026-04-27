/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VTNODE_H
#define VTNODE_H

#include <QList>
#include <QVariant>
#include <QVector>
#include <QIcon>
#include <vector>
namespace vt { class Version; }

class VtNode
{
public:
  VtNode(vt::Version* v, int row = -1, VtNode *parent = 0);
  ~VtNode();

  VtNode *child(int number);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column, int role) const;
  void addChild(vt::Version* v, int row = -1);
  VtNode *parent();
  bool removeChildren(int position, int count);
  int childNumber() const;
  bool setData(int column, const QVariant &value);
  vt::Version* getVersion() { return version; }
  int getRow() const;
  void setRow(int row);

private:
  std::vector<VtNode*> childItems;
  QVector<QVariant> itemData;
  VtNode *parentItem;
  vt::Version *version;
  mutable QIcon *icon;
  int childNum;

  QVariant getThumbnail() const;
  QVariant getDate() const;
  bool setDate(const QVariant &value);
  int row;
};

#endif
