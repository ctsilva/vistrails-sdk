/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "VersionTree.h"
#include "global_private.h"
#include "VersionTreeTraversal.h"
//#include <boost/thread/thread.hpp>
//#include <boost/thread/mutex.hpp>
#include <boost/unordered_map.hpp>
#include <vector>
#include <deque>
#include <iostream>

VT_BEGIN_NAMESPACE

struct VersionIdNode
{
  VersionIdNode():
      parent(VERSION_INVALID),
      firstChild(VERSION_INVALID),
      lastChild(VERSION_INVALID),
      nextSibling(VERSION_INVALID),
      prevSibling(VERSION_INVALID) {}

  VersionId parent;
  VersionId firstChild;
  VersionId lastChild;
  VersionId nextSibling;
  VersionId prevSibling;
};
typedef boost::unordered_map<VersionId, VersionIdNode> IdNodeMap;

class VersionTree::PrivateImpl
{
public:
  void clear() {
    this->nodeMap.clear();
    this->nodeMap[VERSION_ROOT] = VersionIdNode();
  }

  void pruneRecursively(const VersionId &id);
  
  IdNodeMap nodeMap;
};

VT_DEFINE_PIMPL(VersionTree);

VersionTree::VersionTree(): self(new PrivateImpl())
{
  this->self->clear();
}

void VersionTree::clear()
{
  this->self->clear();
}

size_t VersionTree::getNumberOfVersions()
{
  return this->self->nodeMap.size();
}

void VersionTree::add(const VersionId &id, const VersionId &parentId)
{
  if (parentId!=VERSION_INVALID) {
    VersionIdNode *parent = &(this->self->nodeMap[parentId]);
    VersionIdNode *node = &(this->self->nodeMap[id]);
    VersionId lastChild = parent->lastChild;
    parent->lastChild = id;
    node->parent = parentId;
    if (lastChild!=VERSION_INVALID) {
      this->self->nodeMap[lastChild].nextSibling = id;
      node->prevSibling = lastChild;
    }
    else {
      parent->firstChild = id;
    }
  }
}

void VersionTree::prune(const VersionId &id)
{
  if(id != VERSION_INVALID)
  {
    VersionIdNode* node = &(this->self->nodeMap[id]);

    //update parent node
    if(node->parent != VERSION_INVALID)
    {
      VersionIdNode *parent = &(this->self->nodeMap[node->parent]);
      if(parent->firstChild == id)
        parent->firstChild = node->nextSibling;
      if(parent->lastChild == id)
        parent->lastChild = node->prevSibling;
    }

    //update siblings
    if(node->nextSibling != VERSION_INVALID)
    {
      VersionIdNode *next = &(this->self->nodeMap[node->nextSibling]);
      next->prevSibling = node->prevSibling;
    }

    if(node->prevSibling != VERSION_INVALID)
    {
      VersionIdNode *prev = &(this->self->nodeMap[node->prevSibling]);
      prev->nextSibling = node->nextSibling;
    }

    // remove all nodes in subtree rooted at id
    this->self->pruneRecursively(id);
  }
}

void VersionTree::PrivateImpl::pruneRecursively(const VersionId &id)
{
  VersionIdNode* node = &(this->nodeMap[id]);
  VersionId curChild = node->firstChild;
  VersionId nextChild;

  while(curChild != VERSION_INVALID)
  {
    node = &(this->nodeMap[curChild]);
    nextChild = node->nextSibling;
    this->pruneRecursively(curChild);
    curChild = nextChild;
  }

  this->nodeMap.erase(id);
}

#define DEFINE_VERSIONIDTREE_LOOKUP(FUNCTION_NAME, ATTR_NAME)       \
  const VersionId & VersionTree::FUNCTION_NAME(const VersionId &id) \
  {                                                                 \
    IdNodeMap::const_iterator ii = this->self->nodeMap.find(id);    \
    if (ii!=this->self->nodeMap.end()) {                            \
      return (*ii).second.ATTR_NAME;                                \
    }                                                               \
    return VERSION_INVALID;                                         \
  }

DEFINE_VERSIONIDTREE_LOOKUP(getParent, parent);
DEFINE_VERSIONIDTREE_LOOKUP(getFirstChild, firstChild);
DEFINE_VERSIONIDTREE_LOOKUP(getLastChild, lastChild);
DEFINE_VERSIONIDTREE_LOOKUP(getNextSibling, nextSibling);
DEFINE_VERSIONIDTREE_LOOKUP(getPrevSibling, prevSibling);



typedef std::deque<VersionId> VersionDeque;
typedef std::pair<VersionId,VersionId> VersionEdge;
typedef std::deque<VersionEdge> VersionEdgeDeque;
class VersionTreeTraversal::PrivateImpl
{
public:
  VersionTree *    tree;
  TraverseType     currentType;
  VersionId        currentVersion;
  VersionDeque     buffer;
  
  VersionId        edgeParent;
  VersionEdgeDeque edgeBuffer;
  
  IdNodeMap::const_iterator iCur, iEnd;
};

VT_DEFINE_PIMPL(VersionTreeTraversal);

VersionTreeTraversal::VersionTreeTraversal(VersionTree *tree): self(new PrivateImpl())
{
  this->setVersionTree(tree);
}

