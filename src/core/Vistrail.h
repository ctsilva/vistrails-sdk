/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VISTRAIL_H
#define VISTRAIL_H

#include "global.h"
#include "Version.h"

VT_BEGIN_NAMESPACE

class ActionList;
class ActionPath;
class Property;
class VersionTree;

class VT_EXPORT Vistrail
{
  VT_DECLARE_PIMPL(Vistrail);
public:
  Vistrail();
  
  VersionTree * versionTree();
  void          addVersion(Version *version);
  Version *     getVersion(const VersionId & id) const;
  void          getAllVersions(VersionList *list) const;
  PDict *       annotations();
  
  void          getVersionPath(const VersionId &src,
                               const VersionId &dst,
                               VersionPath *path);
  void          prune(const VersionId &id);
  // void          getActionPath(const VersionList &revVersionIds, 
  //                             const VersionList &versionIds, 
  //                             VersionId checkpointId,
  //                             VersionList *path);
  // void          getMaterializationPath(VersionId id, VersionId currentId, VersionList *path);
};

VT_END_NAMESPACE

#endif
