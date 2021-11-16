/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef UF_XML_DAO_FACTORY_H
#define UF_XML_DAO_FACTORY_H

#include "domainfactory.h"
#include "tinyxml/tinyxml.h"
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

#define UF_XML_DAO_PROXY_DEF(CLASSNAME) \
public: \
  class CLASSNAME##Proxy: public uf::XMLDAOProxy { \
  public: \
    virtual uf::XMLDAO *createDao() { return new CLASSNAME(); } \
  }; \
  static uf::XMLDAOProxy *createProxy() {return new CLASSNAME##Proxy();}

namespace uf {

class XMLDAO
{
public:
  virtual const char * getXMLType() { return xmlType.c_str(); }
  virtual DBDomainObj * fromXML(TiXmlElement * elt) = 0;
  virtual TiXmlElement * toXML(const DBDomainObj * vt_obj, 
			       TiXmlElement * elt=NULL) = 0;
protected:
  std::string xmlType;
};

class XMLDAOProxy
{
public:
  virtual ~XMLDAOProxy() {}
  virtual XMLDAO *createDao()=0;
};

class DBAddActionXMLDAOBase;
class DBSceneActionXMLDAOBase;
class DBShapeXMLDAOBase;
class DBDeleteActionXMLDAOBase;
class DBMoveActionXMLDAOBase;

class XMLDAOFactory
{
public:
  // just default to the known base classes
  XMLDAOFactory();

  void setAddActionXMLDAOProxy(XMLDAOProxy * proxy) {
    // void the old static dao
    if (XMLDAOFactory::addActionXMLDAO != NULL) {
      delete XMLDAOFactory::addActionXMLDAO;
      XMLDAOFactory::addActionXMLDAO = NULL;
    }
    this->addActionXMLDAOProxy = proxy;
  }
  XMLDAO * getAddActionXMLDAO();
  void setSceneActionXMLDAOProxy(XMLDAOProxy * proxy) {
    // void the old static dao
    if (XMLDAOFactory::sceneActionXMLDAO != NULL) {
      delete XMLDAOFactory::sceneActionXMLDAO;
      XMLDAOFactory::sceneActionXMLDAO = NULL;
    }
    this->sceneActionXMLDAOProxy = proxy;
  }
  XMLDAO * getSceneActionXMLDAO();
  void setShapeXMLDAOProxy(XMLDAOProxy * proxy) {
    // void the old static dao
    if (XMLDAOFactory::shapeXMLDAO != NULL) {
      delete XMLDAOFactory::shapeXMLDAO;
      XMLDAOFactory::shapeXMLDAO = NULL;
    }
    this->shapeXMLDAOProxy = proxy;
  }
  XMLDAO * getShapeXMLDAO();
  void setDeleteActionXMLDAOProxy(XMLDAOProxy * proxy) {
    // void the old static dao
    if (XMLDAOFactory::deleteActionXMLDAO != NULL) {
      delete XMLDAOFactory::deleteActionXMLDAO;
      XMLDAOFactory::deleteActionXMLDAO = NULL;
    }
    this->deleteActionXMLDAOProxy = proxy;
  }
  XMLDAO * getDeleteActionXMLDAO();
  void setMoveActionXMLDAOProxy(XMLDAOProxy * proxy) {
    // void the old static dao
    if (XMLDAOFactory::moveActionXMLDAO != NULL) {
      delete XMLDAOFactory::moveActionXMLDAO;
      XMLDAOFactory::moveActionXMLDAO = NULL;
    }
    this->moveActionXMLDAOProxy = proxy;
  }
  XMLDAO * getMoveActionXMLDAO();

  static XMLDAOFactory *getGlobalFactory() {
    static XMLDAOFactory* factory = NULL;
    if (!factory) {
      factory = new XMLDAOFactory();
    }
    return factory;
  }

  static boost::posix_time::ptime convertDateTimeFromStr(std::string value);
  static std::string convertDateTimeToStr(const boost::posix_time::ptime & pt);
private:
  XMLDAOProxy * addActionXMLDAOProxy;
  XMLDAOProxy * sceneActionXMLDAOProxy;
  XMLDAOProxy * shapeXMLDAOProxy;
  XMLDAOProxy * deleteActionXMLDAOProxy;
  XMLDAOProxy * moveActionXMLDAOProxy;
  static XMLDAO * addActionXMLDAO;
  static XMLDAO * sceneActionXMLDAO;
  static XMLDAO * shapeXMLDAO;
  static XMLDAO * deleteActionXMLDAO;
  static XMLDAO * moveActionXMLDAO;
};

}

#endif
