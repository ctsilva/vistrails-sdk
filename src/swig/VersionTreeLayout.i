%module vt_VersionTreeLayout

%{
#include "core/VersionTreeLayout.h"
%}

namespace vt {
  class LayoutProperties
  {
  public:
    virtual ~LayoutProperties();
    virtual void setVersionPos(const VersionId &id, float X, float Y)=0;
    virtual float siblingDistance(const VersionId &left, const VersionId &right)=0;
  };

  class VersionTreeLayout
  {
  public:
    VersionTreeLayout();
    void layout(VersionTree *tree, LayoutProperties *props);
  };
}
