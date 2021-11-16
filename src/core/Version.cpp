/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Version.h"
#include "global_private.h"
#include "Action.h"
#include "Property.h"
#include <boost/lexical_cast.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <vector>

VT_BEGIN_NAMESPACE

typedef boost::uuids::uuid UUID;
#define PUUID ((UUID*)(this->data))

VersionId::VersionId()
{
  *PUUID = boost::uuids::nil_generator()();
};

VersionId::VersionId(const char *str)
{
  *PUUID = boost::uuids::string_generator()(str);
};

VersionId::VersionId(const PString &pstr)
{
  this->fromString(pstr);
};

PString VersionId::toString() const
{
  const std::string tmp = boost::lexical_cast<std::string>(*((UUID*)(this->data)));
  const char * value = tmp.c_str();
  return PString(value);
};

void VersionId::fromString(const PString &pstr) 
{
  *PUUID = boost::uuids::string_generator()(pstr.asChar());
}

VersionId VersionId::root()
{
  return VersionId("00000000-0000-0000-0000-000000000000");
}

VersionId VersionId::invalid()
{
  return VersionId("ffffffff-ffff-ffff-ffff-ffffffffffff");
}

VersionId VersionId::gen()
{
  VersionId tmp;
  *((UUID*)(&(tmp.data))) = boost::uuids::random_generator()();
  return tmp;
}

class Version::PrivateImpl
{
public:
  VersionId  id;
  VersionId  parent;
  ActionList performed;
  ActionList inverse;
  ActionList checkpoint;
  PDict      stamp;
  PDict      annotations;
};

VT_DEFINE_PIMPL(Version);

Version::Version(): self(new PrivateImpl)
{
  this->self->id = VERSION_INVALID;
  this->self->parent = VERSION_INVALID;
}

const VersionId & Version::id() const
{
  return this->self->id;
}

void Version::setId(const VersionId &id)
{
  this->self->id = id;
}

const VersionId & Version::parent() const
{
  return this->self->parent;
}

void Version::setParent(const VersionId &parent)
{
  this->self->parent = parent;
}

PDict * Version::stamp()
{
  return &this->self->stamp;
}

PDict * Version::annotations()
{
  return &this->self->annotations;
}

ActionList * Version::performed()
{
  return &this->self->performed;
}

ActionList * Version::inverse()
{
  return &this->self->inverse;
}

ActionList * Version::checkpoint()
{
  return &this->self->checkpoint;
}

typedef std::vector<Version *> VersionVector;

class VersionList::PrivateImpl
{
public:
  PrivateImpl() { items.clear(); }
  VersionVector items;
};

VT_DEFINE_PIMPL(VersionList);

VersionList::VersionList(): self(new PrivateImpl)
{
}

void VersionList::clear()
{
  return this->self->items.clear();
}

size_t VersionList::count() const
{
  return this->self->items.size();
}

void VersionList::set(size_t index, Version *version) const
{
  if (index<this->self->items.size())
    this->self->items[index] = version;
}

void VersionList::add(Version * version)
{
  this->self->items.push_back(version);
}

Version * VersionList::get(size_t index) const
{
  return this->self->items[index];
}

VersionPath::VersionPath(): VersionList()
{
}

void VersionPath::setRootIndex(size_t idx)
{
  this->rootIndex = idx;
}

void VersionPath::addRoot(Version * version)
{
  this->add(version);
  this->setRootIndex(this->count()-1);
}

size_t VersionPath::getRootIndex()
{
  return rootIndex;
}

Version * VersionPath::getRoot()
{
  return this->get(this->rootIndex);
}

VT_END_NAMESPACE
