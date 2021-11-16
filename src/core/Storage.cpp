/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Storage.h"
#include "global_private.h"
#include "Action.h"
#include "ActionFactory.h"
#include "Property.h"
#include "Vistrail.h"
#include <string>

#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>

#include <boost/uuid/uuid_io.hpp>
//#include <tinyxml/tinyxml.h>

VT_BEGIN_NAMESPACE

#define GET_ACTION(NAME)                                  \
  {                                                       \
    PList *NAME = actions.setList(#NAME);                 \
    ActionList *list = version->NAME();                   \
    for (size_t j=0; j<list->count(); j++) {              \
      Action *action = list->get(j);                      \
      PDict *dict = NAME->addDict();                      \
      dict->setString("classname", action->classname());  \
      action->save(dict->setList("data"));                \
    }                                                     \
  }

void Storage::save(Vistrail *vistrail, PDict *sessionInfo)
{
  VersionList versions;
  vistrail->getAllVersions(&versions);
  this->saveBegin(sessionInfo);
  this->saveVistrailAnnotations(vistrail->annotations());
  this->saveNumberOfVersions(versions.count());
  PDict actions;
  for (size_t i=0; i<versions.count(); i++) {
    Version *version = versions.get(i);
    actions.clear();
    GET_ACTION(performed);
    GET_ACTION(inverse);
    GET_ACTION(checkpoint);
    this->saveVersion(version->id(), version->parent(),
                         version->stamp(), version->annotations(),
                         &actions);
  }
  this->saveEnd();
}

#define SET_ACTION(NAME)                                                \
  {                                                                     \
    PList *NAME = actions.get(#NAME)->asList();                         \
    ActionList *list = version->NAME();                                 \
    for (size_t j=0; j<NAME->count(); j++) {                            \
      PDict *dict = NAME->get(j)->asDict();                             \
      Property *classname = dict->get("classname");                     \
      Action *action = factory->createAction(classname->asChar());      \
      action->load(dict->get("data")->asList());                        \
      list->add(action);                                                \
    }                                                                   \
  }

void Storage::load(Vistrail *vistrail)
{
  this->loadBegin();
  this->loadVistrailAnnotations(vistrail->annotations());
  size_t vcount = this->loadNumberOfVersions();
  VersionId id, parent;
  PDict actions;
  ActionFactory *factory = ActionFactory::globalFactory();
  for (size_t i=0; i<vcount; i++) {
    Version *version = new Version();
    actions.clear();
    this->loadVersion(&id, &parent,
                         version->stamp(), version->annotations(),
                         &actions);
    version->setId(id);
    version->setParent(parent);
    SET_ACTION(performed);
    SET_ACTION(inverse);
    SET_ACTION(checkpoint);
    vistrail->addVersion(version);
  }
  this->loadEnd();
}

bool Storage::update(Vistrail *vistrail)
{
  this->load(vistrail);
  return true;
}

// XMLStorage::XMLStorage(const char *fname) {
//   this->fname = fname;
//   std::cout << "setting fname: " << this->fname << ":" << std::endl;
// }

// void XMLStorage::save(Vistrail *vistrail) {
//   std::string my_file = this->fname;
//   //std::cout << "1 saving to " << this->fname << std::endl;
  
//   XMLDAO * dao = XMLDAOFactory::getGlobalFactory()->getVistrailXMLDAO();
//   TiXmlDocument out_doc;
//   VersionTree::PrivateImpl * version_tree = vistrail->getVersionTree()->self;
//   //DBVistrail * db_vistrail = dynamic_cast<DBVistrail *>(version_tree);
//   for(DBVistrail_action_container::iterator it=version_tree->db_get_actions().begin(); it != version_tree->db_get_actions().end(); ++it) {
//     std::cout << (*it)->db_get_id() << " : " << std::endl;
//     DBAction_data_container datas = (*it)->db_get_datas();
//     for(DBAction_data_container::iterator jt=datas.begin();
// 	jt != datas.end();
// 	++jt) {
//       std::cout << "DATA:" << (*jt)->db_get_id() << " : " 
// 		<< (*jt)->db_get_function() << std::endl;
//     }
//   }
//   //std::cout << "2 saving to " << this->fname << std::endl;
//   TiXmlElement * out_elt = dao->toXML(version_tree);
//   out_doc.LinkEndChild(out_elt);
//   //std::cout << "3 saving to " << my_file << std::endl;
//   out_doc.SaveFile(my_file);
//   // delete dao;
// }

// void XMLStorage::load(Vistrail *vistrail) {
//   XMLDAO * dao = XMLDAOFactory::getGlobalFactory()->getVistrailXMLDAO();
//   TiXmlDocument doc(this->fname);
//   bool load_okay = doc.LoadFile();
//   assert(load_okay);
//   TiXmlElement * elt = doc.RootElement();
//   DBDomainObj * d_obj = dao->fromXML(elt);
//   VersionTree::PrivateImpl * version_tree = 
//     dynamic_cast<VersionTree::PrivateImpl *>(d_obj);
//   // vistrail->getVersionTree()->clear();
//   vistrail->getVersionTree()->self = version_tree;
//   // vistrail->getVersionTree()->clear();
//   // for(DBVistrail_action_container::iterator it=version_tree->db_get_actions().begin(); it != version_tree->db_get_actions().end(); ++it) {
//   //   vistrail->getVersionTree()->createVersion((Action *) (*it)->db_get_data()->db_get_action_data(), (*it)->db_get_prevId(), (*it)->db_get_id());
//   // }
//   // delete dao;
// }

/*
struct VersionInfo
{
  VersionId id;
  VersionId parent;
  
  off_t offset;
  size_t actionNameSize;
  size_t dataSize;  
};

class FolderStorage::PrivateImpl
{
public:
  std::string folder;
};

VT_DEFINE_PIMPL(FolderStorage);

FolderStorage::FolderStorage(const char *folder): self(new PrivateImpl())
{
  this->self->folder = std::string(folder);
}

void FolderStorage::save(Vistrail *vistrail)
{
  mkdir(this->self->folder.c_str(), 0777);
  
  size_t count;
  FILE *findex = fopen((this->self->folder+"/index").c_str(), "wb");  
  FILE *fdata = fopen((this->self->folder+"/main").c_str(), "wb");

  Version *versions = vistrail->getVersionTree()->getAllVersions(&count);
  count--;
  fwrite(&count, sizeof(count), 1, findex);
  for (size_t i=0; i<=count; i++) {
    Version *version = versions+i;
    if (version->getId()!=VERSION_ROOT) {
      MultiStream streams;
      version->getAction()->serialize(&streams);
      DataStream *data = streams.defaultStream();
      VersionInfo info;
      info.id = version->getId();
      info.parent = version->getParent();
      
      std::string actionName = version->getAction()->getActionType();
      info.offset = ftello(fdata);
      info.actionNameSize = actionName.size();
      info.dataSize = data->size();
      
      fwrite(actionName.data(), 1, info.actionNameSize, fdata);
      fwrite(data->data(), 1, info.dataSize, fdata);
      fwrite(&info, sizeof(info), 1, findex);
    }
  }
  
  fclose(findex);
  fclose(fdata);
}

void FolderStorage::load(Vistrail *vistrail)
{
  FILE *findex = fopen((this->self->folder+"/index").c_str(), "rb");
  if (!findex) return;
  
  FILE *fdata = fopen((this->self->folder+"/main").c_str(), "rb");
  if (!fdata) return;
  
  VersionTree *tree = vistrail->getVersionTree();
  tree->clear();

  size_t count;
  VersionInfo info;
  char buffer[1024];
  Action *action;
  fread(&count, sizeof(count), 1, findex);
  
  for (size_t i=0; i<count; i++) {
    fread(&info, sizeof(info), 1, findex);
    fseeko(fdata, info.offset, SEEK_SET);
    
    fread(buffer, 1, info.actionNameSize, fdata);
    buffer[info.actionNameSize] = 0;
    action = ActionFactory::getGlobalFactory()->createAction(buffer);
    
    MultiStream streams;
    fread(buffer, 1, info.dataSize, fdata);
    streams.defaultStream()->write(buffer, info.dataSize);

    action->deserialize(&streams);
    tree->createVersion(action, info.parent, info.id);
  }
  
  fclose(findex);
  fclose(fdata);
}
*/

VT_END_NAMESPACE
