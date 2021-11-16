/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef SESSION_FOCUS_H
#define SESSION_FOCUS_H

#include "global.h"
#include "Version.h"
#include "Handler.h"

VT_BEGIN_NAMESPACE

class Action;
class ActionList;
class Session;

class VT_EXPORT SessionFocus
{
  VT_DECLARE_PIMPL(SessionFocus);
public:
  SessionFocus(Session *session);

  Session * session();
  
  const VersionId & currentVersion() const;
  bool              autoSelectVersion() const;
  void              setAutoSelectVersion(bool value);
  void              reset();
  
  const VersionId & selectVersion(const VersionId & id);
  const VersionId & selectVersionByTag(const char *tagName);
  
  const VersionId & selectParent();
  const VersionId & selectLastChild();
  const VersionId & selectRedoChild();

  const VersionId & getParent();
  const VersionId & getLastChild();
  const VersionId & getRedoChild();

  Version *         record(Action *action);
  Version *         record(const ActionList &actions);
  Version *         record(Version *version);

  void              addHandler(HNotify *notify);
  void              removeHandler(HNotify *notify);

  void              versionChanged(const VersionId &id);
  void              vistrailChanged();

  void              prune(const VersionId &id);
  bool              inRedoHistory(const VersionId &id);
  
//   void getVersionPathTo(VersionId dst, VersionList *path);
//   void getPathTo(VersionId dst, ActionPath *path);
};

VT_END_NAMESPACE

#endif
