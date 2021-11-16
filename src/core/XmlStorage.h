/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef XML_STORAGE_H
#define XML_STORAGE_H

#include "global.h"
#include "Property.h"
#include "Storage.h"

VT_BEGIN_NAMESPACE

class VT_EXPORT XmlStorage : public Storage
{
  VT_DECLARE_PIMPL(XmlStorage);

public:
  XmlStorage(const char * filename);

  // Load
  virtual void   loadBegin();
  virtual void   loadVistrailAnnotations(PDict *annotations);
  virtual size_t loadNumberOfVersions();
  virtual void   loadVersion(VersionId *id, VersionId *parent,
                             PDict *stamp, PDict *annotations, PDict *actions);
  virtual void   loadEnd();

  // Save
  virtual void saveBegin(PDict *sessionInfo);
  virtual void saveVistrailAnnotations(PDict *annotations);
  virtual void saveNumberOfVersions(size_t count);
  virtual void saveVersion(const VersionId &id, const VersionId &parent,
                           PDict *stamp, PDict *annotations, PDict *actions);
  virtual void saveEnd();

  // Fetch
  virtual void fetch(void *dataId, void *output) const;
};

VT_END_NAMESPACE

#endif
