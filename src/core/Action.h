/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef ACTION_H
#define ACTION_H

#include "global.h"
#include "Handler.h"

#define VT_DECLARE_ACTION(CLASS, ID)                    \
public:                                                 \
 const char *classname() const { return ID; }           \
 class Creator : public vt::ActionCreator		\
  {                                                     \
  public:                                               \
    vt::Action *createAction() { return new CLASS(); }	\
    const char *actionId() const { return ID; }         \
  };                                                    \
  static Creator *creator() { return new Creator(); }

VT_BEGIN_NAMESPACE

class PList;

class VT_EXPORT Action : public HPropertyIO {};

class VT_EXPORT ActionCreator
{
public:
  virtual ~ActionCreator() {}
  virtual Action *createAction() = 0;
  virtual const char *actionId() const = 0;
};

class VT_EXPORT ActionList
{
  VT_DECLARE_PIMPL(ActionList);
public:
  ActionList();
  ActionList(Action *action);

  size_t   count() const;
  void     clear();
  void     set(size_t index, Action *action) const;
  void     add(Action * action);
  void     add(const ActionList &action);
  Action * get(size_t idx) const;
};

VT_END_NAMESPACE

#endif
