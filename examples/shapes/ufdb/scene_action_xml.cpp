/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "scene_action_xml.h"
#include "scene_action.h"
#include "shape.h"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace uf {

DBDomainObj * DBSceneActionXMLDAOBase::fromXML(TiXmlElement * elt)
{
  assert(elt);
  std::string elt_tag = elt->ValueStr();
  if (elt_tag[0] == '{') {
    elt_tag = elt_tag.substr(elt_tag.find_first_of("}"));
  }
  // assert(elt_tag == "sceneAction");
        
  std::vector<DBShape *> shapes;
        
  // read children
  TiXmlElement * _child_elt = elt->FirstChildElement();
  while (_child_elt) {
    std::string child_tag = _child_elt->ValueStr();
    if (child_tag[0] == '{') {
      child_tag = child_tag.substr(child_tag.find_first_of("}"));
    }
    
    if (child_tag == "shape") {
      DBDomainObj * _vt_obj = XMLDAOFactory::getGlobalFactory()->getShapeXMLDAO()->fromXML(_child_elt);
      DBShape * _data = dynamic_cast<DBShape *>(_vt_obj);
      shapes.push_back(_data);
    } else if (!_child_elt->Value()) {
    } else {
	assert(172 == 173);
	//print '*** ERROR *** tag = %s' % child.tag
    }
    _child_elt = _child_elt->NextSiblingElement();
  }
   
    
  DBSceneAction * obj = DBDomainFactory::getGlobalFactory()->createSceneAction();
  obj->init(shapes);
  obj->is_dirty = false;
  return obj;
}

TiXmlElement * DBSceneActionXMLDAOBase::toXML(const DBDomainObj * vt_obj,
					      TiXmlElement * elt)
{
  const DBSceneAction * obj = dynamic_cast<const DBSceneAction *>(vt_obj);
  if (elt == NULL) {
    elt = new TiXmlElement("sceneAction");
  }
        
  // set elements
  DBSceneAction_shape_container shapes = obj->db_get_shapes();
  for(DBSceneAction_shape_container::iterator it=shapes.begin(); 
       it != shapes.end(); ++it) {
    DBShape * shape = *it;
    TiXmlElement * _child = new TiXmlElement("shape");
    _child = XMLDAOFactory::getGlobalFactory()->getShapeXMLDAO()->toXML(shape, _child);
    elt->LinkEndChild(_child);
  }
        
  return elt;
}

}
