/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "SessionFocus.h"
#include "global_private.h"
#include "Action.h"
#include "Session.h"
#include "VersionTree.h"
#include "VersionTreeTraversal.h"
#include "Vistrail.h"
#include "boost/unordered_map.hpp"
#include <vector>
#include <cstring>

VT_BEGIN_NAMESPACE

typedef boost::unordered_map<VersionId,VersionId> IdMap;
class SessionFocus::PrivateImpl
{
public:
  VersionId             currentVersion;
  bool                  autoSelectVersion;
  Session *             session;
  IdMap                 redoHistory;
  std::vector<HNotify*> handlers;

  void versionSelected(const VersionId &prev, const VersionId &cur)
  {
    std::vector<HNotify*>::iterator it = this->handlers.begin();
    for(;it != this->handlers.end(); it++)
      (*it)->versionSelected(prev, cur);
  }
};

VT_DEFINE_PIMPL(SessionFocus);

SessionFocus::SessionFocus(Session *s): self(new PrivateImpl())
{
  self->session = s;
  self->currentVersion = VERSION_ROOT;
  self->autoSelectVersion = true;
}

Session *SessionFocus::session()
{
  return self->session;
}

const VersionId & SessionFocus::currentVersion() const
{
  return self->currentVersion;
}

bool SessionFocus::autoSelectVersion() const
{
  return self->autoSelectVersion;
}

void SessionFocus::setAutoSelectVersion(bool value)
{
  self->autoSelectVersion = value;
}

void SessionFocus::reset()
{
  self->versionSelected(self->currentVersion, VERSION_ROOT);
  self->currentVersion = VERSION_ROOT;
  self->redoHistory.clear();
}

const VersionId & SessionFocus::selectVersion(const VersionId & v)
{
  //update redo history
  VersionTree *tree = self->session->vistrail()->versionTree();
  VersionId parent = tree->getParent(v);
  VersionId current = v;
  while(parent != VERSION_INVALID)
  {
    if(self->redoHistory.find(parent) != self->redoHistory.end())
    {
      if(self->redoHistory[parent] == current)
        break;
      else
      {
        // remove old redo history branch
        VersionId tempParent = self->redoHistory[parent], tempChild;
        while(self->redoHistory.find(tempParent) != self->redoHistory.end())
        {
          tempChild = self->redoHistory[tempParent];
          self->redoHistory.erase(tempParent);
          tempParent = tempChild;
        }
        self->redoHistory[parent] = current;
        break;
      }
    }
    self->redoHistory[parent] = current;
    current = parent;
    parent = tree->getParent(current);
  }

  if(self->redoHistory.find(v) == self->redoHistory.end())
    self->redoHistory[v] = VERSION_INVALID;

  self->versionSelected(self->currentVersion, v);
  self->currentVersion = v;
  return self->currentVersion;
}


const VersionId & SessionFocus::selectVersionByTag(const char *tagName)
{
  //this is very slow
  Vistrail * vistrail = self->session->vistrail();
  VersionTreeTraversal traversal = VersionTreeTraversal(vistrail->versionTree());
  traversal.traverse(VERSION_ROOT, TRAVERSE_FAST);
  while(!traversal.done())
  {
    Property *p = 
      vistrail->getVersion(traversal.current())->annotations()->get("tag");
    if(p && strcmp(p->asChar(), tagName) == 0)
      return this->selectVersion(traversal.current());
    traversal.next();
  }
  return VERSION_INVALID;
}

const VersionId & SessionFocus::getParent()
{
  VersionTree *tree = self->session->vistrail()->versionTree();
  return tree->getParent(self->currentVersion);
}

const VersionId & SessionFocus::selectParent()
{
  const VersionId & parent = this->getParent();
  if(parent != VERSION_INVALID)
  {
    self->versionSelected(self->currentVersion, parent);
    self->currentVersion = parent;
  }
  return parent;
}

const VersionId & SessionFocus::selectLastChild()
{
  const VersionId & lastChild = this->getLastChild();
  if(lastChild != VERSION_INVALID)
    this->selectVersion(lastChild);
  return lastChild;
}

const VersionId & SessionFocus::selectRedoChild()
{
  const VersionId & redoChild = this->getRedoChild();
  if(redoChild != VERSION_INVALID)
    this->selectVersion(redoChild);
  return redoChild;
}

const VersionId & SessionFocus::getLastChild()
{
  VersionTree *tree = self->session->vistrail()->versionTree();
  return tree->getLastChild(self->currentVersion);
}

const VersionId & SessionFocus::getRedoChild()
{
  IdMap::const_iterator it = self->redoHistory.find(self->currentVersion);
  if (it!=self->redoHistory.end())
    return (*it).second;
  return this->getLastChild();
}

Version * SessionFocus::record(Action *action)
{
  return this->record(ActionList(action));
}

Version * SessionFocus::record(const ActionList &actions)
{
  Version *version = new Version();
  version->performed()->add(actions);
  return this->record(version);
}

Version * SessionFocus::record(Version *version)
{
  version->setParent(self->currentVersion);
  self->session->record(version);

  if(self->redoHistory.find(self->currentVersion) 
    != self->redoHistory.end())
  {
    // remove old redo history branch
    VersionId tempParent = self->redoHistory[self->currentVersion];
    VersionId tempChild;
    while(self->redoHistory.find(tempParent) != self->redoHistory.end())
    {
      tempChild = self->redoHistory[tempParent];
      self->redoHistory.erase(tempParent);
      tempParent = tempChild;
    }
  }
  self->redoHistory[self->currentVersion] = version->id();
  self->redoHistory[version->id()] = VERSION_INVALID;

  if (self->autoSelectVersion)
    self->currentVersion = version->id();

  std::vector<HNotify*>::iterator it = self->handlers.begin();
  for(;it != self->handlers.end(); it++)
    (*it)->versionAdded(version);

  return version;
}

void SessionFocus::prune(const VersionId &id)
{
  this->session()->vistrail()->prune(id);
  this->vistrailChanged();
}

bool SessionFocus::inRedoHistory(const VersionId &id)
{
  return self->redoHistory.find(id) != self->redoHistory.end();
}

void SessionFocus::addHandler(HNotify *notify)
{
  self->handlers.push_back(notify);
}

void SessionFocus::removeHandler(HNotify *notify)
{
  std::vector<HNotify*>::iterator it = self->handlers.begin();
  while(it != self->handlers.end())
  {
    if((*it) == notify)
      it = self->handlers.erase(it);
    else
      it++;
  }
}

void SessionFocus::versionChanged(const VersionId &id)
{
  std::vector<HNotify*>::iterator it = self->handlers.begin();
  for(;it != self->handlers.end();++it)
    (*it)->versionChanged(id);
}

void SessionFocus::vistrailChanged()
{
  std::vector<HNotify*>::iterator it = self->handlers.begin();
  for(;it != self->handlers.end();++it)
    (*it)->vistrailChanged();
}

// void SessionFocus::getVersionIdPathTo(VersionId dst, 
//                                       std::vector<VersionId> & versionIds)
// {
//   Vistrail* vistrail = self->session->vistrail();
//   vistrail->getVersionIdPath(self->currentVersion, dst, versionIds);
// }

// void SessionFocus::getPathTo(VersionId dst, ActionPath * path)
// {
//   Vistrail* vistrail = self->session->vistrail();
//   vistrail->getPath(self->currentVersion, dst, path);
// }

VT_END_NAMESPACE
