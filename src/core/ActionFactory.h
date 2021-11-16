/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef ACTION_FACTORY_H
#define ACTION_FACTORY_H

#include "global.h"

VT_BEGIN_NAMESPACE

class Action;
class ActionCreator;

class VT_EXPORT ActionFactory
{
  VT_DECLARE_PIMPL(ActionFactory);
public:
  ActionFactory();
  void    registerAction(ActionCreator *proxy);
  Action *createAction(const char *actionId);
  
  static ActionFactory *globalFactory();
};

VT_END_NAMESPACE

#endif
