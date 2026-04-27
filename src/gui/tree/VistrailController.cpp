/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "VistrailController.h"
#include "Theme.h"
#include "vtwindow.hpp"
#include <queue>
#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

typedef boost::unordered_map<vt::VersionId, QGraphicsVersionItem*> NodeMap;
typedef boost::unordered_map<vt::VersionId, QGraphicsLinkItem*>  LinkMap;
typedef boost::unordered_set<vt::VersionId> VersionSet;

const static int NUM_LATEST = 6;

class VistrailController::PrivateImpl
{
public:
  NodeMap versions;
  LinkMap edges;
  VersionSet expanded, collapsed, filter, latest_set;
  std::queue<vt::VersionId> latest_queue;
};

void VistrailController::setVersionPos(const vt::VersionId & id,
                                                float X, float Y)
{
  NodeMap::iterator iter = this->self->versions.find(id);
  if(iter != this->self->versions.end())
  {
    float width = iter->second->width;
    float height = iter->second->height;
    float topLeftX = X - width / 2;
    float topLeftY = Y * 75 - height / 2;
    iter->second->setRect(topLeftX, topLeftY, width, height);
  }
}

void VistrailController::removeVersion(const vt::VersionId &id)
{
  NodeMap::iterator iter = this->self->versions.find(id);
  if(iter != this->self->versions.end())
  {
    this->treeScene->removeItem(iter->second);
    delete iter->second;
    iter->second = NULL;
    this->self->versions.erase(iter);
  }
}

void VistrailController::clear()
{
  NodeMap::iterator iter1 = this->self->versions.begin();
  while(iter1 != this->self->versions.end())
  {
    this->treeScene->removeItem(iter1->second);
    delete iter1->second;
    iter1->second = NULL;
    iter1 = this->self->versions.erase(iter1);
  }

  LinkMap::iterator iter2 = this->self->edges.begin();
  while(iter2 != this->self->edges.end())
  {
    this->treeScene->removeItem(iter2->second);
    delete iter2->second;
    iter2->second = NULL;
    iter2 = this->self->edges.erase(iter2);
  }
}

void VistrailController::removeLink(const vt::VersionId &childId)
{
  LinkMap::iterator iter = this->self->edges.find(childId);
  if(iter != this->self->edges.end())
  {
    this->treeScene->removeItem(iter->second);
    delete iter->second;
    iter->second = NULL;
    this->self->edges.erase(iter);
  }
}


QGraphicsLinkItem *VistrailController::findEdge(
  const vt::VersionId &childId)
{
  LinkMap::iterator iter = this->self->edges.find(childId);
  if(iter != this->self->edges.end())
    return iter->second;
  else
    return NULL;
}


void VistrailController::addEdge(const vt::VersionId &childId,
                                 QGraphicsLinkItem *linkShape)
{
  this->self->edges[childId] = linkShape;
}


void VistrailController::addNode(const vt::VersionId &id,
                                 QGraphicsVersionItem *versionShape)
{
  this->self->versions[id] = versionShape;
}


QGraphicsVersionItem* VistrailController::findNode(
  const vt::VersionId &id)
{
  NodeMap::iterator iter = this->self->versions.find(id);
  if(iter != this->self->versions.end())
    return iter->second;
  else
    return NULL;
}


float VistrailController::siblingDistance(const vt::VersionId &l,
                                                   const vt::VersionId &r)
{
  NodeMap::iterator itLeft = this->self->versions.find(l);
  NodeMap::iterator itRight = this->self->versions.find(r);
  if(itLeft != this->self->versions.end() && itRight != this->self->versions.end())
  {
    return itLeft->second->width / 2 + itRight->second->width / 2 + 5;
  }
  return 0;
}

void VistrailController::reset()
{
  delete this->terseTree;
  this->terseTree = new vt::VersionTree();
}

VistrailController::VistrailController(QVersionTreeScene *scene)
  : VersionTreeLayout(), self(new PrivateImpl)
{
  this->treeScene = scene;
  this->sessionFocus = NULL;
  this->terseTree = new vt::VersionTree();
  this->show_full_tree = false;
  this->playback = false;
}


