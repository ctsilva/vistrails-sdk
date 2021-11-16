/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef __VISTRAILCONTROLLER_H__
#define __VISTRAILCONTROLLER_H__

#include "VisTrails.h"
#include <QObject>
#include <QDateTime>
#include <QPoint>

class QGraphicsVersionItem;
class QGraphicsLinkItem;
class QVersionTreeScene;

enum VersionChildren
{
  ExactlyOneChild,
  MoreThanOneChild,
  NoChildren
};

class VersionTreeLayoutProperties: public vt::LayoutProperties
{
public:
  ~VersionTreeLayoutProperties();
};

class VistrailController: public QObject, 
  public vt::VersionTreeLayout,
  public vt::LayoutProperties
{
  Q_OBJECT

public:
  VistrailController(QVersionTreeScene *scene);
  ~VistrailController();

  void setSessionFocus(vt::SessionFocus *focus);
  vt::SessionFocus * getSessionFocus();

  bool mustShow(const vt::VersionId &id);
  bool mustHide(const vt::VersionId &id);
  void rebuildTerseTree();
  vt::VersionTree *getTerseTree();
  vt::VersionTree *getFullTree();

  // LayoutProperties methods
  void setVersionPos(const vt::VersionId & id, float X, float Y);
  float siblingDistance(const vt::VersionId &l, const vt::VersionId &r);

  // VistrailController methods
  VersionChildren getChildEnum(const vt::VersionId &id);
  bool inLatest(const vt::VersionId &id);
  bool inFilter(const vt::VersionId &id);

  // Expand all versions between start and end
  void expandVersions(const vt::VersionId &start, const vt::VersionId &end);

  // Collapse all version including and below id until next tag or branch
  void collapseVersions(vt::VersionId id);

  void clearExpandCollapseState(const vt::VersionId &id);

  void setSelected(const vt::VersionId &id);
  void addToLatest(const vt::VersionId &id);
  void addToFilter(const vt::VersionId &id);
  void clearFilter();
  void filterAnnotation(const char* key, const QString &filter);
  void filterStamp(const char* key, const QString &filter);
  void filterDate(QDateTime start, QDateTime end);
  void rebuildAndLayout();
  void removeLink(const vt::VersionId &childId);
  void removeVersion(const vt::VersionId &id);
  void addNode(const vt::VersionId &id, QGraphicsVersionItem *versionShape);
  void addEdge(const vt::VersionId &childId, QGraphicsLinkItem *linkShape);
  QGraphicsVersionItem *findNode(const vt::VersionId &id);
  QGraphicsLinkItem *findEdge(const vt::VersionId &childId);
  void clear();
  void reset(); //resets fullTree and terseTree
  void setPlaybackStart(const vt::VersionId &id);
  void setPlaybackEnd(const vt::VersionId &id);
  bool animate_layout;
  bool changed;
  bool playback;
  QGraphicsVersionItem *playbackStart;
  QGraphicsVersionItem *playbackEnd;

private:
  class PrivateImpl;
  PrivateImpl *self;

  vt::VersionTree *terseTree;
  QVersionTreeScene *treeScene;
  vt::SessionFocus *sessionFocus;
  
  bool show_full_tree;
};

#endif
