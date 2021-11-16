/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "global.h"
#include "global_private.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

VT_BEGIN_NAMESPACE

void reportWarning(const char *format, ...) {
  va_list argList;
  va_start(argList, format);
  fprintf(stderr, "WARNING: ");
  vfprintf(stderr, format, argList);
  fprintf(stderr, "\n");
  va_end(argList);
}

void reportError(const char *format, ...) {
  va_list argList;
  va_start(argList, format);
  fprintf(stderr, "ERROR: ");
  vfprintf(stderr, format, argList);
  fprintf(stderr, "\n");
  va_end(argList);
  exit(0);
}

// void initialize() {
//   DBDomainFactory * factory = DBDomainFactory::getGlobalFactory();
//   factory->setDBStreamProxy(DBXMLStream::createProxy(),
// 			    "com.vistrails.provexp.tinyxml");
//   factory->setDBStreamProxy(DBRawStream::createProxy(),
// 			    "com.vistrails.provexp.raw");
//   factory->setDBVistrailProxy(VersionTree::createProxy());
//   factory->setDBActionProxy(Version::createProxy());
//   XMLDAOFactory * xmlFactory = XMLDAOFactory::getGlobalFactory();
//   xmlFactory->setStreamXMLDAOProxy(DBStreamXMLDAO::createProxy());
//   xmlFactory->setDataXMLDAOProxy(DBDataXMLDAO::createProxy());
// }

// void set_data_proxy(DBDomainProxy * proxy, const char * subclass_id)
// {
//   DBDomainFactory * factory = DBDomainFactory::getGlobalFactory();
//   factory->setDBDataProxy(proxy, subclass_id);
// }

// void create_xml_stream(Action * action, const char * name, int inlined, 
// 		       TiXmlElement * elt)
// {
//   DBStream * stream = DBDomainFactory::getGlobalFactory()->createStream(
//     "com.vistrails.provexp.tinyxml");
//   DBXMLStream * xml_stream = dynamic_cast<DBXMLStream *>(stream);
//   std::string name_str = name;
//   xml_stream->db_set_name(name_str);
//   int my_inlined = inlined;
//   xml_stream->db_set_inlined(my_inlined);
//   xml_stream->setXmlElement(elt);
//   std::vector<vt::DBStream *> streams;
//   streams.push_back(xml_stream);
//   action->db_set_streams(streams);
// }

VT_END_NAMESPACE