void VersionTreeTraversal::setVersionTree(VersionTree *vt)
{
  this->self->tree = vt;
}

VersionTree * VersionTreeTraversal::versionTree()
{
  return this->self->tree;
}

void VersionTreeTraversal::traverse(const VersionId &root, TraverseType tt)
{
  this->self->currentType = tt;
  this->self->currentVersion = root;
  if (this->self->currentType==TRAVERSE_FAST) {
    this->self->iCur = this->self->tree->self->nodeMap.begin();
    this->self->iEnd = this->self->tree->self->nodeMap.end();
    this->self->currentVersion = (*this->self->iCur).first;
  }
  else if (this->self->currentType==TRAVERSE_EDGE) {
    this->self->edgeParent = VERSION_INVALID;
    this->self->edgeBuffer.clear();
    this->self->edgeBuffer.push_back(VersionEdge(VERSION_INVALID, VERSION_ROOT));
    this->next();
  }
  else {
    this->self->buffer.clear();
    this->self->buffer.push_back(root);
  }
}

bool VersionTreeTraversal::done()
{
  return this->self->currentVersion==VERSION_INVALID;
}

const VersionId & VersionTreeTraversal::current()
{
  return this->self->currentVersion;
}

void VersionTreeTraversal::next(bool traverseCurrent)
{
  VersionId child;
  if (this->self->currentVersion!=VERSION_INVALID) {
    switch (this->self->currentType)
      {
      case TRAVERSE_FAST:
        this->self->iCur++;
        if (this->self->iCur!=this->self->iEnd) {
          this->self->currentVersion = (*this->self->iCur).first;
        }
        else {
          this->self->currentVersion = VERSION_INVALID;
        }
        break;

      case TRAVERSE_BFS:
        this->self->buffer.pop_front();
        if (traverseCurrent) {
          child = this->self->tree->getFirstChild(this->self->currentVersion);
          while (child!=VERSION_INVALID) {
            this->self->buffer.push_back(child);
            child = this->self->tree->getNextSibling(child);
          }
        }
        this->self->currentVersion = this->self->buffer.size()>0?this->self->buffer.front():VERSION_INVALID;
        break;

      case TRAVERSE_DFS:
        this->self->buffer.pop_front();
        if (traverseCurrent) {
          child = this->self->tree->getLastChild(this->self->currentVersion);
          while (child!=VERSION_INVALID) {
            this->self->buffer.push_front(child);
            child = this->self->tree->getPrevSibling(child);
          }
        }
        this->self->currentVersion = this->self->buffer.size()>0?this->self->buffer.front():VERSION_INVALID;
        break;

      case TRAVERSE_EDGE:
        this->self->edgeBuffer.pop_front();
        if (traverseCurrent) {
          child = this->self->tree->getFirstChild(this->self->currentVersion);
          while (child!=VERSION_INVALID) {
            this->self->edgeBuffer.push_back(VersionEdge(this->self->currentVersion, child));
            child = this->self->tree->getNextSibling(child);
          }
        }
        if (this->self->edgeBuffer.size()>0) {
          this->self->edgeParent = this->self->edgeBuffer.front().first;
          this->self->currentVersion = this->self->edgeBuffer.front().second;
        }
        else {
          this->self->edgeParent = this->self->currentVersion = VERSION_INVALID;
        }
        break;
        
      default:
        break;
      }
  }
}

#define DEFINE_VERSIONIDTREETRAVERSAL_FAST_LOOKUP(FUNCTION_NAME, ATTR_NAME) \
  const VersionId & VersionTreeTraversal::FUNCTION_NAME()                   \
  {                                                                         \
    return (*this->self->iCur).second.ATTR_NAME;                            \
  }

DEFINE_VERSIONIDTREETRAVERSAL_FAST_LOOKUP(fastParent, parent)
DEFINE_VERSIONIDTREETRAVERSAL_FAST_LOOKUP(fastFirstChild, firstChild)
DEFINE_VERSIONIDTREETRAVERSAL_FAST_LOOKUP(fastLastChild, lastChild)
DEFINE_VERSIONIDTREETRAVERSAL_FAST_LOOKUP(fastNextSibling, nextSibling)
DEFINE_VERSIONIDTREETRAVERSAL_FAST_LOOKUP(fastPrevSibling, prevSibling)

#define DEFINE_VERSIONIDTREETRAVERSAL_LOOKUP(FUNCTION_NAME, ATTR_NAME)  \
  const VersionId & VersionTreeTraversal::FUNCTION_NAME()               \
  {                                                                     \
    return this->self->tree->ATTR_NAME(this->self->currentVersion);     \
  }

DEFINE_VERSIONIDTREETRAVERSAL_LOOKUP(parent, getParent)
DEFINE_VERSIONIDTREETRAVERSAL_LOOKUP(firstChild, getFirstChild)
DEFINE_VERSIONIDTREETRAVERSAL_LOOKUP(lastChild, getLastChild)
DEFINE_VERSIONIDTREETRAVERSAL_LOOKUP(nextSibling, getNextSibling)
DEFINE_VERSIONIDTREETRAVERSAL_LOOKUP(prevSibling, getPrevSibling)


const VersionId & VersionTreeTraversal::edgeParent()
{
  return this->self->edgeParent;
}

VT_END_NAMESPACE
