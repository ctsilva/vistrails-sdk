/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "VersionTreeFilter.h"
#include "global_private.h"
#include "VersionTreeTraversal.h"
#include <boost/unordered_set.hpp>
#include <boost/unordered_map.hpp>

VT_BEGIN_NAMESPACE

typedef boost::unordered_set<VersionId> IdSet;
typedef boost::unordered_map<VersionId, int> IdIntMap;

class VersionTreeFilter::PrivateImpl
{
public:
  VersionTree *        inTree;
  VersionTree          outTree;
  VersionTreeTraversal traversal;
};

VT_DEFINE_PIMPL(VersionTreeFilter);

VersionTreeFilter::VersionTreeFilter(VersionTree *vt): self(new PrivateImpl())
{
  this->setVersionTree(vt);
}

void VersionTreeFilter::setVersionTree(VersionTree *vt)
{
  this->self->inTree = vt;
  this->self->traversal.setVersionTree(this->self->inTree);
}

VersionTree* VersionTreeFilter::getVersionTree()
{
  return this->self->inTree;
}

bool VersionTreeFilter::mustShow(const VersionId &id)
{
  return true;
}

bool VersionTreeFilter::mustHide(const VersionId &id)
{
  return false;
}

void VersionTreeFilter::rebuild()
{
  IdSet    visibleVersions;
  IdIntMap degrees;
  
  visibleVersions.clear();
  this->self->traversal.traverse(VERSION_ROOT, TRAVERSE_BFS);
  while (!this->self->traversal.done()) {
    VersionId version = this->self->traversal.current();
    degrees[version] = 0;
    if (this->mustHide(version))
      this->self->traversal.next(false);

    else {
      if (this->mustShow(version)) {
        visibleVersions.insert(version);
        VersionId parent = version;
        while ((parent=this->self->inTree->getParent(parent))!=VERSION_INVALID) {
          if (visibleVersions.find(parent)!=visibleVersions.end()) {
            break;
          }
          if ((degrees[parent]++)>1) {
            visibleVersions.insert(parent);
            break;
          }
        }
      }
      this->self->traversal.next(true);
    }
  }

  this->self->outTree.clear();
  IdSet::const_iterator is = visibleVersions.begin();
  for (; is!=visibleVersions.end(); is++) {
    VersionId version = *is;
    VersionId parent = version;
    while ((parent=this->self->inTree->getParent(parent))!=VERSION_INVALID) {
      if (visibleVersions.find(parent)!=visibleVersions.end()) {
        this->self->outTree.add(version, parent);
        break;
      }
    }
  }
}

VersionTree* VersionTreeFilter::getFilteredTree()
{
  return &(this->self->outTree);
}

VT_END_NAMESPACE
