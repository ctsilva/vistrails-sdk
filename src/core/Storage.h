/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef STORAGE_H
#define STORAGE_H

#include "global.h"
#include "Handler.h"

VT_BEGIN_NAMESPACE

class Vistrail;

class VT_EXPORT Storage : public HLoadVistrail,
                          public HSaveVistrail,
                          public HFetchData
{
public:
  virtual ~Storage() {}
  virtual void save(Vistrail *vistrail, PDict *sessionInfo);
  virtual void load(Vistrail *vistrail);
  virtual bool update(Vistrail *vistrail);
};

// class VT_EXPORT XMLStorage : public Storage
// {
//   VT_DECLARE_PIMPL();
// public:
//   XMLStorage(const char * fname);
//   virtual void save(Vistrail *vistrail);
//   virtual void load(Vistrail *vistrail);
// };

/*
class FolderStorage: public Storage
{
  VT_DECLARE_PIMPL(FolderStorage);
public:
  FolderStorage(const char *folder);
  virtual void save(Vistrail *vistrail);
  virtual void load(Vistrail *vistrail);
};
*/

VT_END_NAMESPACE

#endif
