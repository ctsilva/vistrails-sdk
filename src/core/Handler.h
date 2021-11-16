/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef HANDLER_H
#define HANDLER_H

#include "global.h"
#include "Version.h"

VT_BEGIN_NAMESPACE

class PDict;
class PList;
class Session;

class HMerge
{
public:
  virtual ~HMerge() {}
  enum MergeType
    {
      MT_BRANCH = 0,
      MT_MINE_AFTER_THEIRS = 1,
      MT_THEIRS_AFTER_MINE = 2
    };
  virtual MergeType mergeNode(Version*, Version*) { return MT_BRANCH; }
};

class HPropertyIO
{
public:
  virtual ~HPropertyIO() {}
  virtual const char *classname() const = 0;
  virtual void load(PList *plist) = 0;
  virtual void save(PList *plist) const = 0;
};

class HNotify
{
public:
  virtual ~HNotify() {}
  virtual void vistrailChanged() {}
  virtual void versionChanged(const VersionId &id) {}
  virtual void versionAdded(Version *version) {}
  virtual void versionSelected(const VersionId &previous,
                               const VersionId &current) {}
};

class HStamp
{
public:
  virtual ~HStamp() {}
  virtual void stamp(Session *, Version *) const =0;
};

class HFetchData
{
public:
  virtual ~HFetchData() {}
  virtual void fetch(void *dataId, void *output) const = 0;
};

class HSaveVistrail
{
public:
  virtual ~HSaveVistrail() {}
  virtual void saveBegin(PDict *sessionInfo) = 0;
  virtual void saveVistrailAnnotations(PDict *annotations) = 0;
  virtual void saveNumberOfVersions(size_t count) = 0;
  virtual void saveVersion(const VersionId &id, const VersionId &parent,
                           PDict *stamp, PDict *annotations, PDict *actions) = 0;
  virtual void saveEnd() = 0;
};

class HLoadVistrail
{
public:
  virtual ~HLoadVistrail() {}
  virtual void   loadBegin() = 0;
  virtual void   loadVistrailAnnotations(PDict *annotations) = 0;
  virtual size_t loadNumberOfVersions() = 0;
  virtual void   loadVersion(VersionId *id, VersionId *parent,
                             PDict *stamp, PDict *annotations, PDict *actions) = 0;
  virtual void   loadEnd() = 0;
};


VT_END_NAMESPACE

#endif
