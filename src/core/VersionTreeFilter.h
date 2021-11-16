/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VERSION_TREE_FILTER_H
#define VERSION_TREE_FILTER_H

#include "global.h"
#include "VersionTree.h"

VT_BEGIN_NAMESPACE

class VT_EXPORT VersionTreeFilter
{
  VT_DECLARE_PIMPL(VersionTreeFilter);
public:
  VersionTreeFilter(VersionTree *vt);
  
  void         setVersionTree(VersionTree *tree);
  VersionTree* getVersionTree();

  virtual bool mustShow(const VersionId &id);
  virtual bool mustHide(const VersionId &id);
  
  virtual void rebuild();
  VersionTree *getFilteredTree();
};

VT_END_NAMESPACE

#endif
