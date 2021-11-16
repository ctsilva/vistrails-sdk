/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#ifndef UF_DB_MOVE_ACTION
#define UF_DB_MOVE_ACTION

#if !defined(NDEBUG)
#define BOOST_MULTI_INDEX_ENABLE_INVARIANT_CHECKING
#define BOOST_MULTI_INDEX_ENABLE_SAFE_MODE
#endif

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>
#include <boost/multi_index/ordered_index.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/tuple/tuple.hpp>

#include <boost/uuid/uuid.hpp>

#include <string>

#include "domainfactory.h"


namespace uf {



class DBMoveAction : public DBDomainObj
{
  UF_DB_DOMAIN_PROXY_DEF(DBMoveAction)
public:
  DBMoveAction();
  virtual void init(int itemId, double oldPosX, double oldPosY, double newPosX, double newPosY);
  // DBMoveAction(const DBMoveAction & copy);
  // DBMoveAction & do_copy(bool new_ids=false, HASH id_scope=NULL, // 				  HASH id_remap=NULL);
  // friend DBMoveAction & update_version(const DBMoveAction & // 						old_obj, HASH trans_dict, // 						DBMoveAction & // 						new_obj=NULL);
  // LIST db_children(std:string parent_type="", 
  // 			    long parent_id,
  // 			    bool orphan=False);
  // LIST db_deleted_children(bool remove=false);
  // bool has_changes();
  
  int db_get_itemId() const;
  void db_set_itemId(int itemId);
  // property?
  void db_add_itemId(int itemId);
  void db_change_itemId(int itemId);
  void db_delete_itemId(int itemId);

  double db_get_oldPosX() const;
  void db_set_oldPosX(double oldPosX);
  // property?
  void db_add_oldPosX(double oldPosX);
  void db_change_oldPosX(double oldPosX);
  void db_delete_oldPosX(double oldPosX);

  double db_get_oldPosY() const;
  void db_set_oldPosY(double oldPosY);
  // property?
  void db_add_oldPosY(double oldPosY);
  void db_change_oldPosY(double oldPosY);
  void db_delete_oldPosY(double oldPosY);

  double db_get_newPosX() const;
  void db_set_newPosX(double newPosX);
  // property?
  void db_add_newPosX(double newPosX);
  void db_change_newPosX(double newPosX);
  void db_delete_newPosX(double newPosX);

  double db_get_newPosY() const;
  void db_set_newPosY(double newPosY);
  // property?
  void db_add_newPosY(double newPosY);
  void db_change_newPosY(double newPosY);
  void db_delete_newPosY(double newPosY);


  bool is_dirty;
  bool is_new;
protected:
  int itemId;
  //bool itemId_valid;
  double oldPosX;
  //bool oldPosX_valid;
  double oldPosY;
  //bool oldPosY_valid;
  double newPosX;
  //bool newPosX_valid;
  double newPosY;
  //bool newPosY_valid;
};

}

#endif
