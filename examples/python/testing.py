
#
# Copyright (c) 2009-2011 VisTrails, Inc..
#
# This file is part of VisTrails SDK
# (see https://github.com/vistrails/sdk).
#
# License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
#


from PyVisTrails import (
    Action as _Action,
    ActionCreator as _ActionCreator,
    ActionFactory as _ActionFactory,
    Version, Session, SessionFocus, XmlStorage,
)


class Action(_Action):
    _action_id = None

    def classname(self):
        if self._action_id is None:
            raise Exception("Action subclass must set _action_id")
        return self._action_id

    @classmethod
    def creator(cls):
        return ActionFactory.globalFactory().registerAction(cls)

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


class RectAction(Action):
    _action_id = "com.vistrails.provexp.example.rectaction"

    def __init__(self, width=0.0, height=0.0):
        Action.__init__(self)
        self.width = width
        self.height = height

    def load(self, plist):
        print("DESERIALIZING rect")
        self.width = plist.get(0).asFloat()
        self.height = plist.get(1).asFloat()

    def save(self, plist):
        print("SERIALIZING rect", self.width, self.height)
        plist.addFloat(self.width)
        plist.addFloat(self.height)


class OvalAction(Action):
    _action_id = "com.vistrails.provexp.example.ovalaction"

    def __init__(self, radius=0.0):
        Action.__init__(self)
        self.radius = radius

    def load(self, plist):
        print("DESERIALIZING oval")
        self.radius = plist.get(0).asFloat()

    def save(self, plist):
        print("SERIALIZING oval", self.radius)
        plist.addFloat(self.radius)


def run():
    factory = ActionFactory.globalFactory()
    factory.registerAction(OvalAction)
    factory.registerAction(RectAction)

    session = Session()
    focus = SessionFocus(session)
    print("starting createProxy")
    print("done createProxy()")

    # Hold Python references so the C++ side doesn't see freed actions.
    keep_alive = []

    def hold(action):
        keep_alive.append(action)
        return action

    # First version: oval
    v1 = Version()
    v1.performed().add(hold(OvalAction(2.0)))
    focus.record(v1)

    # Second version (child of v1): rect
    v2 = Version()
    v2.performed().add(hold(RectAction(3.0, 4.0)))
    focus.record(v2)

    # Branch back to v1, then add oval
    focus.selectParent()
    v3 = Version()
    v3.performed().add(hold(OvalAction(5.5)))
    focus.record(v3)

    # Branch with inverse: a rect with an inverse rect on undo
    v4 = Version()
    v4.performed().add(hold(RectAction(7.0, 8.0)))
    v4.inverse().add(hold(RectAction(7.0, 8.0)))
    focus.record(v4)

    print("focus currentVersion:", focus.currentVersion())

    storage_out = XmlStorage("vtdemo_out.xml")
    session.save(storage_out)
    return (session, focus, keep_alive)


if __name__ == '__main__':
    run()
