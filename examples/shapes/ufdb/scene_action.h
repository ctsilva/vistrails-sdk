/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

//generated automatically

#ifndef UF_DB_SCENE_ACTION
#define UF_DB_SCENE_ACTION

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

#include "shape.h"

namespace uf {

struct shape_itemId_index;

typedef boost::multi_index_container<
  DBShape*,
  boost::multi_index::indexed_by<
    boost::multi_index::hashed_unique< 
      boost::multi_index::tag< shape_itemId_index >,
      boost::multi_index::const_mem_fun< 
	DBShape,
	int,
	&DBShape::db_get_itemId
      >
    >
  >
> DBSceneAction_shape_container;

typedef DBSceneAction_shape_container::index< shape_itemId_index >::type DBSceneAction_shape_itemId_index_t;
typedef DBSceneAction_shape_itemId_index_t::iterator DBSceneAction_shape_itemId_index_it;


class DBSceneAction : public DBDomainObj
{
  UF_DB_DOMAIN_PROXY_DEF(DBSceneAction)
public:
  DBSceneAction();
  virtual void init(std::vector<DBShape *> shapes);
  // DBSceneAction(const DBSceneAction & copy);
  // DBSceneAction & do_copy(bool new_ids=false, HASH id_scope=NULL, // 				  HASH id_remap=NULL);
  // friend DBSceneAction & update_version(const DBSceneAction & // 						old_obj, HASH trans_dict, // 						DBSceneAction & // 						new_obj=NULL);
  // LIST db_children(std:string parent_type="", 
  // 			    long parent_id,
  // 			    bool orphan=False);
  // LIST db_deleted_children(bool remove=false);
  // bool has_changes();
  
  const DBSceneAction_shape_container & db_get_shapes() const;
  void db_set_shapes(std::vector<DBShape *> & shapes);
  void db_add_shape(DBShape * shape);
  void db_change_shape(DBShape * shape);
  void db_delete_shape(DBShape * shape);
  DBShape * db_get_shape_by_itemId(int key);
  bool db_has_shape_with_itemId(int key);


  bool is_dirty;
  bool is_new;
protected:
  DBSceneAction_shape_container shapes;
  //bool shape_valid;
  std::vector<DBShape *> db_deleted_shapes;
};

}

#endif
