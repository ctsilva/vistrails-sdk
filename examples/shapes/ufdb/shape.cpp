/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#include "shape.h"

namespace uf {

DBShape::DBShape() {
  this->init(-1, -1, std::numeric_limits<double>::signaling_NaN(), std::numeric_limits<double>::signaling_NaN(), -1, -1, -1);
}

void DBShape::init(int itemId, int itemType, double posX, double posY, int r, int g, int b)
{
  this->domainType = "shape";
  this->itemId = itemId;
  this->itemType = itemType;
  this->posX = posX;
  this->posY = posY;
  this->r = r;
  this->g = g;
  this->b = b;

  this->is_dirty = true;
  this->is_new = true;
}

/*
DBShape::DBShape(const DBShape & copy)
{

}

DBShape & DBShape::do_copy(bool new_ids=false, HASH id_scope=NULL, HASH id_remap=NULL)
{

}

friend DBShape & update_version(const DBShape & old_obj, HASH trans_dict, DBShape & new_obj=NULL)
{

}

LIST DBShape::db_children(std:string parent_type="", int parent_id=-1L, bool orphan=False)
{

}

LIST DBShape::db_deleted_children(bool remove=false)
{

}

bool DBShape::has_changes()
{

}
*/

int DBShape::db_get_itemId () const
{
  return itemId;
}

void DBShape::db_set_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBShape::db_add_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBShape::db_change_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBShape::db_delete_itemId(int itemId)
{
  //del this->itemId;
}



int DBShape::db_get_itemType () const
{
  return itemType;
}

void DBShape::db_set_itemType(int itemType)
{
  this->itemType = itemType;
}

void DBShape::db_add_itemType(int itemType)
{
  this->itemType = itemType;
}

void DBShape::db_change_itemType(int itemType)
{
  this->itemType = itemType;
}

void DBShape::db_delete_itemType(int itemType)
{
  //del this->itemType;
}



double DBShape::db_get_posX () const
{
  return posX;
}

void DBShape::db_set_posX(double posX)
{
  this->posX = posX;
}

void DBShape::db_add_posX(double posX)
{
  this->posX = posX;
}

void DBShape::db_change_posX(double posX)
{
  this->posX = posX;
}

void DBShape::db_delete_posX(double posX)
{
  //del this->posX;
}



double DBShape::db_get_posY () const
{
  return posY;
}

void DBShape::db_set_posY(double posY)
{
  this->posY = posY;
}

void DBShape::db_add_posY(double posY)
{
  this->posY = posY;
}

void DBShape::db_change_posY(double posY)
{
  this->posY = posY;
}

void DBShape::db_delete_posY(double posY)
{
  //del this->posY;
}



int DBShape::db_get_r () const
{
  return r;
}

void DBShape::db_set_r(int r)
{
  this->r = r;
}

void DBShape::db_add_r(int r)
{
  this->r = r;
}

void DBShape::db_change_r(int r)
{
  this->r = r;
}

void DBShape::db_delete_r(int r)
{
  //del this->r;
}



int DBShape::db_get_g () const
{
  return g;
}

void DBShape::db_set_g(int g)
{
  this->g = g;
}

void DBShape::db_add_g(int g)
{
  this->g = g;
}

void DBShape::db_change_g(int g)
{
  this->g = g;
}

void DBShape::db_delete_g(int g)
{
  //del this->g;
}



int DBShape::db_get_b () const
{
  return b;
}

void DBShape::db_set_b(int b)
{
  this->b = b;
}

void DBShape::db_add_b(int b)
{
  this->b = b;
}

void DBShape::db_change_b(int b)
{
  this->b = b;
}

void DBShape::db_delete_b(int b)
{
  //del this->b;
}



int DBShape::getPrimaryKey()
{

}

}
