%module vt_VersionTree

%{
#include "core/VersionTree.h"
%}

namespace vt {
  class VersionTree
  {
    friend class VersionTreeTraversal;  
  public:
    VersionTree();

    void add(const VersionId &id, const VersionId &parent);

    const VersionId & getParent(const VersionId &id);
    const VersionId & getFirstChild(const VersionId &id);
    const VersionId & getLastChild(const VersionId &id);
    const VersionId & getNextSibling(const VersionId &id);
    const VersionId & getPrevSibling(const VersionId &id);

    size_t getNumberOfVersions();

    void clear();
  };
}
