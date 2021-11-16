%module vt_Vistrail

%{
#include "core/Vistrail.h"
using namespace vt;
%}

namespace vt {
  class Vistrail
  {
  public:
    Vistrail();
    
    VersionTree * versionTree();
    void          addVersion(Version *version);
    Version *     getVersion(const VersionId & id) const;
    void          getAllVersions(VersionList *list) const;
    PDict *       annotations();
    
    void          getVersionPath(const VersionId &src,
                                 const VersionId &dst,
                                 VersionPath *path);
  };
}
