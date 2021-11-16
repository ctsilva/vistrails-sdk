/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef GLOBAL_PRIVATE_H
#define GLOBAL_PRIVATE_H

#include "global.h"

#define VT_DEFINE_PIMPL_COPY(CLASSNAME)                   \
  CLASSNAME::CLASSNAME(const CLASSNAME &other)            \
      : self(new PrivateImpl(*other.self)) {}             \
  CLASSNAME& CLASSNAME::operator=(const CLASSNAME &other) \
  {                                                       \
    if (this != &other) {                                 \
      delete this->self;                                  \
      this->self = new PrivateImpl(*other.self);          \
    }                                                     \
    return *this;                                         \
  }                                                       \

#define  VT_DEFINE_PIMPL_DESTRUCTOR(CLASSNAME)      \
  CLASSNAME::~CLASSNAME() { delete this->self; }

#define VT_DEFINE_PIMPL(CLASSNAME)              \
  VT_DEFINE_PIMPL_COPY(CLASSNAME)               \
  VT_DEFINE_PIMPL_DESTRUCTOR(CLASSNAME)

VT_BEGIN_NAMESPACE

VT_EXPORT void reportWarning(const char *format, ...);
VT_EXPORT void reportError(const char *format, ...);

VT_END_NAMESPACE

#endif
