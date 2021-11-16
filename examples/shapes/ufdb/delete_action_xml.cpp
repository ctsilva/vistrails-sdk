/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "delete_action_xml.h"
#include "delete_action.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace uf {

DBDomainObj * DBDeleteActionXMLDAOBase::fromXML(TiXmlElement * elt)
{
  assert(elt);
  std::string elt_tag = elt->ValueStr();
  if (elt_tag[0] == '{') {
    elt_tag = elt_tag.substr(elt_tag.find_first_of("}"));
  }
  // assert(elt_tag == "deleteAction");

  // read attributes
  std::string _attr_data;
  int itemId = -1;
  if (elt->Attribute("itemId")) {
    _attr_data = elt->Attribute("itemId");
    if (!_attr_data.empty()) {
      itemId = boost::lexical_cast<int>(_attr_data);
    }
  }
   
    
  DBDeleteAction * obj = DBDomainFactory::getGlobalFactory()->createDeleteAction();
  obj->init(itemId);
  obj->is_dirty = false;
  return obj;
}

TiXmlElement * DBDeleteActionXMLDAOBase::toXML(const DBDomainObj * vt_obj,
					      TiXmlElement * elt)
{
  const DBDeleteAction * obj = dynamic_cast<const DBDeleteAction *>(vt_obj);
  if (elt == NULL) {
    elt = new TiXmlElement("deleteAction");
  }
        
  // set attributes
  std::string _attr_data = "";
  _attr_data = "";
  _attr_data = boost::lexical_cast<std::string>(obj->db_get_itemId());
  elt->SetAttribute("itemId", _attr_data);
        
  return elt;
}

}
