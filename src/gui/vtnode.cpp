/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QStringList>
#include <QIcon>
#include <QDateTime>
#include <QTimeZone>
#include <QVariant>

#include "vtnode.hpp"
#include "vtmodel.hpp"


#include "VisTrails.h"
#include "vtwindow.hpp"

#include <iostream>

inline QString val_or_def(vt::PDict* dict, const char *key, const char *deflt)
{
  vt::Property *p = dict->get(key);
  if(p)
    return QString(p->toString().asChar());
  else
    return QString(deflt);
}

VtNode::VtNode(vt::Version *v, int row, VtNode *parent)
{
  this->parentItem = parent;
  this->version = v;
  this->childNum = 0;
  this->icon = NULL;
  this->row = row;
}

VtNode::~VtNode()
{
  for(; !childItems.empty(); childItems.pop_back())
    delete childItems.back();
}

VtNode *VtNode::child(int number)
{
  if(number < childItems.size() && number >= 0)
    return childItems[number];
  return NULL;
}

int VtNode::childCount() const
{
  return childItems.size();
}

int VtNode::childNumber() const
{
  return childNum;
}

int VtNode::columnCount() const
{
  return VersionData::Count;
}

QVariant VtNode::data(int column, int role) const
{
  if(parentItem == NULL)
    return QVariant();

  switch(role)
  {
  case Qt::DisplayRole:
    switch(column)
    {
    case VersionData::Tag:
      return val_or_def(version->annotations(), "tag", "<untagged>");
    case VersionData::Action:
      return val_or_def(version->annotations(), "label", "<unknown>");
    case VersionData::User:
      return val_or_def(version->stamp(), "user", "<unknown>");
    case VersionData::Date:
      return this->getDate();
    }
    break;
  case Qt::DecorationRole:
    switch(column)
    {
    case VersionData::Thumb:
      return this->getThumbnail();
    }
    break;
  }
  return QVariant();
}

QVariant VtNode::getThumbnail() const
{
  if(this->icon == NULL)
  {
    this->icon = new QIcon();
    QString thumb = val_or_def(version->annotations(), "thumb", "");
    if(!thumb.isEmpty())
    {
      QPixmap pixThumb(VisTrailsWindow::getInstance()->thumbnailDir() + thumb);
      this->icon->addPixmap(pixThumb);
    }
  }
  return *this->icon;
}

QVariant VtNode::getDate() const
{
  time_t t = version->stamp()->get("date")->asDateTime()->toTime();
  QDateTime qtime = QDateTime::fromSecsSinceEpoch(t);
  qtime.setTimeZone(QTimeZone::UTC);
  return qtime.toLocalTime();
}

void VtNode::addChild(vt::Version* v, int row)
{
  childItems.push_back(new VtNode(v, row, this));
  childItems.back()->childNum = childItems.size() - 1;
}

VtNode *VtNode::parent()
{
  return parentItem;
}

bool VtNode::removeChildren(int position, int count)
{
  if (position < 0 || position + count > childItems.size())
    return false;

  for (int i = 0; i < count; ++i)
  {
    delete childItems[position];
    childItems.erase(childItems.begin()+position);
  }

  for(int i = position; i < childItems.size(); ++i)
    childItems[i]->childNum = i;

  return true;
}

inline void setOrRemoveStr(vt::PDict* d, const char* key, const QString& val)
{
  if(val.isEmpty())
    d->remove(key);
  else
  {
    QByteArray bytes = val.toUtf8();
    d->setString(key, bytes.constData());
  }
}

bool VtNode::setData(int column, const QVariant &value)
{
  if(parentItem == NULL)
    return false;

  switch(column)
  {
  case VersionData::Thumb:
    this->icon = NULL;
    setOrRemoveStr(version->annotations(), "thumb", value.toString());
    break;
  case VersionData::Tag:
    setOrRemoveStr(version->annotations(), "tag", value.toString());
    break;
  case VersionData::Action:
    setOrRemoveStr(version->annotations(), "label", value.toString());
    break;
  case VersionData::User:
    setOrRemoveStr(version->stamp(), "user", value.toString());
    break;
  case VersionData::Date:
    return this->setDate(value);
  default:
    return false;
  }
  return true;
}

bool VtNode::setDate(const QVariant &value)
{
  QDateTime qtime = QDateTime::fromString(value.toString(), "yyyy-M-d hh:mm:ss ap");
  qtime.setTimeZone(QTimeZone::LocalTime);
  version->stamp()->setDateTime("date", qtime.toUTC().toSecsSinceEpoch());
  return true;
}

int VtNode::getRow() const
{
  return row;
}

void VtNode::setRow(int row)
{
  this->row = row;
}
