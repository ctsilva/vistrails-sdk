/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef UF_DB_DOMAIN_FACTORY_H
#define UF_DB_DOMAIN_FACTORY_H

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/unordered_map.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <boost/uuid/uuid_io.hpp>


#define UF_DB_DOMAIN_PROXY_DEF(CLASSNAME) \
public: \
  class CLASSNAME##Proxy: public uf::DBDomainProxy { \
  public: \
    virtual uf::DBDomainObj *createObject() { return new CLASSNAME(); } \
  }; \
  static uf::DBDomainProxy *createProxy() {return new CLASSNAME##Proxy();}

namespace uf {

class DBDomainObj
{
public:
  virtual const char * getDomainType() { return domainType.c_str(); }
  virtual std::string db_get_id() const { 
    return boost::lexical_cast<std::string>(boost::uuids::nil_uuid()); 
  }
protected:
  std::string domainType;
};

class DBDomainProxy
{
public:
  virtual ~DBDomainProxy() {}
  virtual DBDomainObj *createObject()=0;
};

class DBAddAction;
class DBSceneAction;
class DBShape;
class DBDeleteAction;
class DBMoveAction;

class DBDomainFactory
{
public:
  // just default to the known base classes
  DBDomainFactory();

  void setDBAddActionProxy(DBDomainProxy * proxy) {
    this->addActionProxy = proxy;
  }
  DBAddAction * createAddAction();
  void setDBSceneActionProxy(DBDomainProxy * proxy) {
    this->sceneActionProxy = proxy;
  }
  DBSceneAction * createSceneAction();
  void setDBShapeProxy(DBDomainProxy * proxy) {
    this->shapeProxy = proxy;
  }
  DBShape * createShape();
  void setDBDeleteActionProxy(DBDomainProxy * proxy) {
    this->deleteActionProxy = proxy;
  }
  DBDeleteAction * createDeleteAction();
  void setDBMoveActionProxy(DBDomainProxy * proxy) {
    this->moveActionProxy = proxy;
  }
  DBMoveAction * createMoveAction();

  static DBDomainFactory *getGlobalFactory() {
    static DBDomainFactory* factory = NULL;
    if (!factory) {
      factory = new DBDomainFactory();
    }
    return factory;
  }

  static boost::posix_time::ptime convertDateTimeFromStr(std::string value);
  static std::string convertDateTimeToStr(const boost::posix_time::ptime & pt);

private:
  DBDomainProxy * addActionProxy;
  DBDomainProxy * sceneActionProxy;
  DBDomainProxy * shapeProxy;
  DBDomainProxy * deleteActionProxy;
  DBDomainProxy * moveActionProxy;
};

}

#endif
