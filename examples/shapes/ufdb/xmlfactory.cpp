/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "domainfactory.h"

#include "add_action_xml.h"
#include "scene_action_xml.h"
#include "shape_xml.h"
#include "delete_action_xml.h"
#include "move_action_xml.h"

namespace uf {

XMLDAO * XMLDAOFactory::addActionXMLDAO = NULL;
XMLDAO * XMLDAOFactory::sceneActionXMLDAO = NULL;
XMLDAO * XMLDAOFactory::shapeXMLDAO = NULL;
XMLDAO * XMLDAOFactory::deleteActionXMLDAO = NULL;
XMLDAO * XMLDAOFactory::moveActionXMLDAO = NULL;

XMLDAOFactory::XMLDAOFactory()
{
  this->addActionXMLDAOProxy = DBAddActionXMLDAOBase::createProxy();
  this->sceneActionXMLDAOProxy = DBSceneActionXMLDAOBase::createProxy();
  this->shapeXMLDAOProxy = DBShapeXMLDAOBase::createProxy();
  this->deleteActionXMLDAOProxy = DBDeleteActionXMLDAOBase::createProxy();
  this->moveActionXMLDAOProxy = DBMoveActionXMLDAOBase::createProxy();
}

XMLDAO * XMLDAOFactory::getAddActionXMLDAO() {
  if (XMLDAOFactory::addActionXMLDAO == NULL) {
    XMLDAO * dao = this->addActionXMLDAOProxy->createDao();
    XMLDAOFactory::addActionXMLDAO = dynamic_cast<DBAddActionXMLDAOBase *>(dao);
  }
  return XMLDAOFactory::addActionXMLDAO;
}
XMLDAO * XMLDAOFactory::getSceneActionXMLDAO() {
  if (XMLDAOFactory::sceneActionXMLDAO == NULL) {
    XMLDAO * dao = this->sceneActionXMLDAOProxy->createDao();
    XMLDAOFactory::sceneActionXMLDAO = dynamic_cast<DBSceneActionXMLDAOBase *>(dao);
  }
  return XMLDAOFactory::sceneActionXMLDAO;
}
XMLDAO * XMLDAOFactory::getShapeXMLDAO() {
  if (XMLDAOFactory::shapeXMLDAO == NULL) {
    XMLDAO * dao = this->shapeXMLDAOProxy->createDao();
    XMLDAOFactory::shapeXMLDAO = dynamic_cast<DBShapeXMLDAOBase *>(dao);
  }
  return XMLDAOFactory::shapeXMLDAO;
}
XMLDAO * XMLDAOFactory::getDeleteActionXMLDAO() {
  if (XMLDAOFactory::deleteActionXMLDAO == NULL) {
    XMLDAO * dao = this->deleteActionXMLDAOProxy->createDao();
    XMLDAOFactory::deleteActionXMLDAO = dynamic_cast<DBDeleteActionXMLDAOBase *>(dao);
  }
  return XMLDAOFactory::deleteActionXMLDAO;
}
XMLDAO * XMLDAOFactory::getMoveActionXMLDAO() {
  if (XMLDAOFactory::moveActionXMLDAO == NULL) {
    XMLDAO * dao = this->moveActionXMLDAOProxy->createDao();
    XMLDAOFactory::moveActionXMLDAO = dynamic_cast<DBMoveActionXMLDAOBase *>(dao);
  }
  return XMLDAOFactory::moveActionXMLDAO;
}

boost::posix_time::ptime XMLDAOFactory::convertDateTimeFromStr(std::string value) {
  boost::posix_time::ptime pt;
  boost::posix_time::time_input_facet * f(new boost::posix_time::time_input_facet("%Y-%m-%d %H:%M:%S"));
  std::stringstream ss(value);
  ss.imbue(std::locale(std::locale::classic(), f));
  ss >> pt;
  return pt;
}
 
std::string XMLDAOFactory::convertDateTimeToStr(const boost::posix_time::ptime & pt) {
  boost::posix_time::time_facet * f(new boost::posix_time::time_facet("%Y-%m-%d %H:%M:%S"));
  std::stringstream ss;
  ss.imbue(std::locale(std::locale::classic(), f));
  ss << pt;
  return ss.str();
}

}
