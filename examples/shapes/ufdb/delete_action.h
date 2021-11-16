/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#ifndef UF_DB_DELETE_ACTION
#define UF_DB_DELETE_ACTION

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



class DBDeleteAction : public DBDomainObj
{
  UF_DB_DOMAIN_PROXY_DEF(DBDeleteAction)
public:
  DBDeleteAction();
  virtual void init(int itemId);
  // DBDeleteAction(const DBDeleteAction & copy);
  // DBDeleteAction & do_copy(bool new_ids=false, HASH id_scope=NULL, // 				  HASH id_remap=NULL);
  // friend DBDeleteAction & update_version(const DBDeleteAction & // 						old_obj, HASH trans_dict, // 						DBDeleteAction & // 						new_obj=NULL);
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


  bool is_dirty;
  bool is_new;
protected:
  int itemId;
  //bool itemId_valid;
};

}

#endif
