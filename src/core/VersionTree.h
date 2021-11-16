/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VERSION_TREE_H
#define VERSION_TREE_H

#include "global.h"
#include "Version.h"

VT_BEGIN_NAMESPACE

class ActionList;
class PDict;

class VT_EXPORT VersionTree
{
  VT_DECLARE_PIMPL(VersionTree);
  friend class VersionTreeTraversal;  
public:
  VersionTree();

  void add(const VersionId &id, const VersionId &parent);
  void prune(const VersionId &id);

  const VersionId & getParent(const VersionId &id);
  const VersionId & getFirstChild(const VersionId &id);
  const VersionId & getLastChild(const VersionId &id);
  const VersionId & getNextSibling(const VersionId &id);
  const VersionId & getPrevSibling(const VersionId &id);

  size_t getNumberOfVersions();

  void clear();
};

VT_END_NAMESPACE

#endif
