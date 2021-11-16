%module(directors="1") vt_Handler
// %module vt_Handler

%feature("director") HFetchData;
%feature("director") HSaveVistrail;
%feature("director") HLoadVistrail;

#if defined(FALSE)
// #if defined(SWIGJAVA)

// Turn the proxy class into an interface
//%typemap(javaclassmodifiers) vt::HPropertyIO    "public interface";
//%typemap(javaclassmodifiers) vt::HStamp         "public interface";
%typemap(javaclassmodifiers) vt::HFetchData     "public interface";
%typemap(javaclassmodifiers) vt::HSaveVistrail  "public interface";
%typemap(javaclassmodifiers) vt::HLoadVistrail  "public interface";
//%typemap(javabody) vt::HPropertyIO    "";
//%typemap(javabody) vt::HStamp         "";
%typemap(javabody) vt::HFetchData     "";
%typemap(javabody) vt::HSaveVistrail  "";
%typemap(javabody) vt::HLoadVistrail  "";
//%typemap(javafinalize) vt::HPropertyIO   "";
//%typemap(javafinalize) vt::HStamp        "";
%typemap(javafinalize) vt::HFetchData    "";
%typemap(javafinalize) vt::HSaveVistrail "";
%typemap(javafinalize) vt::HLoadVistrail "";
//%typemap(javadestruct) vt::HPropertyIO   "";
//%typemap(javadestruct) vt::HStamp        "";
%typemap(javadestruct) vt::HFetchData    "";
%typemap(javadestruct) vt::HSaveVistrail "";
%typemap(javadestruct) vt::HLoadVistrail "";

// Turn the methods into abstract methods
//%typemap(javaout) const char * vt::HPropertyIO  ::classname ";";
//%typemap(javaout) void vt::HPropertyIO  ::load ";";
//%typemap(javaout) void vt::HPropertyIO  ::save ";";
//%typemap(javaout) void vt::HStamp       ::stamp ";";
%typemap(javaout) void vt::HFetchData   ::fetch ";";
%typemap(javaout) void vt::HSaveVistrail::saveBegin ";";
%typemap(javaout) void vt::HSaveVistrail::saveVistrailAnnotations ";";
%typemap(javaout) void vt::HSaveVistrail::saveNumberOfVersions ";";
%typemap(javaout) void vt::HSaveVistrail::saveVersion ";";
%typemap(javaout) void vt::HSaveVistrail::saveEnd ";";
%typemap(javaout) void vt::HLoadVistrail::loadBegin ";";
%typemap(javaout) void vt::HLoadVistrail::loadVistrailAnnotations ";";
%typemap(javaout) size_t vt::HLoadVistrail::loadNumberOfVersions ";";
%typemap(javaout) void vt::HLoadVistrail::loadVersion ";";
%typemap(javaout) void vt::HLoadVistrail::loadEnd ";";

//%javamethodmodifiers vt::HPropertyIO  ::classname "abstract public";
//%javamethodmodifiers vt::HPropertyIO  ::load "abstract public";
//%javamethodmodifiers vt::HPropertyIO  ::save "abstract public";
//%javamethodmodifiers vt::HStamp       ::stamp "abstract public";
%javamethodmodifiers vt::HFetchData   ::fetch "abstract public";
%javamethodmodifiers vt::HSaveVistrail::saveBegin "abstract public";
%javamethodmodifiers vt::HSaveVistrail::saveVistrailAnnotations "abstract public";
%javamethodmodifiers vt::HSaveVistrail::saveNumberOfVersions "abstract public";
%javamethodmodifiers vt::HSaveVistrail::saveVersion "abstract public";
%javamethodmodifiers vt::HSaveVistrail::saveEnd "abstract public";
%javamethodmodifiers vt::HLoadVistrail::loadBegin "abstract public";
%javamethodmodifiers vt::HLoadVistrail::loadVistrailAnnotations "abstract public";
%javamethodmodifiers vt::HLoadVistrail::loadNumberOfVersions "abstract public";
%javamethodmodifiers vt::HLoadVistrail::loadVersion "abstract public";
%javamethodmodifiers vt::HLoadVistrail::loadEnd "abstract public";

#endif

%{
#include "core/Handler.h"
%}

namespace vt{

  class HMerge
  {
  public:
    virtual ~HMerge() {}
    enum MergeType
      {
        MT_BRANCH = 0,
        MT_MINE_AFTER_THEIRS = 1,
        MT_THEIRS_AFTER_MINE = 2
      };
    virtual MergeType mergeNode(Version*, Version*) { return MT_BRANCH; }
  };

  class HNotify
  {
  public:
    virtual ~HNotify() {}
    virtual void vistrailChanged() {}
    virtual void versionChanged(const VersionId &id) {}
    virtual void versionAdded(Version *version) {}
    virtual void versionSelected(const VersionId &previous,
                                 const VersionId &current) {}
  };

  class HStamp
  {
  public:
    virtual ~HStamp() {}
    virtual void stamp(Session *, Version *) const =0;
  };

  class HFetchData
  {
  public:
    virtual ~HFetchData() {}
    virtual void fetch(void *dataId, void *output) const = 0;
  };

  class HSaveVistrail
  {
  public:
    virtual ~HSaveVistrail() {}
    virtual void saveBegin(PDict *sessionInfo) = 0;
    virtual void saveVistrailAnnotations(PDict *annotations) = 0;
    virtual void saveNumberOfVersions(size_t count) = 0;
    virtual void saveVersion(const VersionId &id, const VersionId &parent,
                             PDict *stamp, PDict *annotations, PDict *actions) = 0;
    virtual void saveEnd() = 0;
  };

  class HLoadVistrail
  {
  public:
    virtual ~HLoadVistrail() {}
    virtual void   loadBegin() = 0;
    virtual void   loadVistrailAnnotations(PDict *annotations) = 0;
    virtual size_t loadNumberOfVersions() = 0;
    virtual void   loadVersion(VersionId *id, VersionId *parent,
                               PDict *stamp, PDict *annotations, PDict *actions) = 0;
    virtual void   loadEnd() = 0;
  };


}
