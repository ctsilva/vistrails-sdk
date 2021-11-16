/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "vttableproxy.hpp"
#include <vector>
#include <iostream>
#include "vtmodel.hpp"
#include "vtnode.hpp"

VtTableProxyModel::VtTableProxyModel(VtModel *srcModel,
				     QObject *parent)
  : QAbstractProxyModel(parent)
{
  this->setSourceModel(srcModel);
}

void VtTableProxyModel::setSourceModel(QAbstractItemModel *srcModel)
{
  beginResetModel();
  QAbstractItemModel * curSrcModel = this->sourceModel();
  if (sourceModel()) {
    disconnect(sourceModel(), SIGNAL(rowsInserted(QModelIndex,int,int)),
	       this, SLOT(srcRowsInserted(QModelIndex,int,int)));
  }
  QAbstractProxyModel::setSourceModel(srcModel);
  connect(srcModel, SIGNAL(rowsInserted(QModelIndex,int,int)),
	  this, SLOT(srcRowsInserted(QModelIndex,int,int)));
  endResetModel();
  this->vtModel = (VtModel*)srcModel;
}

QVariant VtTableProxyModel::headerData(
  int section, Qt::Orientation orientation, int role) const
{
  return this->vtModel->headerData(section, orientation, role);
}

QModelIndex VtTableProxyModel::index(int row, int column, 
                                     const QModelIndex & parent) const
{
  if(vtModel->getItems().count() > row)
    return createIndex(row, column, vtModel->getItems()[row]);
  return QModelIndex();
}

QModelIndex VtTableProxyModel::parent(const QModelIndex &index) const
{
  return QModelIndex();
}

int VtTableProxyModel::rowCount(const QModelIndex &parent) const
{
  return vtModel->getItems().count();
}

int VtTableProxyModel::columnCount(const QModelIndex &parent) const
{
  return VersionData::Count;
}

QModelIndex VtTableProxyModel::mapToSource(const QModelIndex &proxyIndex) const
{
  if(!proxyIndex.isValid())
    return QModelIndex();

  VtNode* node = static_cast<VtNode*>(proxyIndex.internalPointer());
  return vtModel->index(node->getVersion(), proxyIndex.column());
}

QModelIndex VtTableProxyModel::mapFromSource(const QModelIndex & sourceIndex) const
{
  if(!sourceIndex.isValid())
    return QModelIndex();

  VtNode* node = static_cast<VtNode*>(sourceIndex.internalPointer());
  return createIndex(node->getRow(), sourceIndex.column(), node);
}

void VtTableProxyModel::srcRowsInserted(QModelIndex idx, int start, int end) {
  int num = end - start + 1;
  int count = this->vtModel->getItems().count();
  int mStart = count - num; 
  int mEnd = count - 1;
  
  this->beginInsertRows(QModelIndex(), mStart, mEnd);
  this->endInsertRows();
}

void VtTableProxyModel::clearModel()
{
  this->beginResetModel();
  this->vtModel->clearModel();
  this->endResetModel();
}
