
#
# Copyright (c) 2009-2011 VisTrails, Inc..
#
# This file is part of VisTrails SDK
# (see https://github.com/vistrails/sdk).
#
# License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
#


from PyVisTrails import Action as _Action, ActionCreator as _ActionCreator, \
    ActionFactory as _ActionFactory, \
    Version, ActionList, Session, SessionFocus, XmlStorage

class Action(_Action):
    _action_id = None
    
    def classname(self):
        if self._action_id is None:
            raise Exception("Action subclass must set _action_id")
        return self._action_id

    @classmethod
    def creator(klass):
        return ActionFactory.globalFactory().registerAction(klass)

    def load(self, plist):
        raise Exception("Action subclass must define load method")

    def save(self, plist):
        raise Exception("Action subclass must define save method")

class ActionCreator(_ActionCreator):
    def __init__(self, klass):
        _ActionCreator.__init__(self)
        self._klass = klass
        self._actions = []
    
    def createAction(self):
        action = self._klass()
        self._actions.append(action)
        return action
        
    def actionId(self):
        return self._klass._action_id

class ActionFactory(object):
    _instance = None

    def __init__(self):
        self._inner_factory = _ActionFactory.globalFactory()
        self._creators = {}

    def registerAction(self, action_klass):
        if action_klass not in self._creators:
            creator = ActionCreator(action_klass)
            self._creators[action_klass] = creator
            self._inner_factory.registerAction(creator)
        return self._creators[action_klass]

    @staticmethod
    def globalFactory():
        if ActionFactory._instance is None:
            ActionFactory._instance = ActionFactory()
        return ActionFactory._instance

class OvalAction(Action):
    _action_id = "com.vistrails.example.OvalAction"

    def __init__(self, radius=0.0):
        Action.__init__(self)
        self.radius = radius

    def load(self, plist):
        self.radius = plist.get(0).asFloat()

    def save(self, plist):
        plist.addFloat(self.radius)

class RectAction(Action):
    _action_id = "com.vistrails.example.RectAction"

    def __init__(self, width=0.0, height=0.0):
        Action.__init__(self)
        self.width = width
        self.height = height

    def load(self, plist):
        self.width = plist.get(0).asFloat()
        self.height = plist.get(1).asFloat()

    def save(self, plist):
        plist.addFloat(self.width)
        plist.addFloat(self.height)

def main():
    factory = ActionFactory.globalFactory()
    factory.registerAction(OvalAction)
    factory.registerAction(RectAction)

    session = Session()
    focus = SessionFocus(session)
    in_storage = XmlStorage("test_vt.xml");
    session.load(in_storage);

    version = Version()
    action1 = OvalAction(5.5)
    action2 = RectAction(3.5, 4.5)
    version.performed().add(action1)
    version.performed().add(action2)
    focus.record(version)

    version2 = Version()
    action3 = OvalAction(8.5)
    version2.performed().add(action3)
    focus.record(version2)

    out_storage = XmlStorage("test_vt_out.xml")
    session.save(out_storage);

if __name__ == '__main__':
    main()
