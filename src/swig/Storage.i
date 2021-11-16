%module(directors="1") vt_Storage

%feature("director") Storage;

%include "Handler.i";

#if defined(FALSE) 
// #if defined(SWIGJAVA)
//%warnfilter(SWIGWARN_JAVA_MULTIPLE_INHERITANCE) vt::Storage;

// Modify multiple inherited base classes into inheriting interfaces
%typemap(javainterfaces) vt::Storage "HLoadVistrail, HSaveVistrail, HFetchData";
%typemap(javabase, replace="1") vt::Storage "";

// Features are inherited by derived classes, so override this
%javamethodmodifiers vt::Storage::fetch "public";
%javamethodmodifiers vt::Storage::saveBegin "public";
%javamethodmodifiers vt::Storage::saveVistrailAnnotations "public";
%javamethodmodifiers vt::Storage::saveNumberOfVersions "public";
%javamethodmodifiers vt::Storage::saveVersion "public";
%javamethodmodifiers vt::Storage::saveEnd "public";
%javamethodmodifiers vt::Storage::loadBegin "public";
%javamethodmodifiers vt::Storage::loadVistrailAnnotations "public";
%javamethodmodifiers vt::Storage::loadNumberOfVersions "public";
%javamethodmodifiers vt::Storage::loadVersion "public";
%javamethodmodifiers vt::Storage::loadEnd "public";

//make Storage abstract
%feature("abstract") vt::Storage;
%typemap(javaclassmodifiers) vt::Storage "public abstract class";

#endif

%{
#include "core/Storage.h"
%}

namespace vt {
  /* class Storage : public HLoadVistrail, */
  /*   public HSaveVistrail, */
  /*   public HFetchData */
  class Storage
  {
  public:
    virtual ~Storage();
    void save(Vistrail *vistrail, PDict *sessionInfo);
    void load(Vistrail *vistrail);
    bool update(Vistrail *vistrail);

    virtual void   loadBegin() = 0;
    virtual void   loadVistrailAnnotations(PDict *annotations) = 0;
    virtual size_t loadNumberOfVersions() = 0;
    virtual void   loadVersion(VersionId *id, VersionId *parent,
                               PDict *stamp, PDict *annotations, PDict *actions) = 0;
    virtual void   loadEnd() = 0;

    virtual void saveBegin(PDict *sessionInfo) = 0;
    virtual void saveVistrailAnnotations(PDict *annotations) = 0;
    virtual void saveNumberOfVersions(size_t count) = 0;
    virtual void saveVersion(const VersionId &id, const VersionId &parent,
                             PDict *stamp, PDict *annotations, PDict *actions) = 0;
    virtual void saveEnd() = 0;

    virtual void fetch(void *dataId, void *output) const = 0;

  };
}
