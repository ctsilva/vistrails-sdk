%module vt_VersionTreeTraversal

%{
#include "core/VersionTreeTraversal.h"
%}

namespace vt {
  enum TraverseType
    {
      TRAVERSE_FAST=0,
      TRAVERSE_BFS=1,
      TRAVERSE_DFS=2,
      TRAVERSE_EDGE=3
    };

  class VersionTreeTraversal
  {
    friend class VersionTreeLayout;
  public:
    VersionTreeTraversal(VersionTree *tree=NULL);

    void          setVersionTree(VersionTree *tree);
    VersionTree * versionTree();

    void              traverse(const VersionId &root, TraverseType tt);
    bool              done();
    void              next(bool traverseCurrent=true);
    const VersionId & current();

    const VersionId & fastParent();
    const VersionId & fastFirstChild();
    const VersionId & fastLastChild();
    const VersionId & fastNextSibling();
    const VersionId & fastPrevSibling();

    const VersionId & parent();
    const VersionId & firstChild();
    const VersionId & lastChild();
    const VersionId & nextSibling();
    const VersionId & prevSibling();
    
    const VersionId & edgeParent();
  };
}
