%module vt_Session

%feature("notabstract");

%{
#include "core/Session.h"
%}

namespace vt {
  class Session: public HStamp
  {
  public:
    Session();
    PDict *           info();
    void              record(Version *version);
    Vistrail *        vistrail() const;
    void              setVistrail(Vistrail *vistrail);

    void              save(Storage *);
    void              load(Storage *);

    void              setStampHandler(HStamp *stamper);
    void              stamp(Session *, Version *) const;

    void              bind(Storage *storage);
    void              update(HMerge *handler=NULL);
    void              commit(HMerge *handler=NULL);
    void              discard();
    const VersionId & getUpdatedId(const VersionId &id);
  };
}
