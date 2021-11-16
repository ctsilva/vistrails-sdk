/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Session.h"
#include "global_private.h"
#include "Action.h"
#include "ActionFactory.h"
#include "Handler.h"
#include "Property.h"
#include "Storage.h"
#include "Vistrail.h"
#include <vector>
#include <boost/unordered_map.hpp>
#include <time.h>

VT_BEGIN_NAMESPACE

typedef boost::unordered_map<VersionId,VersionId> IdMap;
typedef std::vector<Version*> VersionVector;
class Session::PrivateImpl
{
public:
  PrivateImpl(): vistrail(new Vistrail()), storage(NULL), stamper(NULL) {}
  ~PrivateImpl() { if (this->vistrail) delete this->vistrail; }

  Vistrail *     vistrail;
  PDict          info;
  HStamp *       stamper;
  
  Storage *      storage;
  VersionVector  uncommitted;
  IdMap          idMap;
};

VT_DEFINE_PIMPL(Session);

Session::Session(): self(new PrivateImpl())
{
  this->self->stamper = this;
}

PDict * Session::info()
{
  return &this->self->info;
}

void Session::setStampHandler(HStamp *stamper)
{
  this->self->stamper = stamper;
}

void Session::stamp(Session *session, Version *version) const
{
  version->stamp()->merge(session->info());
  version->stamp()->setDateTime("date", time(NULL));
}

void Session::save(Storage *storage)
{
  storage->save(this->self->vistrail, this->info());
}

void Session::load(Storage *storage)
{
  storage->load(this->self->vistrail);
}

void Session::bind(Storage *storage)
{
  this->self->storage = storage;
}

void Session::record(Version *version)
{
  if (this->self->stamper)
    this->self->stamper->stamp(this, version);
  this->self->vistrail->addVersion(version);
  if (version->id()!=VERSION_INVALID)
    this->self->uncommitted.push_back(version);
}

void Session::update(HMerge *handler)
{
  if (!this->self->storage || !this->self->storage->update(this->self->vistrail))
    return;
  this->self->idMap.clear();
  VersionVector::const_iterator it;
  for (it=this->self->uncommitted.begin();
       it!=this->self->uncommitted.end(); it++) {
    Version *version = (*it);
    VersionId oldId = version->id();
    version->setParent(this->getUpdatedId(version->parent()));
    this->self->vistrail->addVersion(version);
    this->self->idMap[oldId] = version->id();
  }
}

void Session::commit(HMerge *handler)
{
  this->self->storage->save(this->self->vistrail, this->info());
  this->self->uncommitted.clear();
}

void Session::discard()
{
  this->self->uncommitted.clear();
}

Vistrail* Session::vistrail() const
{
  return this->self->vistrail;
}

void Session::setVistrail(Vistrail *vistrail)
{
  this->self->vistrail = vistrail;
}

const VersionId & Session::getUpdatedId(const VersionId &id)
{
  IdMap::const_iterator i = this->self->idMap.find(id);
  if (i!=this->self->idMap.end())
    return (*i).second;
  return id;
}

VT_END_NAMESPACE
