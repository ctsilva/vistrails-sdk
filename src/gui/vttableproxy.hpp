/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VTTABLEPROXY_HPP
#define VTTABLEPROXY_HPP

#include <QAbstractProxyModel>

class VtModel;

class VtTableProxyModel : public QAbstractProxyModel
{
  Q_OBJECT

public:
  explicit VtTableProxyModel(VtModel *srcModel, QObject *parent=0);
  void setSourceModel(QAbstractItemModel *srcModel);

  virtual QModelIndex index(int row, int column, 
  			    const QModelIndex & parent=QModelIndex()) const;
  virtual QModelIndex mapFromSource(const QModelIndex &sourceIndex) const;
  virtual QModelIndex mapToSource(const QModelIndex &proxyIndex) const;
  virtual int rowCount(const QModelIndex &parent=QModelIndex()) const;
  virtual int columnCount(const QModelIndex &parent=QModelIndex()) const;
  virtual QModelIndex parent(const QModelIndex &index) const;
  virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  //bool hasChildren(const QModelIndex &parent) const;
  //bool setHeaderData(int section, Qt::Orientation orientation, 
  //		     const QVariant &value, int role);
  //void setHorizontalHeaderLabels(const QStringList &labels);
  virtual void clearModel();

private slots:
  void srcRowsInserted(QModelIndex idx, int start, int end);


private:
  VtModel* vtModel;
  //void load();
  //int getTotalChildCount(QModelIndex index=QModelIndex()) const;
  //QModelIndex findIndex(int row, int col, int startCount=0, 
  //			QModelIndex parent=QModelIndex()) const;
  
  //QList<QString *> uuidMap;
    
};

#endif
