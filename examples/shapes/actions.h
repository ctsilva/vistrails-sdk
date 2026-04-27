/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QColor>
#include <QPointF>
#include <QRandomGenerator>

#include "core/Action.h"
#include "core/Handler.h"

#include "tinyxml/tinyxml.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include <iostream>
#include <vector>

#include "ufdb/add_action.h"
#include "ufdb/move_action.h"
#include "ufdb/delete_action.h"
#include "ufdb/scene_action.h"
#include "ufdb/shape.h"
#include "ufdb/domainfactory.h"
#include "ufdb/xmlfactory.h"

#ifndef ACTIONS_H
#define ACTIONS_H

class DiagramAction : public vt::Action
{
public:
  //virtual QUndoCommand *buildCommand(DiagramScene *scene) = 0;
  virtual void execute(DiagramScene *scene) = 0;
  virtual uf::XMLDAO * get_dao() const = 0;
  virtual uf::DBDomainObj * get_data() const = 0;
  virtual void set_data(uf::DBDomainObj * data) = 0;

  // vt::Action interface methods
  // const char *classname() const;
  void save(vt::PList *list) const;
  void load(vt::PList *list);
};

class AddAction : public DiagramAction
{
  //VT_DB_DOMAIN_PROXY_DEF(AddAction);
  VT_DECLARE_ACTION(AddAction, "AddAction");
public:
  AddAction();
  AddAction(DiagramItem::DiagramType itemType, QPointF pos, 
	    DiagramScene * scene, int id, QColor color=Qt::transparent);

  virtual uf::XMLDAO * get_dao() const { 
    return uf::XMLDAOFactory::getGlobalFactory()->getAddActionXMLDAO(); 
  }
  virtual void set_data(uf::DBDomainObj * obj) {
    this->data = dynamic_cast<uf::DBAddAction *>(obj);
  }
  virtual uf::DBDomainObj * get_data() const {
    return this->data;
  }

  virtual void execute(DiagramScene *scene);

private:
  uf::DBAddAction * data;
};

class MoveAction : public DiagramAction
{
  //VT_DB_DOMAIN_PROXY_DEF(MoveAction);
  VT_DECLARE_ACTION(MoveAction, "MoveAction");
public:
  MoveAction();
  MoveAction(DiagramItem * movedItem, const QPointF & position);

  virtual uf::XMLDAO * get_dao() const { 
    return uf::XMLDAOFactory::getGlobalFactory()->getMoveActionXMLDAO(); 
  }
  virtual void set_data(uf::DBDomainObj * obj) {
    this->data = dynamic_cast<uf::DBMoveAction *>(obj);
  }
  virtual uf::DBDomainObj * get_data() const {
    return this->data;
  }

  virtual void execute(DiagramScene *scene);

private:
  uf::DBMoveAction * data;
};

class DeleteAction : public DiagramAction
{
  //VT_DB_DOMAIN_PROXY_DEF(DeleteAction);
  VT_DECLARE_ACTION(DeleteAction, "DeleteAction");
public:
  DeleteAction();
  DeleteAction(int itemId);

  virtual uf::XMLDAO * get_dao() const { 
    return uf::XMLDAOFactory::getGlobalFactory()->getDeleteActionXMLDAO(); 
  }
  virtual void set_data(uf::DBDomainObj * obj) {
    this->data = dynamic_cast<uf::DBDeleteAction *>(obj);
  }
  virtual uf::DBDomainObj * get_data() const {
    return this->data;
  }

  virtual void execute(DiagramScene *scene);

private:
  uf::DBDeleteAction * data;
};

class SceneAction : public DiagramAction
{
  //VT_DB_DOMAIN_PROXY_DEF(SceneAction);
  VT_DECLARE_ACTION(SceneAction, "SceneAction");
public:
  SceneAction();
  SceneAction(DiagramScene * scene);

 virtual uf::XMLDAO * get_dao() const { 
    return uf::XMLDAOFactory::getGlobalFactory()->getSceneActionXMLDAO(); 
  }
  virtual void set_data(uf::DBDomainObj * obj) {
    this->data = dynamic_cast<uf::DBSceneAction *>(obj);
  }
  virtual uf::DBDomainObj * get_data() const {
    return this->data;
  }

  virtual void execute(DiagramScene *scene);

private:
  uf::DBSceneAction * data;
};

#endif
