%module vt_SessionFocus

%{
#include "core/SessionFocus.h"
%}

namespace vt {
  class SessionFocus
  {
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
  };
}
