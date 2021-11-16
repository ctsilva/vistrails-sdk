
#
# Copyright (c) 2009-2011 VisTrails, Inc..
#
# This file is part of VisTrails SDK
# (see https://github.com/vistrails/sdk).
#
# License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
#


import vistrails

class PyActionProxy(vistrails.PyActionProxy):
    def __init__(self):
        vistrails.PyActionProxy.__init__(self)
        self.set_create_cb(getattr(self, 'createObject'))

    def createObject(self):
        return self._klass()

class PyAction(vistrails.PyAction):
    def __init__(self, my_type, my_version, my_function="normal"):
        vistrails.PyAction.__init__(self, my_type, my_version, my_function)
        if hasattr(self, 'serialize'):
            self.set_serialize_cb(getattr(self, 'serialize'))
        if hasattr(self, 'deserialize'):
            self.set_deserialize_cb(getattr(self, 'deserialize'))

    @classmethod
    def createProxy(klass):
        new_klass = type(klass.__name__ + "Proxy", (PyActionProxy,), 
                         {'_klass': klass})
        # print "new_klass:", new_klass.__name__
        return new_klass()
            
class RectAction(PyAction):
    def __init__(self, function="normal"):
        PyAction.__init__(self, "com.vistrails.provexp.example.rectaction",
                          "0.1", function)
        self.a_var = "rect"
        self.elt = None
    def serialize(self, s_type):
        print "SERIALIZING", s_type, self.a_var
        self.elt = vistrails.TiXmlElement("rect")
        vistrails.create_xml_stream(self, "default", 1, self.elt)
    def deserialize(self):
        print "DESERIALIZING", self.a_var

class OvalAction(PyAction):
    def __init__(self, function="normal"):
        PyAction.__init__(self, "com.vistrails.provexp.example.ovalaction",
                          "0.1", function)
        self.a_var = "oval"
        self.elt = None
    def serialize(self, s_type):
        print "SERIALIZING", s_type, self.a_var
        self.elt = vistrails.TiXmlElement("oval")
        vistrails.create_xml_stream(self, "default", 1, self.elt)
    def deserialize(self):
        print "DESERIALIZING", self.a_var

class RectActionInverse(RectAction):
    def __init__(self):
        RectAction.__init__(self, "inverse")

class OvalActionInverse(OvalAction):
    def __init__(self):
        OvalAction.__init__(self, "inverse")

class RectActionCheckpoint(RectAction):
    def __init__(self):
        RectAction.__init__(self, "checkpoint")
    
class OvalActionCheckpoint(OvalAction):
    def __init__(self):
        OvalAction.__init__(self, "checkpoint")

def run(vt_changed_cb=None):
    vistrails.py_initialize(vt_changed_cb)
    session = vistrails.Session()
    session.setUser("dakoop@gmail.com")
    print "user:", session.getUser()
    focus = vistrails.SessionFocus(session)
    print "starting createProxy"
    vistrails.set_data_proxy(OvalAction.createProxy(),
                             "com.vistrails.provexp.example.ovalaction")
    vistrails.set_data_proxy(RectAction.createProxy(),
                             "com.vistrails.provexp.example.rectaction")
    vistrails.set_data_proxy(OvalActionInverse.createProxy(),
                             "com.vistrails.provexp.example.ovalinverse")
    print "done createProxy()"

    focus.record(OvalAction())
    focus.record(RectAction())
    focus.selectParent()
    focus.record(OvalAction())
    focus.record(RectAction(), RectActionInverse())

    storage = vistrails.XMLStorage("vtdemo.xml")
    session.bind(storage)
    session.update()
    print 'focus currentVersion:', focus.getCurrentVersion()
    print 'session updated id:', session.getUpdatedId(focus.getCurrentVersion())
    focus.selectVersion(session.getUpdatedId(focus.getCurrentVersion()))

    storage_out = vistrails.XMLStorage("vtdemo_out.xml")
    storage_out.save(session.getVistrail())

    # tree = session.getVistrail().getVersionTree()
    # version_ids = ["00000000-0000-0000-0000-000000000000"]
    # while len(version_ids) > 0:
    #     version_id = version_ids.pop(0)
    #     version = tree.getVersion(version_id)
    #     if version != None:
    #         print version.getId(), version.getParent(), version.getUser(), version.getDate()
    #     child_id = tree.getFirstChild(version_id)
    #     while child_id != "":
    #         version_ids.append(child_id)
    #         child_id = tree.getNextSibling(child_id)
    # print 'tree id:', id(tree)
    return (session, focus)

if __name__ == '__main__':
    run()
