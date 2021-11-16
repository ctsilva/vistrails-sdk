/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QtGui>

#include "delegate.hpp"
#include "preferencesdialog.hpp"
#include "info.hpp"
#include "galleryview.hpp"
#include "vtmodel.hpp"
#include "vtnode.hpp"
#include "vttableproxy.hpp"
#include "vtsortfilterproxy.hpp"
#include "vtwindow.hpp"

static const int THUMBNAIL_SIZE = 88;

ListDelegate::ListDelegate(QObject *parent) : QStyledItemDelegate(parent)
{
}

/*
  editorEvent(QEvent, QAbstractItemModel, QStyleOptionViewItem, QModelIndex)
  Here we handle the left click on the custom-dir-view
*/
bool ListDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
    QMouseEvent *k = (QMouseEvent*)event;
    VisTrailsWindow *mw = VisTrailsWindow::getInstance();
    if (k->button() == Qt::LeftButton)
    { 
      PlaybackSelectionMode mode = mw->getPlaybackSelectionMode();
      if(mode == NotSelectingPlayback)
        return false;

      QModelIndex tableIdx = ((QAbstractProxyModel*)model)->mapToSource(index);
      VtNode* node = static_cast<VtNode*>(tableIdx.internalPointer());
      if(mode == SelectPlaybackStart)
      {
        mw->setPlaybackStart(node->getVersion()->id());
        return false; // propogate mouse click
      }
      if(mode == SelectPlaybackEnd)
      {
        mw->setPlaybackEnd(node->getVersion()->id());
        return true; // don't propogate (don't want to perform regular selection on this item)
      }
    }
    if (k->button() == Qt::RightButton)
    {
      if(index.isValid())
      {
        QModelIndex tableIdx = ((QAbstractProxyModel*)model)->mapToSource(index);
        if(tableIdx.isValid())
        {
          VtNode* node = static_cast<VtNode*>(tableIdx.internalPointer());
          mw->getVersionContextMenu()->setVersion(node->getVersion()->id());
          mw->getVersionContextMenu()->popup(k->globalPos());
          mw->getVersionContextMenu()->show();
        }
      }
      return true; // don't select on right click
    }
  }
  return false;
}


/*
  sizeHint(QStyleOptionViewItem, QModelIndex)
  alocate each item size in the dir-view.
*/
QSize ListDelegate::sizeHint(const QStyleOptionViewItem & option ,
                            const QModelIndex & index) const
{
  if (index.column() != 0)
    return QSize(0,0);

  QSize theSize = QStyledItemDelegate::sizeHint(option, index);
  theSize.setHeight(THUMBNAIL_SIZE);
  return theSize;
}


/*
  setColor(const QModelIndex &index) const
  Set the transparency for the dir-view
*/
quint16 ListDelegate::setColor(const QModelIndex &index) const
{
  QSettings *settings = VisTrailsWindow::getInstance()->getSettings();	     
  bool temp = settings->value("listColorState", true).toBool();

  quint16 alpha = index.row();
  if (alpha == 0)
    alpha = 1.2;
  if(temp) {	
    if (alpha > 55)
      alpha = 255-66;
    else
      alpha = 255 - alpha*2.2;
  }
  return alpha;
}


