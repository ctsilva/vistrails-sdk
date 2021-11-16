/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#ifndef UF_DB_ADD_ACTION
#define UF_DB_ADD_ACTION

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



class DBAddAction : public DBDomainObj
{
  UF_DB_DOMAIN_PROXY_DEF(DBAddAction)
public:
  DBAddAction();
  virtual void init(int itemId, int itemType, double posX, double posY, int r, int g, int b);
  // DBAddAction(const DBAddAction & copy);
  // DBAddAction & do_copy(bool new_ids=false, HASH id_scope=NULL, // 				  HASH id_remap=NULL);
  // friend DBAddAction & update_version(const DBAddAction & // 						old_obj, HASH trans_dict, // 						DBAddAction & // 						new_obj=NULL);
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

  int db_get_itemType() const;
  void db_set_itemType(int itemType);
  // property?
  void db_add_itemType(int itemType);
  void db_change_itemType(int itemType);
  void db_delete_itemType(int itemType);

  double db_get_posX() const;
  void db_set_posX(double posX);
  // property?
  void db_add_posX(double posX);
  void db_change_posX(double posX);
  void db_delete_posX(double posX);

  double db_get_posY() const;
  void db_set_posY(double posY);
  // property?
  void db_add_posY(double posY);
  void db_change_posY(double posY);
  void db_delete_posY(double posY);

  int db_get_r() const;
  void db_set_r(int r);
  // property?
  void db_add_r(int r);
  void db_change_r(int r);
  void db_delete_r(int r);

  int db_get_g() const;
  void db_set_g(int g);
  // property?
  void db_add_g(int g);
  void db_change_g(int g);
  void db_delete_g(int g);

  int db_get_b() const;
  void db_set_b(int b);
  // property?
  void db_add_b(int b);
  void db_change_b(int b);
  void db_delete_b(int b);


  bool is_dirty;
  bool is_new;
protected:
  int itemId;
  //bool itemId_valid;
  int itemType;
  //bool itemType_valid;
  double posX;
  //bool posX_valid;
  double posY;
  //bool posY_valid;
  int r;
  //bool r_valid;
  int g;
  //bool g_valid;
  int b;
  //bool b_valid;
};

}

#endif
