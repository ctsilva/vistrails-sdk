/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#include "move_action.h"

namespace uf {

DBMoveAction::DBMoveAction() {
  this->init(-1, std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN());
}

void DBMoveAction::init(int itemId, double oldPosX, double oldPosY, double newPosX, double newPosY)
{
  this->domainType = "move_action";
  this->itemId = itemId;
  this->oldPosX = oldPosX;
  this->oldPosY = oldPosY;
  this->newPosX = newPosX;
  this->newPosY = newPosY;

  this->is_dirty = true;
  this->is_new = true;
}

/*
DBMoveAction::DBMoveAction(const DBMoveAction & copy)
{

}

DBMoveAction & DBMoveAction::do_copy(bool new_ids=false, HASH id_scope=NULL, HASH id_remap=NULL)
{

}

friend DBMoveAction & update_version(const DBMoveAction & old_obj, HASH trans_dict, DBMoveAction & new_obj=NULL)
{

}

LIST DBMoveAction::db_children(std:string parent_type="", int parent_id=-1L, bool orphan=False)
{

}

LIST DBMoveAction::db_deleted_children(bool remove=false)
{

}

bool DBMoveAction::has_changes()
{

}
*/

int DBMoveAction::db_get_itemId () const
{
  return itemId;
}

void DBMoveAction::db_set_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBMoveAction::db_add_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBMoveAction::db_change_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBMoveAction::db_delete_itemId(int itemId)
{
  //del this->itemId;
}



double DBMoveAction::db_get_oldPosX () const
{
  return oldPosX;
}

void DBMoveAction::db_set_oldPosX(double oldPosX)
{
  this->oldPosX = oldPosX;
}

void DBMoveAction::db_add_oldPosX(double oldPosX)
{
  this->oldPosX = oldPosX;
}

void DBMoveAction::db_change_oldPosX(double oldPosX)
{
  this->oldPosX = oldPosX;
}

void DBMoveAction::db_delete_oldPosX(double oldPosX)
{
  //del this->oldPosX;
}



double DBMoveAction::db_get_oldPosY () const
{
  return oldPosY;
}

void DBMoveAction::db_set_oldPosY(double oldPosY)
{
  this->oldPosY = oldPosY;
}

void DBMoveAction::db_add_oldPosY(double oldPosY)
{
  this->oldPosY = oldPosY;
}

void DBMoveAction::db_change_oldPosY(double oldPosY)
{
  this->oldPosY = oldPosY;
}

void DBMoveAction::db_delete_oldPosY(double oldPosY)
{
  //del this->oldPosY;
}



double DBMoveAction::db_get_newPosX () const
{
  return newPosX;
}

void DBMoveAction::db_set_newPosX(double newPosX)
{
  this->newPosX = newPosX;
}

void DBMoveAction::db_add_newPosX(double newPosX)
{
  this->newPosX = newPosX;
}

void DBMoveAction::db_change_newPosX(double newPosX)
{
  this->newPosX = newPosX;
}

void DBMoveAction::db_delete_newPosX(double newPosX)
{
  //del this->newPosX;
}



double DBMoveAction::db_get_newPosY () const
{
  return newPosY;
}

void DBMoveAction::db_set_newPosY(double newPosY)
{
  this->newPosY = newPosY;
}

void DBMoveAction::db_add_newPosY(double newPosY)
{
  this->newPosY = newPosY;
}

void DBMoveAction::db_change_newPosY(double newPosY)
{
  this->newPosY = newPosY;
}

void DBMoveAction::db_delete_newPosY(double newPosY)
{
  //del this->newPosY;
}




}
