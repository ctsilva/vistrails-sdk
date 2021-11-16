/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "XmlStorage.h"
#include "global_private.h"

#include "tinyxml/tinyxml.h"
#include <string>
#include <vector>

VT_BEGIN_NAMESPACE

class XmlStorage::PrivateImpl
{
public:
  PrivateImpl();
  ~PrivateImpl();
  std::string filename;
  TiXmlDocument * doc;
  TiXmlElement * currentVersionElt;
};

XmlStorage::PrivateImpl::PrivateImpl() {
  doc = NULL;
  currentVersionElt = NULL;
}

XmlStorage::PrivateImpl::~PrivateImpl() {
  if (doc) {
    delete doc;
  }
}

VT_DEFINE_PIMPL(XmlStorage);

XmlStorage::XmlStorage(const char * filename): Storage(), self(new PrivateImpl())
{
  this->self->filename = std::string(filename);
}

bool setXmlAttribute(TiXmlElement * elt, Property * prop, std::string attrName)
{
  if (elt == NULL || attrName.empty()) {
    return false;
  }
  
  switch (prop->type()) 
    {
    case PROPERTY_NULL:
    case PROPERTY_BOOL:
    case PROPERTY_DOUBLE:
    case PROPERTY_INT:
    case PROPERTY_INT64:
    case PROPERTY_STRING:
    case PROPERTY_DATETIME:
      elt->SetAttribute(attrName, std::string(prop->toString().asChar()));
    case PROPERTY_DATA:
    case PROPERTY_LIST:
    case PROPERTY_DICTIONARY:
    case PROPERTY_USER:
    default:
      return false;
      break;
    }

}    

Property * getXmlAttribute(TiXmlElement * elt, PropertyType type, 
			   std::string attrName)
{
  Property * prop = NULL;
  const char * attrData = NULL;

  switch (type)
    {
    case PROPERTY_NULL:
      prop = new PNull();
      break;
    case PROPERTY_BOOL:
      attrData = elt->Attribute(attrName.c_str());
      if (attrData && strlen(attrData) > 0) {
	prop = new PBool();
	prop->fromString(PString(attrData));
      }
      break;
    case PROPERTY_DOUBLE:
      attrData = elt->Attribute(attrName.c_str());
      if (attrData && strlen(attrData) > 0) {
	prop = new PDouble();
	prop->fromString(PString(attrData));
      }
      break;
    case PROPERTY_INT:
      attrData = elt->Attribute(attrName.c_str());
      if (attrData && strlen(attrData) > 0) {
	prop = new PInt();
	prop->fromString(PString(attrData));
      }
      break;
    case PROPERTY_INT64:
      attrData = elt->Attribute(attrName.c_str());
      if (attrData && strlen(attrData) > 0) {
	prop = new PInt64();
	prop->fromString(PString(attrData));
      }
      break;
    case PROPERTY_STRING:
      attrData = elt->Attribute(attrName.c_str());
      if (attrData && strlen(attrData) > 0) {
	prop = new PString(attrData);
      }
      break;
    case PROPERTY_DATETIME:
      attrData = elt->Attribute(attrName.c_str());
      if (attrData && strlen(attrData) > 0) {
	prop = new PDateTime();
	prop->fromString(PString(attrData));
      }
      break;
    case PROPERTY_DATA:
    case PROPERTY_LIST:
    case PROPERTY_DICTIONARY:
    case PROPERTY_USER:
    default:
      break;
    }
  return prop;
}

TiXmlElement * createXmlElement(Property * prop, std::string tagName)
{
  TiXmlElement * elt = new TiXmlElement(tagName);
  TiXmlText * text = new TiXmlText(std::string(prop->toString().asChar()));
  elt->LinkEndChild(text);
  return elt;
}

