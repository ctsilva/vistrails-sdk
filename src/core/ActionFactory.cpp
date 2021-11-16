/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "ActionFactory.h"
#include "global_private.h"
#include "Action.h"
#include <boost/unordered_map.hpp>

VT_BEGIN_NAMESPACE

static ActionFactory* __factory = NULL;

typedef boost::unordered_map<std::string, ActionCreator*> CreatorMap;

class ActionFactory::PrivateImpl
{
public:
  CreatorMap creatorMap;
};


VT_DEFINE_PIMPL(ActionFactory);

ActionFactory::ActionFactory(): self(new PrivateImpl())
{
}

void ActionFactory::registerAction(ActionCreator *creator)
{
  this->self->creatorMap[creator->actionId()] = creator;
}

Action* ActionFactory::createAction(const char *name)
{
  CreatorMap::iterator it = this->self->creatorMap.find(name);
  if (it!=this->self->creatorMap.end())
    return (*it).second->createAction();
  return NULL;
}

ActionFactory* ActionFactory::globalFactory()
{
  if (!__factory) {
    __factory = new ActionFactory();
  }
  return __factory;
}

VT_END_NAMESPACE
