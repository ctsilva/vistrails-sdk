%module(directors="1") vt_Action

%feature("director") Action;
%feature("director") ActionCreator;

%{
#include "core/Action.h"
#include "core/Handler.h"
%}

namespace vt {
  class Action : public HPropertyIO 
  {  
  };

  class ActionCreator
  {
  public:
    virtual ~ActionCreator() {}
    virtual Action *createAction() = 0;
    virtual const char *actionId() const = 0;
  };

  class ActionList
  {
    VT_DECLARE_PIMPL(ActionList);
  public:
    ActionList();
    ActionList(Action *action);

    size_t   count() const;
    void     clear();
    void     set(size_t index, Action *action) const;
    void     add(Action * action);
    void     add(const ActionList &action);
    Action * get(size_t idx) const;
  };

}
