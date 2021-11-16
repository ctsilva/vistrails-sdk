/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Action.h"
#include "global_private.h"
#include <vector>

VT_BEGIN_NAMESPACE

typedef std::vector<Action *> ActionVector;

class ActionList::PrivateImpl
{
public:
  PrivateImpl() { items.clear(); }
  ActionVector items;
};

VT_DEFINE_PIMPL(ActionList);

ActionList::ActionList(): self(new PrivateImpl)
{
}

ActionList::ActionList(Action *action): self(new PrivateImpl)
{
  this->add(action);
}

void ActionList::clear()
{
  return this->self->items.clear();
}

size_t ActionList::count() const
{
  return this->self->items.size();
}

void ActionList::set(size_t index, Action *action) const
{
  if (index<this->self->items.size())
    this->self->items[index] = action;
}

void ActionList::add(Action * action)
{
  this->self->items.push_back(action);
}

void ActionList::add(const ActionList &actions)
{
  this->self->items.insert(this->self->items.end(),
                           actions.self->items.begin(),
                           actions.self->items.end());
}

Action * ActionList::get(size_t index) const
{
  return this->self->items[index];
}

VT_END_NAMESPACE