VistrailController::~VistrailController()
{
  delete this->terseTree;
  this->terseTree = NULL;
}

void VistrailController::setPlaybackStart(const vt::VersionId &id)
{
  this->playbackStart = this->findNode(id);
}

void VistrailController::setPlaybackEnd(const vt::VersionId &id)
{
  this->playbackEnd = this->findNode(id);
}

void VistrailController::setSessionFocus(vt::SessionFocus *focus)
{
  this->sessionFocus = focus;
}

vt::SessionFocus * VistrailController::getSessionFocus()
{
  return this->sessionFocus;
}

void VistrailController::rebuildAndLayout()
{
  this->rebuildTerseTree();
  this->layout(this->getTerseTree(), this);
}

bool VistrailController::mustShow(const vt::VersionId &id)
{
  vt::Version *version = this->sessionFocus->session()->vistrail()
    ->getVersion(id);
  if(show_full_tree ||
    id == vt::VERSION_ROOT ||
    this->getChildEnum(id) != ExactlyOneChild ||
    this->self->expanded.find(id) != this->self->expanded.end() ||
    id == this->sessionFocus->currentVersion() ||
    this->inLatest(id) ||
    (version && version->annotations()->get("tag")) ||
    this->inFilter(id))
  {
    return true;
  }
  return false;
}

bool VistrailController::mustHide(const vt::VersionId &id)
{
  if(this->self->collapsed.find(id) != this->self->collapsed.end() &&
    id != this->sessionFocus->currentVersion()
    && !this->inFilter(id))
    return true;
  return false;
}

void VistrailController::setSelected(const vt::VersionId &id)
{
  NodeMap::iterator it = this->self->versions.find(id);
  if(it != this->self->versions.end())
  {
    it->second->setSelected(true);
    if(this->self->collapsed.find(id) != this->self->collapsed.end())
    {
      this->self->collapsed.erase(id);

      // DAK not sure why we have to wait to do this...
      //if(VisTrailsWindow::getInstance()->getCurrentViewIndex() == 0) //tree view
      this->treeScene->setupScene();
    }
  }
}

void VistrailController::expandVersions(const vt::VersionId &start,
                                        const vt::VersionId &end)
{
  vt::VersionId id = this->getFullTree()->getParent(end);
  while(id != start)
  {
    if(this->self->collapsed.find(id) != this->self->collapsed.end())
      this->self->collapsed.erase(id);
    this->self->expanded.insert(id);
    id = this->getFullTree()->getParent(id);
  }
}

void VistrailController::collapseVersions(vt::VersionId id)
{
  vt::VersionId current = this->sessionFocus->currentVersion();
  while(id != vt::VERSION_INVALID)
  {
    if(this->self->expanded.find(id) != this->self->expanded.end())
      this->self->expanded.erase(id);
    if(!this->mustShow(id))
    {
      this->self->collapsed.insert(id);
      id = this->getFullTree()->getFirstChild(id);
    }
    else
      break;
  }
}

void VistrailController::clearExpandCollapseState(const vt::VersionId &id)
{
  if(this->self->expanded.find(id) != this->self->expanded.end())
    this->self->expanded.erase(id);

  if(this->self->collapsed.find(id) != this->self->collapsed.end())
    this->self->collapsed.erase(id);
}

VersionChildren VistrailController::getChildEnum(const vt::VersionId &id)
{
  vt::VersionTree *tree = this->getFullTree();
  vt::VersionId child = tree->getFirstChild(id);
  if(child != vt::VERSION_INVALID)
    if(tree->getNextSibling(child) != vt::VERSION_INVALID)
      return MoreThanOneChild;
    else
      return ExactlyOneChild;
  return NoChildren;
}

bool VistrailController::inLatest(const vt::VersionId &id)
{
  return this->self->latest_set.find(id) != this->self->latest_set.end();
}

