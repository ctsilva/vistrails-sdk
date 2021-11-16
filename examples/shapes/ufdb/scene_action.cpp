/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#include "scene_action.h"

namespace uf {

DBSceneAction::DBSceneAction() {
  std::vector<DBShape *> shapes;
  this->init(shapes);
}

void DBSceneAction::init(std::vector<DBShape *> shapes)
{
  this->domainType = "scene_action";
  db_deleted_shapes = std::vector<DBShape *>();
  //this->shapes = DBSceneAction_shape_container();
  for (std::vector<DBShape *>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
    this->shapes.insert(*it);
  }

  this->is_dirty = true;
  this->is_new = true;
}

/*
DBSceneAction::DBSceneAction(const DBSceneAction & copy)
{

}

DBSceneAction & DBSceneAction::do_copy(bool new_ids=false, HASH id_scope=NULL, HASH id_remap=NULL)
{

}

friend DBSceneAction & update_version(const DBSceneAction & old_obj, HASH trans_dict, DBSceneAction & new_obj=NULL)
{

}

LIST DBSceneAction::db_children(std:string parent_type="", int parent_id=-1L, bool orphan=False)
{

}

LIST DBSceneAction::db_deleted_children(bool remove=false)
{

}

bool DBSceneAction::has_changes()
{

}
*/

const DBSceneAction_shape_container & DBSceneAction::db_get_shapes() const
{
  return shapes;
}

 void DBSceneAction::db_set_shapes(std::vector<DBShape *> & shapes)
{
  this->shapes.clear();
  for (std::vector<DBShape *>::iterator it = shapes.begin(); it != shapes.end(); ++it) {
    this->shapes.insert(*it);
  }
}

void DBSceneAction::db_add_shape(DBShape * shape)
{
  this->shapes.insert(shape);
}

void DBSceneAction::db_change_shape(DBShape * shape)
{
  //this->shapes.replace(), modify()?;
  //this->shapes.insert(shape);
}

void DBSceneAction::db_delete_shape(DBShape * shape)
{
}

/*
const DBSceneAction_shape_container & DBSceneAction::db_get_shapes()
{
  return this->shapes;
}
*/


DBShape * DBSceneAction::db_get_shape_by_itemId(int key)
{
  DBSceneAction_shape_itemId_index_it it = shapes.get< shape_itemId_index >().find(key);
  return *it;
}

bool DBSceneAction::db_has_shape_with_itemId(int key)
{
  DBSceneAction_shape_itemId_index_it it = shapes.get< shape_itemId_index >().find(key);
  return (it != shapes.get< shape_itemId_index >().end());
}



}
