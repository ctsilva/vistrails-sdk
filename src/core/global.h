/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef GLOBAL_H
#define GLOBAL_H

#define VT_BEGIN_NAMESPACE namespace vt {
#define VT_END_NAMESPACE }
#define VT_USING_NAMESPACE using namespace vt;

#define VT_DECLARE_PIMPL(CLASSNAME)             \
public:                                         \
 CLASSNAME(const CLASSNAME &);                  \
 CLASSNAME& operator=(const CLASSNAME &);       \
 virtual ~CLASSNAME();                          \
private:                                        \
 class PrivateImpl;                             \
 PrivateImpl *self;                             \
 friend class PrivateImpl;

#if defined (_WIN32) 
  #if defined(VisTrails_EXPORTS)
    #define  VT_EXPORT __declspec(dllexport)
  #else
    #define  VT_EXPORT __declspec(dllimport)
  #endif /* VisTrails_EXPORTS */
#else /* VT (_WIN32) */
  #define VT_EXPORT
#endif

#endif
