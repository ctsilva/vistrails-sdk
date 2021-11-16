/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */
// Description:

#ifndef VERSION_TREE_TRAVERSAL_H
#define VERSION_TREE_TRAVERSAL_H

#include "global.h"
#include "VersionTree.h"

VT_BEGIN_NAMESPACE

enum TraverseType
  {
    TRAVERSE_FAST=0,
    TRAVERSE_BFS=1,
    TRAVERSE_DFS=2,
    TRAVERSE_EDGE=3
  };

class VT_EXPORT VersionTreeTraversal
{
  VT_DECLARE_PIMPL(VersionTreeTraversal);
  friend class VersionTreeLayout;
public:
  VersionTreeTraversal(VersionTree *tree=NULL);

  void          setVersionTree(VersionTree *tree);
  VersionTree * versionTree();

  void              traverse(const VersionId &root, TraverseType tt);
  bool              done();
  void              next(bool traverseCurrent=true);
  const VersionId & current();

  const VersionId & fastParent();
  const VersionId & fastFirstChild();
  const VersionId & fastLastChild();
  const VersionId & fastNextSibling();
  const VersionId & fastPrevSibling();

  const VersionId & parent();
  const VersionId & firstChild();
  const VersionId & lastChild();
  const VersionId & nextSibling();
  const VersionId & prevSibling();
  
  const VersionId & edgeParent();
};

VT_END_NAMESPACE

#endif
