/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "add_action_xml.h"
#include "add_action.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace uf {

DBDomainObj * DBAddActionXMLDAOBase::fromXML(TiXmlElement * elt)
{
  assert(elt);
  std::string elt_tag = elt->ValueStr();
  if (elt_tag[0] == '{') {
    elt_tag = elt_tag.substr(elt_tag.find_first_of("}"));
  }
  // assert(elt_tag == "addAction");

  // read attributes
  std::string _attr_data;
  int itemId = -1;
  if (elt->Attribute("itemId")) {
    _attr_data = elt->Attribute("itemId");
    if (!_attr_data.empty()) {
      itemId = boost::lexical_cast<int>(_attr_data);
    }
  }
  int itemType = -1;
  if (elt->Attribute("itemType")) {
    _attr_data = elt->Attribute("itemType");
    if (!_attr_data.empty()) {
      itemType = boost::lexical_cast<int>(_attr_data);
    }
  }
  double posX = std::numeric_limits<double>::signaling_NaN();
  if (elt->Attribute("posX")) {
    _attr_data = elt->Attribute("posX");
    if (!_attr_data.empty()) {
      posX = boost::lexical_cast<double>(_attr_data);
    }
  }
  double posY = std::numeric_limits<double>::signaling_NaN();
  if (elt->Attribute("posY")) {
    _attr_data = elt->Attribute("posY");
    if (!_attr_data.empty()) {
      posY = boost::lexical_cast<double>(_attr_data);
    }
  }
  int r = -1;
  if (elt->Attribute("r")) {
    _attr_data = elt->Attribute("r");
    if (!_attr_data.empty()) {
      r = boost::lexical_cast<int>(_attr_data);
    }
  }
  int g = -1;
  if (elt->Attribute("g")) {
    _attr_data = elt->Attribute("g");
    if (!_attr_data.empty()) {
      g = boost::lexical_cast<int>(_attr_data);
    }
  }
  int b = -1;
  if (elt->Attribute("b")) {
    _attr_data = elt->Attribute("b");
    if (!_attr_data.empty()) {
      b = boost::lexical_cast<int>(_attr_data);
    }
  }
   
    
  DBAddAction * obj = DBDomainFactory::getGlobalFactory()->createAddAction();
  obj->init(itemId, itemType, posX, posY, r, g, b);
  obj->is_dirty = false;
  return obj;
}

TiXmlElement * DBAddActionXMLDAOBase::toXML(const DBDomainObj * vt_obj,
					      TiXmlElement * elt)
{
  const DBAddAction * obj = dynamic_cast<const DBAddAction *>(vt_obj);
  if (elt == NULL) {
    elt = new TiXmlElement("addAction");
  }
        
  // set attributes
  std::string _attr_data = "";
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_itemId());
  elt->SetAttribute("itemId", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_itemType());
  elt->SetAttribute("itemType", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_posX());
  elt->SetAttribute("posX", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_posY());
  elt->SetAttribute("posY", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_r());
  elt->SetAttribute("r", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_g());
  elt->SetAttribute("g", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_b());
  elt->SetAttribute("b", _attr_data);
        
  return elt;
}

}
