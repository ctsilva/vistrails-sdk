/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "actions.h"
//#include "db/vt_stream.h"
#include "VisTrails.h"

void DiagramAction::save(vt::PList *list) const
{
  TiXmlElement * elt = this->get_dao()->toXML(this->get_data());
  TiXmlPrinter printer;
  elt->Accept(&printer);
  list->addString(printer.CStr());
  delete elt;
}

void DiagramAction::load(vt::PList *list)
{
  TiXmlDocument doc;
  doc.Parse(list->get(0)->asString()->asChar());
  uf::DBDomainObj * obj = this->get_dao()->fromXML(doc.RootElement());
  this->set_data(obj);
}

AddAction::AddAction() : DiagramAction() 
{ 
  std::cout << "ADD_ACTION DEFAULT CONSTRUCTOR" << std::endl;
  std::string type = "com.vistrails.provexp.example.addaction"; 
  this->data = new uf::DBAddAction();
  
  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

AddAction::AddAction(DiagramItem::DiagramType addType, QPointF pos, 
		     DiagramScene * scene, int id, QColor color) : \
  DiagramAction()
{
  int r,g,b;

  std::string type = "com.vistrails.provexp.example.addaction";
  data = new uf::DBAddAction();
  
  if (color==Qt::transparent) {
    auto *rng = QRandomGenerator::global();
    color = QColor(rng->bounded(256), rng->bounded(256), rng->bounded(256));
  }
  color.getRgb(&r, &g, &b);
  printf("RGB FIRST %d %d %d\n", r,g,b);
  data->db_set_r(r);
  data->db_set_g(g);
  data->db_set_b(b);
  data->db_set_itemType((int) addType);
  data->db_set_posX((double) pos.x());
  data->db_set_posY((double) pos.y());
  data->db_set_itemId(id);

  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

void AddAction::execute(DiagramScene *scene)
{
  DiagramItem::DiagramType addType =
    (DiagramItem::DiagramType) data->db_get_itemType();
  QColor color = QColor(data->db_get_r(), data->db_get_g(), data->db_get_b());
  printf("RGB %d %d %d\n", data->db_get_r(),data->db_get_g(),data->db_get_b());
  DiagramItem * item = new DiagramItem(addType, 0, 0, color);
  item->setPos(QPointF(data->db_get_posX(), data->db_get_posY()));
  item->setId(data->db_get_itemId());

  scene->addDiagramItem(item);
  scene->clearSelection();
  scene->update();
}

MoveAction::MoveAction() : DiagramAction() 
{ 
  std::string type = "com.vistrails.provexp.example.moveaction"; 
  this->data = new uf::DBMoveAction();
  
  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

MoveAction::MoveAction(DiagramItem * movedItem, const QPointF & position) : \
  DiagramAction()
{
  std::string type = "com.vistrails.provexp.example.moveaction";
  data = new uf::DBMoveAction();

  data->db_set_itemId(movedItem->id());
  data->db_set_newPosX(position.x());
  data->db_set_newPosY(position.y());

  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

void MoveAction::execute(DiagramScene *scene)
{
  DiagramItem * item = scene->getDiagramItem(data->db_get_itemId());
  if (item != nullptr) {
    item->setPos(QPointF(data->db_get_newPosX(), data->db_get_newPosY()));
    scene->update();
  }
}

DeleteAction::DeleteAction() : DiagramAction() 
{ 
  std::string type = "com.vistrails.provexp.example.deleteaction";
  this->data = new uf::DBDeleteAction();
  
  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

DeleteAction::DeleteAction(int itemId) : DiagramAction()
{
  std::string type = "com.vistrails.provexp.example.deleteaction";
  data = new uf::DBDeleteAction();

  data->db_set_itemId(itemId);

  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

void DeleteAction::execute(DiagramScene *scene)
{
  scene->removeDiagramItem(data->db_get_itemId());
}

SceneAction::SceneAction() : DiagramAction()
{
  std::string type = "com.vistrails.provexp.example.sceneaction";
  this->data = new uf::DBSceneAction();

  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

SceneAction::SceneAction(DiagramScene * scene) : DiagramAction()
{
  std::string type = "com.vistrails.provexp.example.sceneaction";
  this->data = new uf::DBSceneAction();

  QList<QGraphicsItem *> items = scene->items();
  for(QList<QGraphicsItem *>::const_iterator it=items.begin(); 
      it != items.end(); ++it) {  
    int r, g, b;
    DiagramItem * item = static_cast<DiagramItem *>(*it);
    item->brush().color().getRgb(&r, &g, &b);
    uf::DBShape * shape = new uf::DBShape();
    shape->init(item->id(),
		(int) item->diagramType(),
		(double) item->pos().x(),
		(double) item->pos().y(),
		r, g, b);
    data->db_add_shape(shape);
  }
  
  // std::vector<vt::DBStream *> streams;
  // this->init("", type, "1.0", "normal", streams);
}

void SceneAction::execute(DiagramScene * scene)
{
  for(uf::DBSceneAction_shape_container::const_iterator it =
	data->db_get_shapes().begin(); it != data->db_get_shapes().end(); ++it)
    {
      uf::DBShape * shape = *it;
      QColor color = QColor(shape->db_get_r(), shape->db_get_g(), 
			    shape->db_get_b());
      DiagramItem * item = 
	new DiagramItem((DiagramItem::DiagramType) shape->db_get_itemType(), 
			0, 0, color);
      item->setId(shape->db_get_itemId());
      item->setPos(QPointF(shape->db_get_posX(), shape->db_get_posY()));
      scene->addDiagramItem(item);
    }
  scene->clearSelection();
  scene->update();
}
