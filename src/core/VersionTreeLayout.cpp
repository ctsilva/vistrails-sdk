/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "VersionTreeLayout.h"
#include "global_private.h"
#include "VersionTreeTraversal.h"
#include <boost/unordered_map.hpp>

VT_BEGIN_NAMESPACE

LayoutProperties::~LayoutProperties()
{
}

struct VersionLayoutNode
{
  VersionLayoutNode():
      thread(VERSION_INVALID), ancestor(VERSION_INVALID),
      level(0), number(0), mod(0), prelim(0), shift(0), change(0) {}

  VersionId thread;
  VersionId ancestor;
  int level;
  int number;
  float mod;
  float prelim;
  float shift;
  float change;
};
#define USE_VECTOR 0
#if USE_VECTOR
typedef std::vector<VersionLayoutNode> IdLayoutNodeMap;
#else
typedef boost::unordered_map<VersionId, VersionLayoutNode> IdLayoutNodeMap;
#endif

class VersionTreeLayout::PrivateImpl
{
public:
  void firstWalk(const VersionId &);
  void apportion(const VersionId &, VersionId &);
  void executeShifts(const VersionId &);
  void moveSubtree(const VersionId &, const VersionId &, float);
  const VersionId & ancestor(const VersionId &, const VersionId &, const VersionId &);
  const VersionId & nextRight(const VersionId &);
  const VersionId & nextLeft(const VersionId &);
  void secondWalk(const VersionId &, float);
  
  VersionTree *tree;
  LayoutProperties *props;
  
  IdLayoutNodeMap nodeMap;
};

VT_DEFINE_PIMPL(VersionTreeLayout);

VersionTreeLayout::VersionTreeLayout(): self(new PrivateImpl())
{
}

void VersionTreeLayout::layout(VersionTree *tree, LayoutProperties *props)
{
  VersionTreeTraversal traversal(tree);
  traversal.traverse(VERSION_ROOT, TRAVERSE_EDGE);
#if USE_VECTOR
  this->self->nodeMap.resize(tree->getMaxVersionId()+1);
#else
  this->self->nodeMap.clear();
#endif
  while (!traversal.done()) {
    VersionId v = traversal.current();
    VersionId old_v = v;
    VersionLayoutNode *node = &(this->self->nodeMap[v]);
    node->ancestor = v;
    v = traversal.parent();
    if (v!=VERSION_INVALID) {
      node->level = this->self->nodeMap[v].level+1;
    }
    v = traversal.prevSibling();
    if (v!=VERSION_INVALID) {
      node->number = this->self->nodeMap[v].number+1;
    }
    traversal.next();
  }
  this->self->tree = tree;
  this->self->props = props;
  this->self->firstWalk(VERSION_ROOT);
  this->self->secondWalk(VERSION_ROOT, -this->self->nodeMap[VERSION_ROOT].prelim);
}

void VersionTreeLayout::PrivateImpl::firstWalk(const VersionId &v)
{
  VersionId w, defaultAncestor;
  VersionLayoutNode *wNode, *vNode;
  float midpoint;
  
  defaultAncestor = this->tree->getFirstChild(v);
  vNode = &(this->nodeMap[v]);
  if (defaultAncestor==VERSION_INVALID) {
    midpoint = 0;
  }
  else{
    wNode = &(this->nodeMap[defaultAncestor]);
    for (w=defaultAncestor; w!=VERSION_INVALID; w=this->tree->getNextSibling(w)) {
      this->firstWalk(w);
      this->apportion(w, defaultAncestor);
    }
    this->executeShifts(w);
    midpoint = wNode->prelim;
    wNode = &(this->nodeMap[this->tree->getLastChild(v)]);
    midpoint = (midpoint + wNode->prelim)/2.0;
  }
  
  w = this->tree->getPrevSibling(v);
  if (w!=VERSION_INVALID) {
    wNode = &(this->nodeMap[w]);
    vNode->prelim = wNode->prelim + this->props->siblingDistance(v, w);
    vNode->mod = vNode->prelim - midpoint;
  }
  else {
    vNode->prelim = midpoint;
  }
}

