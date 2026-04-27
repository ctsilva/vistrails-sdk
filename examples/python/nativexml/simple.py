
#
# Copyright (c) 2009-2011 VisTrails, Inc..
#
# This file is part of VisTrails SDK
# (see https://github.com/vistrails/sdk).
#
# License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
#


from xml.etree import ElementTree as ET
import PyVisTrails
from PyVisTrails import Action as _Action, ActionCreator as _ActionCreator, \
    ActionFactory as _ActionFactory, \
    Version, ActionList, Session, SessionFocus, Storage, \
    Property, PBool, PDouble, PFloat, PInt, PInt64, PDateTime, PString, PList, \
    PDict, PData, PListIterator, PDictIterator

PNull = Property

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

class XmlStorage(Storage):
    def __init__(self, fname):
        Storage.__init__(self)
        self._fname = fname
        self._doc = None
        self._current_version_iter = None

    def set_xml_attribute(self, elt, prop, attr_name):
        if elt is None or not attr_name:
            return False

        if prop.type() in set([PyVisTrails.PROPERTY_NULL,
                               PyVisTrails.PROPERTY_BOOL,
                               PyVisTrails.PROPERTY_DOUBLE,
                               PyVisTrails.PROPERTY_INT,
                               PyVisTrails.PROPERTY_INT64,
                               PyVisTrails.PROPERTY_STRING,
                               PyVisTrails.PROPERTY_DATETIME]):
            elt.set(attr_name, prop.toString().asChar())
            return True
        return False

    def get_xml_attribute(self, elt, prop_type, attr_name):
        prop = None
        prop_dict = {PyVisTrails.PROPERTY_BOOL: PBool,
                     PyVisTrails.PROPERTY_DOUBLE: PDouble,
                     PyVisTrails.PROPERTY_INT: PInt,
                     PyVisTrails.PROPERTY_INT64: PInt64,
                     PyVisTrails.PROPERTY_STRING: PString,
                     PyVisTrails.PROPERTY_DATETIME: PDateTime}
        if prop_type in prop_dict:
            attr_data = elt.get(attr_name)
            if attr_data:
                prop = prop_dict[prop_type]()
                prop.fromString(PString(attr_data))
        elif prop_type == PyVisTrails.PROPERTY_NULL:
            prop = PNull()

        return prop

    def create_xml_element(self, prop, tag_name):
        elt = ET.Element(tag_name)
        elt.text = prop.toString().asChar()
        return elt

    def set_xml_element(self, prop, tag_name="", parent_elt=None):
        elt = None
        prop_dict = {PyVisTrails.PROPERTY_BOOL: "bool",
                     PyVisTrails.PROPERTY_DOUBLE: "double",
                     PyVisTrails.PROPERTY_FLOAT: "float",
                     PyVisTrails.PROPERTY_INT: "int",
                     PyVisTrails.PROPERTY_INT64: "int64",
                     PyVisTrails.PROPERTY_DATETIME: "datetime"}

        if prop.type() in prop_dict:
            if not tag_name:
                tag_name = "_" + prop_dict[prop.type()]
            elt = self.create_xml_element(prop, tag_name)
            elt.set("type", prop_dict[prop.type()])
        elif prop.type() == PyVisTrails.PROPERTY_NULL:
            if not tag_name:
                tag_name = "_null"
            elt = ET.Element(tag_name)
            elt.set("type", "null")
        elif prop.type() == PyVisTrails.PROPERTY_STRING:
            if not tag_name:
                tag_name = "_string"
            elt = ET.Element(tag_name)
            elt.text = prop.asChar()
            elt.set("type", "string")
        elif prop.type() == PyVisTrails.PROPERTY_LIST:
            if not tag_name:
                tag_name = "_list"
            elt = ET.Element(tag_name)
            elt.set("type", "list")
            for i in range(prop.asList().count()):
                self.set_xml_element(prop.asList().get(i), "", elt)
        elif prop.type() == PyVisTrails.PROPERTY_DICTIONARY:
            if not tag_name:
                tag_name = "_dict"
            elt = ET.Element(tag_name)
            elt.set("type", "dict")
            it = PDictIterator()
            it.init(prop.asDict())
            while it.hasNext():
                self.set_xml_element(it.property(), it.key(), elt)
                it.next()
        elif prop.type() == PyVisTrails.PROPERTY_DATA:
            if not tag_name:
                tag_name = "_data"
            elt = ET.Element(tag_name)
            elt.set("type", "data")
        elif prop.type() == PyVisTrails.PROPERTY_USER:
            # FIXME
            if not tag_name:
                tag_name = "_user"
            elt = ET.Element(tag_name)
            elt.set("type", "user")

        if parent_elt is not None:
            parent_elt.append(elt)
        return elt

    def get_xml_element(self, elt, prop=None):
        type = elt.get("type")
        if not type:
            if prop is None:
                prop = PNull()
                type = ""
        
        if type == "null":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_NULL)
            else:
                prop = PNull()
        elif type == "bool":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_BOOL)
            else:
                prop = PBool()
            prop.fromString(PString(elt.text))
        elif type == "double":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_DOUBLE)
            else:
                prop = PDouble()
            prop.fromString(PString(elt.text))
        elif type == "float":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_FLOAT)
            else:
                prop = PFloat()
            prop.fromString(PString(elt.text))
        elif type == "int":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_INT)
            else:
                prop = PInt()
            prop.fromString(PString(elt.text))
        elif type == "int64":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_INT64)
            else:
                prop = PInt64()
            prop.fromString(PString(elt.text))
        elif type == "datetime":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_DATETIME)
            else:
                prop = PDateTime()
            prop.fromString(PString(elt.text))
        elif type == "string":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_STRING)
            else:
                prop = PString()
            if elt.text:
                prop.asString().setValue(elt.text)
            else:
                prop.asString().setValue("")
        elif type == "list":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_LIST)
            else:
                prop = PList()
            # go through all child elements here
            idx = 0
            new_child_props = []
            for child_elt in list(elt):
                child_prop = None
                if idx < prop.asList().count():
                    child_prop = prop.asList().get(idx)
                    idx += 1
                child_prop = self.get_xml_element(child_elt, child_prop)
                new_child_props.append(child_prop)
            for child_prop in new_child_props:
                child_prop.thisown = False
                prop.asList().add(child_prop)
        elif type == "dict":
            if prop is not None:
                assert(prop.type() == PyVisTrails.PROPERTY_DICTIONARY)
            else:
                prop = PDict()
            for child_elt in list(elt):
                child_prop = prop.asDict().get(child_elt.tag)
                child_prop = self.get_xml_element(child_elt, child_prop)
                child_prop.thisown = False
                prop.asDict().set(child_elt.tag, child_prop)
        elif type == "data":
            if prop:
                assert(prop.type() == PyVisTrails.PROPERTY_DATA)
            else:
                prop = PData()
        elif type == "user":
            prop = PNull()
        return prop
                
    # load
    def loadBegin(self):
        self._doc = ET.parse(self._fname)

    def loadVistrailAnnotations(self, annotations):
        annotations_elt = self._doc.getroot().find("annotations")
        self.get_xml_element(annotations_elt, annotations)

    def loadNumberOfVersions(self):
        count_prop = self.get_xml_attribute(self._doc.getroot(),
                                            PyVisTrails.PROPERTY_INT64,
                                            "numVersions")
        return count_prop.asInt64()

    def loadVersion(self, id, parent, stamp, annotations, actions):
        if self._current_version_iter is None:
            self._current_version_iter = iter(self._doc.iter("version"))
        elt = next(self._current_version_iter)
        version_dict = PDict()
        version_dict.set("stamp", stamp)
        version_dict.set("annotations", annotations)
        version_dict.set("actions", actions)
        self.get_xml_element(elt, version_dict)
        id.fromString(version_dict.get("id").asString())
        parent.fromString(version_dict.get("parent").asString())

        for tag in ["stamp", "annotations", "actions"]:
            null_obj = PNull()
            null_obj.thisown = False
            version_dict.set(tag, null_obj)

    def loadEnd(self):
        pass

    # save
    def saveBegin(self, sessionInfo):
        if self._doc is None:
            root = ET.Element("vistrail")
            self._doc = ET.ElementTree(root)
            root.append(ET.Element("versions"))

    def saveVistrailAnnotations(self, annotations):
        self.set_xml_element(annotations, "annotations", self._doc.getroot())

    def saveNumberOfVersions(self, count):
        # FIXME fix constructor to differentiate type
        count_prop = PInt64()
        count_prop.setValue(count)
        self.set_xml_attribute(self._doc.getroot(), count_prop, 
                               "numVersions")

    def saveVersion(self, id, parent, stamp, annotations, actions):
        version_dict = PDict()
        version_dict.set("id", id.toString().makeCopy())
        version_dict.set("parent", parent.toString().makeCopy())
        version_dict.set("stamp", stamp)
        version_dict.set("annotations", annotations)
        version_dict.set("actions", actions)
        self.set_xml_element(version_dict, "version", 
                             self._doc.getroot().find("versions"))
        for tag in ["stamp", "annotations", "actions"]:
            null_obj = PNull()
            null_obj.thisown = False
            version_dict.set(tag, null_obj)
        
    def saveEnd(self):
        def indent(elem, level=0):
            i = "\n" + level*"    "
            if len(elem):
                if not elem.text or not elem.text.strip():
                    elem.text = i + "    "
                if not elem.tail or not elem.tail.strip():
                    elem.tail = i
                for elem in elem:
                    indent(elem, level+1)
                if not elem.tail or not elem.tail.strip():
                    elem.tail = i
            else:
                if level and (not elem.tail or not elem.tail.strip()):
                    elem.tail = i
        indent(self._doc.getroot())
        self._doc.write(self._fname)

    # fetch
    def fetch(self, dataId, output):
        pass
    

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
    in_storage = XmlStorage("test_vt.xml")
    session.load(in_storage)
    focus.reset()

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