TiXmlElement * setXmlElement(Property * prop, std::string tagName="", 
			     TiXmlElement * parentElt=NULL)
{
  TiXmlElement * elt = NULL;
  switch (prop->type())
    {
    case PROPERTY_NULL:
      if (tagName.empty()) {
	tagName = "_null";
      }
      elt = new TiXmlElement(tagName);
      elt->SetAttribute("type", "null");
      break;
    case PROPERTY_BOOL:
      if (tagName.empty()) {
	tagName = "_bool";
      }
      elt = createXmlElement(prop, tagName);
      elt->SetAttribute("type", "bool");
      break;
    case PROPERTY_DOUBLE:
      if (tagName.empty()) {
	tagName = "_double";
      }
      elt = createXmlElement(prop, tagName);
      elt->SetAttribute("type", "double");
      break;
    case PROPERTY_FLOAT:
      if (tagName.empty()) {
	tagName = "_float";
      }
      elt = createXmlElement(prop, tagName);
      elt->SetAttribute("type", "float");
      break;
    case PROPERTY_INT: 
      if (tagName.empty()) {
	tagName = "_int";
      }
      elt = createXmlElement(prop, tagName);
      elt->SetAttribute("type", "int");
      break;
    case PROPERTY_INT64:
      if (tagName.empty()) {
	tagName = "_int64";
      }
      elt = createXmlElement(prop, tagName);
      elt->SetAttribute("type", "int64");
      break;
    case PROPERTY_STRING:
      {
	if (tagName.empty()) {
	  tagName = "_string";
	}
	elt = new TiXmlElement(tagName);
	TiXmlText * text = new TiXmlText(std::string(prop->asChar()));
	elt->LinkEndChild(text);
	elt->SetAttribute("type", "string");
      }
      break;
    case PROPERTY_DATETIME:
      if (tagName.empty()) {
	tagName = "_datetime";
      }
      elt = createXmlElement(prop, tagName);
      elt->SetAttribute("type", "datetime");
      break;
    case PROPERTY_LIST:
      if (tagName.empty()) {
	tagName = "_list";
      }
      elt = new TiXmlElement(tagName);
      elt->SetAttribute("type", "list");
      if (prop->asList()->count() > 0) {
	PList::Iterator it;
	for(it.init(prop->asList()); it.hasNext(); it.next()) {
	  setXmlElement(it.property(), "", elt);
	}
      }
      break;
    case PROPERTY_DICTIONARY:
      if (tagName.empty()) {
	tagName = "_dict";
      }
      elt = new TiXmlElement(tagName);
      elt->SetAttribute("type", "dict");
      if (prop->asDict()->count() > 0) {
	PDict::Iterator it;
	for(it.init(prop->asDict()); it.hasNext(); it.next()) {
	  setXmlElement(it.property(), it.key(), elt);
	}
      }
      break;
    case PROPERTY_DATA:
      // ### FIXME ### ADD IMPLEMENTATION ###
      if (tagName.empty()) {
	tagName = "_data";
      }
      elt = new TiXmlElement(tagName);
      elt->SetAttribute("type", "data");
      break;
    case PROPERTY_USER:
      // ### FIXME ### ADD IMPLEMENTATION ###
      if (tagName.empty()) {
	tagName = "_user";
      }
      elt = new TiXmlElement(tagName);
      elt->SetAttribute("type", "user");
      break;
    }

  if (parentElt) {
    parentElt->LinkEndChild(elt);
  }
  return elt;
}

Property * getXmlElement(TiXmlElement * elt, Property * prop=NULL)
{
  const char * type = elt->Attribute("type");
  std::string typeStr;
  if (!type || strlen(type) < 0) {
    if (!prop) {
      prop = new PNull();
      typeStr = "";
    }
  } else {
    typeStr = type;
  }

  if (typeStr == "null") {
    if (prop) {
      assert(prop->type() == PROPERTY_NULL);
    } else {
      prop = new PNull();
    }
  } else if (typeStr == "bool") {
    if (prop) {
      assert(prop->type() == PROPERTY_BOOL);
    } else {
      prop = new PBool();
    }
    prop->fromString(PString(elt->GetText()));
  } else if (typeStr == "double") {
    if (prop) {
      assert(prop->type() == PROPERTY_DOUBLE);
    } else {
      prop = new PDouble();
    }
    prop->fromString(PString(elt->GetText()));
  } else if (typeStr == "float") {
    if (prop) {
      assert(prop->type() == PROPERTY_FLOAT);
    } else {
      prop = new PFloat();
    }
    prop->fromString(PString(elt->GetText()));
  } else if (typeStr == "int") {
    if (prop) {
      assert(prop->type() == PROPERTY_INT);
    } else {
      prop = new PInt();
    }
    prop->fromString(PString(elt->GetText()));
  } else if (typeStr == "int64") {
    if (prop) {
      assert(prop->type() == PROPERTY_INT64);
    } else {
      prop = new PInt64();
    }
    prop->fromString(PString(elt->GetText()));
  } else if (typeStr == "datetime") {
    if (prop) {
      assert(prop->type() == PROPERTY_DATETIME);
    } else {
      prop = new PDateTime();
    }
    prop->fromString(PString(elt->GetText()));
  } else if (typeStr == "string") {
    if (prop) {
      assert(prop->type() == PROPERTY_STRING);
    } else {
      prop = new PString();
    }
	const char* text = elt->GetText();
	if(text)
	  prop->asString()->setValue(text);
	else
	  prop->asString()->setValue("");
  } else if (typeStr == "list") {
    if (prop) {
      assert (prop->type() == PROPERTY_LIST);
    } else {
      prop = new PList();
    }
    PList::Iterator it;
    it.init(prop->asList());
    std::vector<Property *> newChildElts;
    for(TiXmlElement *childElt=elt->FirstChildElement(); childElt; 
	childElt = childElt->NextSiblingElement()) {
      Property * childProp = NULL;
      if (it.hasNext()) {
	childProp = it.property();
	it.next();
      }
      childProp = getXmlElement(childElt, childProp);
      newChildElts.push_back(childProp);
    }
    for(std::vector<Property *>::const_iterator jt=newChildElts.begin();
	jt != newChildElts.end(); ++jt) {
      prop->asList()->add(*jt);
    }
  } else if (typeStr == "dict") {
    if (prop) {
      assert (prop->type() == PROPERTY_DICTIONARY);
    } else {
      prop = new PDict();
    }
    PDict::Iterator it;
    it.init(prop->asDict());
    for(TiXmlElement *childElt=elt->FirstChildElement(); childElt; 
	childElt = childElt->NextSiblingElement()) {
      Property * childProp = prop->asDict()->get(childElt->Value());
      childProp = getXmlElement(childElt, childProp);
      prop->asDict()->set(childElt->Value(), childProp);
    }
  } else if (typeStr == "data") {
    // ### FIXME ### ADD IMPLEMENTATION
    if (prop) {
      assert (prop->type() == PROPERTY_DATA);
    } else {
      prop = new PData();
    }
  } else if (typeStr == "user") {
    // ### FIXME ###
    prop = new PNull();
  }
  
  return prop;
}

