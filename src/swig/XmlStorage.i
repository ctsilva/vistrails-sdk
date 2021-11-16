%module vt_XmlStorage

%{
#include "core/XmlStorage.h"
%}

namespace vt {
  class XmlStorage : public Storage
  {
  public:
    XmlStorage(const char * filename);

    // Load
    virtual void   loadBegin();
    virtual void   loadVistrailAnnotations(PDict *annotations);
    virtual size_t loadNumberOfVersions();
    virtual void   loadVersion(VersionId *id, VersionId *parent,
                               PDict *stamp, PDict *annotations, PDict *actions);
    virtual void   loadEnd();

    // Save
    virtual void saveBegin(PDict *sessionInfo);
    virtual void saveVistrailAnnotations(PDict *annotations);
    virtual void saveNumberOfVersions(size_t count);
    virtual void saveVersion(const VersionId &id, const VersionId &parent,
                             PDict *stamp, PDict *annotations, PDict *actions);
    virtual void saveEnd();

    // Fetch
    virtual void fetch(void *dataId, void *output) const;
  };
}
