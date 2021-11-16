%module vt_HPropertyIO

%{
#include "core/Handler.h"
%}

namespace vt{

  class HPropertyIO
  {
  public:
    virtual ~HPropertyIO() {}
    virtual const char *classname() const = 0;
    virtual void load(PList *plist) = 0;
    virtual void save(PList *plist) const = 0;
  };
  
}