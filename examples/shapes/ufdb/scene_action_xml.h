/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef UF_DB_XML_SCENE_ACTION
#define UF_DB_XML_SCENE_ACTION

#include "tinyxml/tinyxml.h"
#include "xmlfactory.h"
#include "domainfactory.h"

namespace uf {

class DBSceneActionXMLDAOBase : public XMLDAO
{
UF_XML_DAO_PROXY_DEF(DBSceneActionXMLDAOBase)
public:
  DBSceneActionXMLDAOBase() { }

  DBDomainObj * fromXML(TiXmlElement * elt);
  TiXmlElement * toXML(const DBDomainObj * vt_obj, TiXmlElement * elt=NULL);
};

}

#endif
