/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VTSORTFILTERPROXY_HPP
#define VTSORTFILTERPROXY_HPP
#include <QtGui>
#include <QSortFilterProxyModel>

class VtTableProxyModel;

/*
  class VtSortFilterProxy
*/
class VtSortFilterProxy : public QSortFilterProxyModel
{
  Q_OBJECT

  public:
  explicit VtSortFilterProxy(VtTableProxyModel *srcModel, QObject *parent=0);
  void clearModel();
  void setDate(bool date);
public slots:
  void clearFilters();
protected:
  bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
private:
  bool bDate;
};

#endif