/*
  paint(QPainter, QStyleOptionViewItem, QModelIndex)
  Create custom look for directory view.
*/
void ListDelegate::paint(QPainter *painter, 
                        const QStyleOptionViewItem &option,
                        const QModelIndex &index) const
{    
  if (index.column() != VersionData::Thumb) {
    return;
  } 
  
  if (option.state && QStyle::State_HasFocus && !index.parent().isValid())
  {
    painter->save();

    QPen pen = QPen(Qt::black);

    pen.setWidth(2);
    painter->setPen(pen);
    if (option.state & QStyle::State_Selected)
      painter->setBrush(QBrush(Qt::darkCyan));
    else
      painter->setBrush(QBrush(QColor(100,150,237,255)));
    painter->drawRect(option.rect.adjusted(-18,-1,-1,-1));
    painter->restore();
  }
  else if (option.state && QStyle::State_HasFocus && index.parent().isValid())
  {
    if (index.sibling(index.row(), index.column()).isValid())
    {
      painter->save();
      QPen pen = QPen(Qt::green);

      // const quint16 childAlpha = setColor(index);
      QBrush cBrush = QBrush(QColor(41,171,135,255));

      pen.setWidth(1);
      painter->setPen(pen);
      painter->setBrush(cBrush);

      //Set the left border to origin for all child
      int tempX = -(option.rect.x());
      painter->drawRect(option.rect.adjusted(tempX,-1,-1,-1));
      painter->restore();
    }
  }

  //draw thumbnail centered
  QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    
  QSize iconSize;
  if(!icon.availableSizes().isEmpty())
    iconSize = icon.availableSizes()[0];

  int iconTop = option.rect.top();
  int iconLeft = 0;
  if (option.rect.height() > iconSize.height())
    iconTop += (option.rect.height() - iconSize.height()) / 2.0;
  if(iconSize.width() < THUMBNAIL_SIZE)
    iconLeft = (THUMBNAIL_SIZE - iconSize.width()) / 2.0;
  icon.paint(painter, iconLeft, iconTop, iconSize.width(),
             iconSize.height(), Qt::AlignCenter, QIcon::Normal, QIcon::On);

  painter->save();

  QString sTag = index.sibling(index.row(), VersionData::Tag).data(Qt::DisplayRole).toString();
  QString sAction = index.sibling(index.row(), VersionData::Action).data(Qt::DisplayRole).toString();
  QString sDate = index.sibling(index.row(), VersionData::Date).data(Qt::DisplayRole).toDateTime()
    .toString("yyyy-M-d hh:mm:ss ap");
  QString sUser = index.sibling(index.row(), VersionData::User).data(Qt::DisplayRole).toString();    

  QFont tagFont = QApplication::font();
  if (sTag != "<untagged>") {
    tagFont.setBold(true);
  }
  QFontMetrics boldMetrics(tagFont);    
  painter->setFont(tagFont);
  int textWidth = option.rect.width() - THUMBNAIL_SIZE - 12;
  painter->drawText(QRectF(THUMBNAIL_SIZE + 7, 
                           option.rect.top() + 2,
                           textWidth,
                           boldMetrics.height()),
                    boldMetrics.elidedText(sTag, Qt::ElideRight, 
                                           textWidth));
    
  QFont regFont = QApplication::font();
  QFontMetrics regMetrics(regFont);
  painter->setFont(regFont);
  QList<QString> otherText;
  otherText << sAction << sDate << sUser;
  for(int i = 0; i < otherText.size(); i++) {
    int top = option.rect.top() + 2 + boldMetrics.height() + \
      regMetrics.height() * i;
    painter->drawText(QRectF(THUMBNAIL_SIZE + 7, 
                             top,
                             textWidth,
                             regMetrics.height()),
                      regMetrics.elidedText(otherText[i], Qt::ElideRight,
                                            textWidth));
  }
    
  // draw playback borders
  VisTrailsWindow *mw = VisTrailsWindow::getInstance();
  if(mw->playback())
  {
    static const QPen greenPen = QPen(QBrush(Qt::green), 2);
    static const QPen redPen = QPen(QBrush(Qt::red), 2);

    bool drawBorder = false;

    QModelIndex tableIdx = ((QAbstractProxyModel*)index.model())->mapToSource(index);
    if(tableIdx.isValid())
    {
      VtNode* node = static_cast<VtNode*>(tableIdx.internalPointer());

      if (node->getVersion()->id() == mw->getPlaybackStart())
      {
        painter->setPen(greenPen);
        drawBorder = true;
      }
      else if (node->getVersion()->id() == mw->getPlaybackEnd())
      {
        painter->setPen(redPen);
        drawBorder = true;
      }
    }

    if(drawBorder)
      painter->drawRect(option.rect.adjusted(1,1,-1,-3));
  }

  painter->restore();
}


ListDelegate::~ListDelegate()
{ }