void VersionTreeLayout::PrivateImpl::apportion(const VersionId &v, VersionId &defaultAncestor)
{
  VersionId vip, vop, vim, vom, w, nr, nl;
  float sip, sop, sim, som, shift;
  w = this->tree->getPrevSibling(v);
  if (w!=VERSION_INVALID) {
    vip = vop = v;
    vim = w;
    for (; w!=VERSION_INVALID; vom=w, w=this->tree->getPrevSibling(w));
    sip = this->nodeMap[vip].mod;
    sop = this->nodeMap[vop].mod;
    sim = this->nodeMap[vim].mod;
    som = this->nodeMap[vom].mod;
    while ((nr=this->nextRight(vim))!=VERSION_INVALID &&
           (nl=this->nextLeft(vip))!=VERSION_INVALID) {
      vim = nr;
      vip = nl;
      vom = this->nextLeft(vom);
      vop = this->nextRight(vop);
      this->nodeMap[vop].ancestor = v;
      shift = ((this->nodeMap[vim].prelim+sim) -
               (this->nodeMap[vip].prelim+sip) + 
               this->props->siblingDistance(vim, vip));
      if (shift>0.0) {
        this->moveSubtree(this->ancestor(vim, v, defaultAncestor), v, shift);
        sip += shift;
        sop += shift;
      }
      sim += this->nodeMap[vim].mod;
      sip += this->nodeMap[vip].mod;
      som += this->nodeMap[vom].mod;
      sop += this->nodeMap[vop].mod;
    }
    if ((nr=this->nextRight(vim))!=VERSION_INVALID && this->nextRight(vop)==VERSION_INVALID) {
      VersionLayoutNode *node = &(this->nodeMap[vop]);
      node->thread = nr;
      node->mod += sim-sop;
    }
    if ((nl=this->nextLeft(vip))!=VERSION_INVALID && this->nextLeft(vom)==VERSION_INVALID) {
      VersionLayoutNode *node = &(this->nodeMap[vom]);
      node->thread = nl;
      node->mod += sip-som;
      defaultAncestor = v;
    }
  }
}

const VersionId &VersionTreeLayout::PrivateImpl::nextLeft(const VersionId &v)
{
  const VersionId * leftMostChild = &this->tree->getFirstChild(v);
  if (*leftMostChild!=VERSION_INVALID)
    return *leftMostChild;
  else
    return this->nodeMap[v].thread;
}

const VersionId &VersionTreeLayout::PrivateImpl::nextRight(const VersionId &v)
{
  const VersionId * rightMostChild = &this->tree->getLastChild(v);
  if (*rightMostChild!=VERSION_INVALID)
    return *rightMostChild;
  else
    return this->nodeMap[v].thread;
}

void VersionTreeLayout::PrivateImpl::moveSubtree(const VersionId &wm, const VersionId &wp, float shift)
{
  VersionLayoutNode *wmNode = &(this->nodeMap[wm]);
  VersionLayoutNode *wpNode = &(this->nodeMap[wp]);
  int substrees = wpNode->number-wmNode->number;
  wpNode->change -= shift/substrees;
  wpNode->shift += shift;
  wmNode->change += shift/substrees;
  wpNode->prelim += shift;
  wpNode->mod += shift;
}

void VersionTreeLayout::PrivateImpl::executeShifts(const VersionId &v)
{
  float shift = 0;
  float change = 0;
  for (VersionId w=this->tree->getLastChild(v);
       w!=VERSION_INVALID;
       w=this->tree->getPrevSibling(w)) {
    VersionLayoutNode *node = &(this->nodeMap[w]);
    node->prelim += shift;
    node->mod += shift;
    change += node->change;
    shift += node->shift + change;
  }
}

const VersionId &VersionTreeLayout::PrivateImpl::ancestor(const VersionId &vim, const VersionId &v, const VersionId &defaultAncestor)
{
  VersionId *vimAncestor = &this->nodeMap[vim].ancestor;
  VersionId vP = this->tree->getParent(v);
  if (vP!=VERSION_INVALID && this->tree->getParent(*vimAncestor)==vP)
    return *vimAncestor;
  else
    return defaultAncestor;
}

void VersionTreeLayout::PrivateImpl::secondWalk(const VersionId &v, float m)
{
  VersionLayoutNode *node = &(this->nodeMap[v]);
  this->props->setVersionPos(v, node->prelim+m, node->level);

  for (VersionId w=this->tree->getFirstChild(v);
       w!=VERSION_INVALID;
       w=this->tree->getNextSibling(w)) {
    this->secondWalk(w, m+node->mod);
  }
}

VT_END_NAMESPACE
