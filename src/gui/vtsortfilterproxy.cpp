/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "vtsortfilterproxy.hpp"
#include "vttableproxy.hpp"
#include "vtmodel.hpp"
#include "vtwindow.hpp"

VtSortFilterProxy::VtSortFilterProxy(VtTableProxyModel *srcModel,
				     QObject *parent)
  : QSortFilterProxyModel(parent)
{
  this->setSourceModel(srcModel);
  this->bDate = false;
}

void VtSortFilterProxy::clearFilters()
{
  this->setFilterWildcard("");
  VisTrailsWindow::getInstance()->getTreeScene()->controller->clearFilter();
}

void VtSortFilterProxy::clearModel()
{
  this->beginResetModel();
  ((VtTableProxyModel*)this->sourceModel())->clearModel();
  this->endResetModel();
}

void VtSortFilterProxy::setDate(bool flag) { 
  this->bDate = flag;
}

bool VtSortFilterProxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const 
{
  if(this->bDate)
  {
    QModelIndex index = sourceModel()->index(source_row, VersionData::Date, source_parent);
    if(index.isValid())
    {
      VisTrailsWindow *mw = VisTrailsWindow::getInstance();
      return mw->dateInRange(index.data().toDateTime());
    }
  }
  return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
}
