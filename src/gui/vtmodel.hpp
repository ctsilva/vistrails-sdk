/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QPersistentModelIndex>
#include <QVariant>
#include <QVector>

class VtNode;

namespace vt { class Vistrail; class Version;}

namespace VersionData
{
  enum Enum
  {
    Thumb = 0,
    Tag,
    Action,
    Date,
    User,
    Count
  };
}

class VtModel : public QAbstractItemModel
{
  Q_OBJECT

public:
  VtModel(QObject *parent = 0);
  ~VtModel();

  void setVistrail(vt::Vistrail *v);
  QVariant data(const QModelIndex &index, int role) const;
  QVariant headerData(int section, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;

  QModelIndex index(int row, int column,
                    const QModelIndex &parent = QModelIndex()) const;
  QModelIndex index(int row, int column, VtNode* parentItem) const;
  QModelIndex index(vt::Version* v, int column = 0) const;
  QModelIndex parent(const QModelIndex &index) const;

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  int columnCount(const QModelIndex &parent = QModelIndex()) const;

  Qt::ItemFlags flags(const QModelIndex &index) const;
  bool setData(const QModelIndex &index, const QVariant &value,
               int role = Qt::EditRole);
  bool setHeaderData(int section, Qt::Orientation orientation,
                     const QVariant &value, int role = Qt::EditRole);

  void addVersion(vt::Version *v);
  void addChild(vt::Version *v, const QModelIndex &parent = QModelIndex());
  bool removeRows(int position, int rows,
                  const QModelIndex &parent = QModelIndex());


  void setThumbnailDir(QString dir);
  QString thumbnailDir();

  VtNode *getItem(const QModelIndex &index) const;
  vt::Version *getVersion(const QModelIndex &index) const;
  void clearModel();

  const QList<VtNode*>& getItems() const;

private:
  void setupModelData();
  void clear();

  VtNode *rootItem;
  vt::Vistrail *vistrail;
  QVector<QVariant> headers;
  QHash<vt::Version*, QPersistentModelIndex> indices;
  QString thumbDir;
  QList<VtNode*> items;

};

#endif
