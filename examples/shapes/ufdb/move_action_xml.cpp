/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "move_action_xml.h"
#include "move_action.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace uf {

DBDomainObj * DBMoveActionXMLDAOBase::fromXML(TiXmlElement * elt)
{
  assert(elt);
  std::string elt_tag = elt->ValueStr();
  if (elt_tag[0] == '{') {
    elt_tag = elt_tag.substr(elt_tag.find_first_of("}"));
  }
  // assert(elt_tag == "moveAction");

  // read attributes
  std::string _attr_data;
  int itemId = -1;
  if (elt->Attribute("itemId")) {
    _attr_data = elt->Attribute("itemId");
    if (!_attr_data.empty()) {
      itemId = boost::lexical_cast<int>(_attr_data);
    }
  }
  double oldPosX = std::numeric_limits<double>::signaling_NaN();
  if (elt->Attribute("oldPosX")) {
    _attr_data = elt->Attribute("oldPosX");
    if (!_attr_data.empty()) {
      oldPosX = boost::lexical_cast<double>(_attr_data);
    }
  }
  double oldPosY = std::numeric_limits<double>::signaling_NaN();
  if (elt->Attribute("oldPosY")) {
    _attr_data = elt->Attribute("oldPosY");
    if (!_attr_data.empty()) {
      oldPosY = boost::lexical_cast<double>(_attr_data);
    }
  }
  double newPosX = std::numeric_limits<double>::signaling_NaN();
  if (elt->Attribute("newPosX")) {
    _attr_data = elt->Attribute("newPosX");
    if (!_attr_data.empty()) {
      newPosX = boost::lexical_cast<double>(_attr_data);
    }
  }
  double newPosY = std::numeric_limits<double>::signaling_NaN();
  if (elt->Attribute("newPosY")) {
    _attr_data = elt->Attribute("newPosY");
    if (!_attr_data.empty()) {
      newPosY = boost::lexical_cast<double>(_attr_data);
    }
  }
   
    
  DBMoveAction * obj = DBDomainFactory::getGlobalFactory()->createMoveAction();
  obj->init(itemId, oldPosX, oldPosY, newPosX, newPosY);
  obj->is_dirty = false;
  return obj;
}

TiXmlElement * DBMoveActionXMLDAOBase::toXML(const DBDomainObj * vt_obj,
					      TiXmlElement * elt)
{
  const DBMoveAction * obj = dynamic_cast<const DBMoveAction *>(vt_obj);
  if (elt == NULL) {
    elt = new TiXmlElement("moveAction");
  }
        
  // set attributes
  std::string _attr_data = "";
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_itemId());
  elt->SetAttribute("itemId", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_oldPosX());
  elt->SetAttribute("oldPosX", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_oldPosY());
  elt->SetAttribute("oldPosY", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_newPosX());
  elt->SetAttribute("newPosX", _attr_data);
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_newPosY());
  elt->SetAttribute("newPosY", _attr_data);
        
  return elt;
}

}