void XmlStorage::loadBegin()
{
  this->self->doc = new TiXmlDocument(this->self->filename.c_str());
  bool load_okay = this->self->doc->LoadFile();
  assert(load_okay);
}

void XmlStorage::loadVistrailAnnotations(PDict *annotations)
{
  TiXmlElement * rootElement = this->self->doc->RootElement();
  TiXmlElement * annotationsElt = rootElement->FirstChildElement("annotations");
  getXmlElement(annotationsElt, annotations);
}

size_t XmlStorage::loadNumberOfVersions()
{
  TiXmlElement * rootElement = this->self->doc->RootElement();
  Property * countProp = getXmlAttribute(rootElement, PROPERTY_INT64,
					 "numVersions");
  return (size_t) countProp->asInt64();
}

void XmlStorage::loadVersion(VersionId *id, VersionId *parent,
			     PDict *stamp, PDict *annotations, 
			     PDict *actions)
{
  TiXmlElement * rootElement = this->self->doc->RootElement();
  PDict versionDict;
  if (this->self->currentVersionElt == NULL) {
    this->self->currentVersionElt = \
      rootElement->FirstChildElement("versions")->FirstChildElement();
  } else {
    this->self->currentVersionElt = \
      this->self->currentVersionElt->NextSiblingElement();
  }
  versionDict.set("stamp", stamp);
  versionDict.set("annotations", annotations);
  versionDict.set("actions", actions);
  getXmlElement(this->self->currentVersionElt, &versionDict);
  id->fromString(*(versionDict.get("id")->asString()));
  parent->fromString(*(versionDict.get("parent")->asString()));
  // this is needed so the deconstructor works as desired
  versionDict.set("stamp", new PNull());
  versionDict.set("annotations", new PNull());
  versionDict.set("actions", new PNull());
}

void XmlStorage::loadEnd()
{

}

void XmlStorage::saveBegin(PDict *sessionInfo)
{
  if (! this->self->doc) {
    this->self->doc = new TiXmlDocument();
    TiXmlElement * rootElement = new TiXmlElement("vistrail");
    this->self->doc->LinkEndChild(rootElement);
    rootElement->LinkEndChild(new TiXmlElement("versions"));
  }
}

void XmlStorage::saveVistrailAnnotations(PDict *annotations)
{
  TiXmlElement * rootElement = this->self->doc->RootElement();
  setXmlElement(annotations, "annotations", rootElement);
}

void XmlStorage::saveNumberOfVersions(size_t count)
{
  TiXmlElement * rootElement = this->self->doc->RootElement();
  setXmlAttribute(rootElement, new PInt64(count), "numVersions");
}

void XmlStorage::saveVersion(const VersionId &id, 
			     const VersionId &parent,
			     PDict *stamp, PDict *annotations, 
			     PDict *actions)
{
  PDict versionDict;
  versionDict.set("id", id.toString().makeCopy());
  versionDict.set("parent", parent.toString().makeCopy());
  versionDict.set("stamp", stamp);
  versionDict.set("annotations", annotations);
  versionDict.set("actions", actions);
  TiXmlElement * rootElement = this->self->doc->RootElement();
  setXmlElement(&versionDict, "version", 
		rootElement->FirstChildElement("versions"));
  // this is needed so the deconstructor works as desired
  versionDict.set("stamp", new PNull());
  versionDict.set("annotations", new PNull());
  versionDict.set("actions", new PNull());
}

void XmlStorage::saveEnd()
{
  this->self->doc->SaveFile(this->self->filename.c_str());
}

void XmlStorage::fetch(void *dataId, void *output) const
{

}

VT_END_NAMESPACE
