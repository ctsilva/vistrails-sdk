/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#include "delete_action.h"

namespace uf {

DBDeleteAction::DBDeleteAction() {
  this->init(-1);
}

void DBDeleteAction::init(int itemId)
{
  this->domainType = "delete_action";
  this->itemId = itemId;

  this->is_dirty = true;
  this->is_new = true;
}

/*
DBDeleteAction::DBDeleteAction(const DBDeleteAction & copy)
{

}

DBDeleteAction & DBDeleteAction::do_copy(bool new_ids=false, HASH id_scope=NULL, HASH id_remap=NULL)
{

}

friend DBDeleteAction & update_version(const DBDeleteAction & old_obj, HASH trans_dict, DBDeleteAction & new_obj=NULL)
{

}

LIST DBDeleteAction::db_children(std:string parent_type="", int parent_id=-1L, bool orphan=False)
{

}

LIST DBDeleteAction::db_deleted_children(bool remove=false)
{

}

bool DBDeleteAction::has_changes()
{

}
*/

int DBDeleteAction::db_get_itemId () const
{
  return itemId;
}

void DBDeleteAction::db_set_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBDeleteAction::db_add_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBDeleteAction::db_change_itemId(int itemId)
{
  this->itemId = itemId;
}

void DBDeleteAction::db_delete_itemId(int itemId)
{
  //del this->itemId;
}




}
