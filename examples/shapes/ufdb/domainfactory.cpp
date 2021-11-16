/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "domainfactory.h"

#include "add_action.h"
#include "scene_action.h"
#include "shape.h"
#include "delete_action.h"
#include "move_action.h"

namespace uf {

DBDomainFactory::DBDomainFactory()
{
  this->addActionProxy = DBAddAction::createProxy();
  this->sceneActionProxy = DBSceneAction::createProxy();
  this->shapeProxy = DBShape::createProxy();
  this->deleteActionProxy = DBDeleteAction::createProxy();
  this->moveActionProxy = DBMoveAction::createProxy();
}

DBAddAction * DBDomainFactory::createAddAction() {
  DBDomainObj *obj = this->addActionProxy->createObject();
  DBAddAction * cast_obj = dynamic_cast<DBAddAction *>(obj);
  return cast_obj;
}
DBSceneAction * DBDomainFactory::createSceneAction() {
  DBDomainObj *obj = this->sceneActionProxy->createObject();
  DBSceneAction * cast_obj = dynamic_cast<DBSceneAction *>(obj);
  return cast_obj;
}
DBShape * DBDomainFactory::createShape() {
  DBDomainObj *obj = this->shapeProxy->createObject();
  DBShape * cast_obj = dynamic_cast<DBShape *>(obj);
  return cast_obj;
}
DBDeleteAction * DBDomainFactory::createDeleteAction() {
  DBDomainObj *obj = this->deleteActionProxy->createObject();
  DBDeleteAction * cast_obj = dynamic_cast<DBDeleteAction *>(obj);
  return cast_obj;
}
DBMoveAction * DBDomainFactory::createMoveAction() {
  DBDomainObj *obj = this->moveActionProxy->createObject();
  DBMoveAction * cast_obj = dynamic_cast<DBMoveAction *>(obj);
  return cast_obj;
}

boost::posix_time::ptime DBDomainFactory::convertDateTimeFromStr(std::string value) {
  boost::posix_time::ptime pt;
  boost::posix_time::time_input_facet * f(new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S"));
  std::stringstream ss(value);
  ss.imbue(std::locale(std::locale::classic(), f));
  ss >> pt;
  return pt;
}
 
std::string DBDomainFactory::convertDateTimeToStr(const boost::posix_time::ptime & pt) {
  boost::posix_time::time_facet * f(new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S"));
  std::stringstream ss;
  ss.imbue(std::locale(std::locale::classic(), f));
  ss << pt;
  return ss.str();
}

}    
