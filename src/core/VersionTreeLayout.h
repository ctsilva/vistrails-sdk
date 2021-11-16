/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */
// Description:

#ifndef VERSION_TREE_LAYOUT_H
#define VERSION_TREE_LAYOUT_H

#include "global.h"
#include "VersionTree.h"

VT_BEGIN_NAMESPACE

class VT_EXPORT LayoutProperties
{
public:
  virtual ~LayoutProperties();
  virtual void setVersionPos(const VersionId &id, float X, float Y)=0;
  virtual float siblingDistance(const VersionId &left, const VersionId &right)=0;
};

class VT_EXPORT VersionTreeLayout
{
  VT_DECLARE_PIMPL(VersionTreeLayout);
public:
  VersionTreeLayout();
  void layout(VersionTree *tree, LayoutProperties *props);
};

VT_END_NAMESPACE

#endif
