/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QtGui>
#include <QVariant>

#include "vtnode.hpp"
#include "vtmodel.hpp"

#include "VisTrails.h"

VtModel::VtModel(QObject *parent)
  : QAbstractItemModel(parent)
{  
  this->headers << tr("Thumb") << tr("Tag") << tr("Action") << tr("Date") << tr("User");

  rootItem = new VtNode(NULL);
}

VtModel::~VtModel()
{
  delete rootItem;
}

void VtModel::setVistrail(vt::Vistrail* v)
{
  this->clear();
  this->vistrail = v;
  setupModelData();
}

int VtModel::columnCount(const QModelIndex & /* parent */) const
{
  return rootItem->columnCount();
}

QVariant VtModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  VtNode *item = getItem(index);

  return item->data(index.column(), role);
}

Qt::ItemFlags VtModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

VtNode *VtModel::getItem(const QModelIndex &index) const
{
  if (index.isValid()) {
    VtNode *item = static_cast<VtNode*>(index.internalPointer());
    if (item) return item;
  }
  return rootItem;
}

vt::Version *VtModel::getVersion(const QModelIndex &index) const
{
  return getItem(index)->getVersion();
}

QVariant VtModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    return this->headers.value(section);

  return QVariant();
}

QModelIndex VtModel::index(int row, int column, const QModelIndex &parent) const
{
  if (parent.isValid() && parent.column() != 0)
    return QModelIndex();

  return index(row, column, getItem(parent));
}

QModelIndex VtModel::index(int row, int column, VtNode* parentItem) const
{
  VtNode *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex VtModel::index(vt::Version *v, int column) const
{
  if(indices.contains(v))
    return createIndex(indices[v].row(), column, indices[v].internalPointer());
  return QModelIndex();
}

void VtModel::addVersion(vt::Version *v)
{
  if(v->parent() != vt::VERSION_ROOT)
    this->addChild(v, index(vistrail->getVersion(v->parent())));
  else
    this->addChild(v, QModelIndex());
}


void VtModel::addChild(vt::Version *v, const QModelIndex &parent)
{
  VtNode *parentItem = getItem(parent);
  int pos = parentItem->childCount();

  beginInsertRows(parent, pos, pos);
  parentItem->addChild(v, items.count());
  indices.insert(v, QPersistentModelIndex(this->index(pos,0,parentItem)));
  items.push_back(parentItem->child(pos));
  endInsertRows();
}

QModelIndex VtModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  VtNode *childItem = getItem(index);
  VtNode *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool VtModel::removeRows(int position, int rows, const QModelIndex &parent)
{
  VtNode *parentItem = getItem(parent);

  if(!parentItem || position < 0 || parentItem->childCount() < position + rows)
    return false;
  
  bool success = true;
  int tableRow = parentItem->child(position)->getRow();

  //walk tree from each child and accumulate all rows to be removed
  QMap<int,bool> rowMap;
  vt::VersionTree *tree = vistrail->versionTree();
  vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(tree);
  for(int i = position; i < position + rows; ++i)
  {
    VtNode* child = parentItem->child(i);
    traversal.traverse(child->getVersion()->id(), vt::TRAVERSE_DFS);

    // insert all version rows
    for(; !traversal.done(); traversal.next())
    {
      QModelIndex idx = index(vistrail->getVersion(traversal.current()));
      VtNode* node = static_cast<VtNode*>(idx.internalPointer());
      rowMap.insert(node->getRow(), true);
    }
  }

  //sort rows in descending order to ensure child nodes are removed before parent nodes
  QList<int> rowList = rowMap.keys(); //keys of map are auto sorted ascending

  //remove rows one at a time
  for(int i = rowList.count() - 1; i > -1; --i)
  {
    int r = rowList[i];
    VtNode* node = items.takeAt(r);
    QModelIndex p = this->parent(indices[node->getVersion()]);
    beginRemoveRows(p, node->childNumber(), node->childNumber());
    node->parent()->removeChildren(node->childNumber(), 1);
    endRemoveRows();
  }

  //reset row values for items
  for(int i = 0; i < items.count(); ++i)
    items[i]->setRow(i);

  return success;
}

int VtModel::rowCount(const QModelIndex &parent) const
{
  VtNode *parentItem = getItem(parent);

  return parentItem->childCount();
}

bool VtModel::setData(const QModelIndex &index, const QVariant &value,
                        int role)
{
  if (role != Qt::EditRole)
    return false;

  VtNode *item = getItem(index);
  bool result = item->setData(index.column(), value);

  if (result)
    emit dataChanged(index, index);

  return result;
}

bool VtModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
  if (role != Qt::EditRole || orientation != Qt::Horizontal)
    return false;

  if(section < headers.size())
  {
    headers[section] = value;
    emit headerDataChanged(orientation, section, section);
    return true;
  }
  return false;
}

void VtModel::setupModelData()
{
  // set up traversal
  vt::VersionTree *tree = vistrail->versionTree();
  vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(tree);
  traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_DFS);
  traversal.next(); //skip version root

  // add all versions
  for(; !traversal.done(); traversal.next())
  {
    vt::Version *v = vistrail->getVersion(traversal.current());
    QModelIndex parent = index(vistrail->getVersion(v->parent()));
    this->addChild(v, parent);
  }
}

void VtModel::setThumbnailDir(QString dir)
{
  this->thumbDir = dir;
}

QString VtModel::thumbnailDir()
{
  return this->thumbDir;
}

void VtModel::clearModel()
{
  this->beginResetModel();
  this->clear();
  this->endResetModel();
}

void VtModel::clear()
{
  delete rootItem;
  rootItem = new VtNode(NULL);
  items.clear();
  indices.clear();
}

const QList<VtNode*>& VtModel::getItems() const
{
  return items;
}