TableDelegate::TableDelegate(QObject *parent) : QStyledItemDelegate(parent)
{ }

TableDelegate::~TableDelegate()
{ }

QSize TableDelegate::sizeHint(const QStyleOptionViewItem & option ,
                            const QModelIndex & index) const
{
  if(index.column() == 0)
    return QSize(THUMBNAIL_SIZE, THUMBNAIL_SIZE);
  else
    return QStyledItemDelegate::sizeHint(option, index);
}

bool TableDelegate::editorEvent(QEvent *event, 
                                QAbstractItemModel *model, 
                                const QStyleOptionViewItem &option, 
                                const QModelIndex &index)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
    QMouseEvent *k = (QMouseEvent*)event;
    VisTrailsWindow *mw = VisTrailsWindow::getInstance();
    vt::VersionId id = vt::VERSION_INVALID;
    if(index.isValid())
    {
      QModelIndex tableIdx = ((QAbstractProxyModel*)index.model())->mapToSource(index);
      if(tableIdx.isValid())
      {
        VtNode* node = static_cast<VtNode*>(tableIdx.internalPointer());
        id = node->getVersion()->id();
      }
    }
    
    if (k->button() == Qt::LeftButton)
    {
      PlaybackSelectionMode mode = mw->getPlaybackSelectionMode();
      if(mode == NotSelectingPlayback)
        return false;

      if(mode == SelectPlaybackStart)
      {
        mw->setPlaybackStart(id);
        return false; // mouse click not handled
      }
      if(mode == SelectPlaybackEnd)
      {
        mw->setPlaybackEnd(id);
        return true; // mouse click handled (don't want to perform regular selection on this item)
      }
    }
    else if (k->button() == Qt::RightButton)
    {
      mw->getVersionContextMenu()->setVersion(id);
      mw->getVersionContextMenu()->popup(k->globalPos());
      mw->getVersionContextMenu()->show();
      return true; // don't select on right click
    }
  }
  return false;
}

void TableDelegate::paint(QPainter *painter, 
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
  //draw base item
  if(index.column() == VersionData::Thumb) { //custom draw image to properly center it
    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    
    QSize iconSize;
    if(!icon.availableSizes().isEmpty())
      iconSize = icon.availableSizes()[0];

    int iconTop = option.rect.top();
    int iconLeft = 0;
    int iconBottom = option.rect.height() < iconSize.height() ? option.rect.height() : iconSize.height();
    if (option.rect.height() > iconSize.height())
      iconTop += (option.rect.height() - iconSize.height()) / 2.0;
    if(iconSize.width() < THUMBNAIL_SIZE)
      iconLeft = (THUMBNAIL_SIZE - iconSize.width()) / 2.0;
    icon.paint(painter, iconLeft, iconTop, iconSize.width(),
	       iconBottom);
  }
  else
    QStyledItemDelegate::paint(painter, option, index);

  //draw playback border if needed
  VisTrailsWindow *mw = VisTrailsWindow::getInstance();
  if(index.column() == VersionData::Thumb && mw->playback())
  {
    static const QPen greenPen = QPen(QBrush(Qt::green), 2);
    static const QPen redPen = QPen(QBrush(Qt::red), 2);

    painter->save();

    bool isPlaybackNode = false;
    vt::VersionId id = vt::VERSION_INVALID;
    if(index.isValid())
    {
      QModelIndex tableIdx = ((QAbstractProxyModel*)index.model())->mapToSource(index);
      if(tableIdx.isValid())
      {
        VtNode* node = static_cast<VtNode*>(tableIdx.internalPointer());
        id = node->getVersion()->id();
      }
    }
    if (id == mw->getPlaybackStart())
    {
      painter->setPen(greenPen);
      isPlaybackNode = true;
    }
    else if (id == mw->getPlaybackEnd())
    {
      painter->setPen(redPen);
      isPlaybackNode = true;
    }
    if(isPlaybackNode)
      painter->drawRect(option.rect.adjusted(1,1,-1,-1));

    painter->restore();
  }
}
