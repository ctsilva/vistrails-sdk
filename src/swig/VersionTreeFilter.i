%module vt_VersionTreeFilter

%{
#include "core/VersionTreeFilter.h"
%}

namespace vt {
  class VersionTreeFilter
  {
  public:
    VersionTreeFilter(VersionTree *vt);
    
    void         setVersionTree(VersionTree *tree);
    VersionTree* getVersionTree();

    virtual bool mustShow(const VersionId &id);
    virtual bool mustHide(const VersionId &id);
    
    virtual void rebuild();
    VersionTree *getFilteredTree();
  };
}
