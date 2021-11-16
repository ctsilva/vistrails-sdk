%module vt_ActionFactory

%{
#include "core/ActionFactory.h"
%}

namespace vt {
  class ActionFactory
  {
  public:
    ActionFactory();
    void    registerAction(ActionCreator *proxy);
    Action *createAction(const char *actionId);
    
    static ActionFactory *globalFactory();
  };
}