void VistrailController::addToLatest(const vt::VersionId &id)
{
  this->self->latest_queue.push(id);
  this->self->latest_set.insert(id);
  if(this->self->latest_queue.size() > NUM_LATEST)
  {
    this->self->latest_set.erase(this->self->latest_queue.front());
    this->self->latest_queue.pop();
  }
}

void VistrailController::addToFilter(const vt::VersionId &id)
{
  if(VisTrailsWindow::getInstance()->bFiltering)
    this->self->filter.insert(id);
}

void VistrailController::clearFilter()
{
  this->self->filter.clear();
}

bool VistrailController::inFilter(const vt::VersionId &id)
{
  if(VisTrailsWindow::getInstance()->bFiltering)
    return this->self->filter.find(id) != this->self->filter.end();
  return false;
}

void VistrailController::rebuildTerseTree()
{
  this->terseTree->clear();
  VersionSet visible;
  vt::VersionTree *fullTree = this->getFullTree();
  vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(fullTree);
  traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_DFS);
  visible.insert(vt::VERSION_ROOT);
  traversal.next(); //skip root
  while(!traversal.done())
  {
    vt::VersionId current = traversal.current();
    if(!this->mustHide(current) && this->mustShow(current))
    {
      visible.insert(current);
      vt::VersionId visibleAncestor = fullTree->getParent(current);
      while(visible.find(visibleAncestor) == visible.end())
        visibleAncestor = this->getFullTree()->getParent(visibleAncestor);
      this->terseTree->add(current, visibleAncestor);
    }
    traversal.next();
  }
}

vt::VersionTree *VistrailController::getTerseTree()
{
  return this->terseTree;
}

vt::VersionTree *VistrailController::getFullTree()
{
  return this->sessionFocus->session()->vistrail()->versionTree();
}

void VistrailController::filterAnnotation(const char* key, 
                                          const QString &filter)
{
  QByteArray data = filter.toUtf8();
  const char* cfilter = data.constData();

  vt::Vistrail *vistrail = this->sessionFocus->session()->vistrail();
  vt::VersionTree *fullTree = vistrail->versionTree();
  vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(fullTree);
  traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_FAST);
  while(!traversal.done())
  {
    vt::Version *current = vistrail->getVersion(traversal.current());
    if(current)
    {
      vt::Property *p = current->annotations()->get(key);
      if(p && strstr(p->asChar(), cfilter))
        this->self->filter.insert(current->id());
    }
    traversal.next();
  }
}

void VistrailController::filterStamp(const char* key, 
                                     const QString &filter)
{
  QByteArray data = filter.toUtf8();
  const char* cfilter = data.constData();

  vt::Vistrail *vistrail = this->sessionFocus->session()->vistrail();
  vt::VersionTree *fullTree = vistrail->versionTree();
  vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(fullTree);
  traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_FAST);
  while(!traversal.done())
  {
    vt::Version *current = vistrail->getVersion(traversal.current());
    if(current)
    {
      vt::Property *p = current->stamp()->get(key);
      if(p && strstr(p->asChar(), cfilter))
        this->self->filter.insert(current->id());
    }
    traversal.next();
  }
}

void VistrailController::filterDate(QDateTime from, 
                                    QDateTime to)
{
  from.setTimeSpec(Qt::LocalTime);
  to.setTimeSpec(Qt::LocalTime);
  time_t from_t = from.toUTC().toSecsSinceEpoch();
  time_t to_t = to.toUTC().toSecsSinceEpoch();

  vt::Vistrail *vistrail = this->sessionFocus->session()->vistrail();
  vt::VersionTree *fullTree = vistrail->versionTree();
  vt::VersionTreeTraversal traversal = vt::VersionTreeTraversal(fullTree);
  traversal.traverse(vt::VERSION_ROOT, vt::TRAVERSE_FAST);
  while(!traversal.done())
  {
    vt::Version *current = vistrail->getVersion(traversal.current());
    if(current)
    {
      vt::Property *p = current->stamp()->get("date");
      if(p)
      {
        time_t date = p->asDateTime()->toTime();
        if(date >= from_t && date <= to_t)
          this->self->filter.insert(current->id());
      }
    }
    traversal.next();
  }
}
