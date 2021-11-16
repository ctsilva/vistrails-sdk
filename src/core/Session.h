/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef SESSION_H
#define SESSION_H

#include "global.h"
#include "Handler.h"

VT_BEGIN_NAMESPACE

class ActionList;
class Storage;
class Vistrail;

class VT_EXPORT Session: public HStamp
{
  VT_DECLARE_PIMPL(Session);
public:
  Session();
  PDict *           info();
  void              record(Version *version);
  Vistrail *        vistrail() const;
  void              setVistrail(Vistrail *vistrail);

  void              save(Storage *);
  void              load(Storage *);

  void              setStampHandler(HStamp *stamper);
  void              stamp(Session *, Version *) const;

  void              bind(Storage *storage);
  void              update(HMerge *handler=NULL);
  void              commit(HMerge *handler=NULL);
  void              discard();
  const VersionId & getUpdatedId(const VersionId &id);
};

VT_END_NAMESPACE

#endif
