/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "core/global.h"
#include "core/Action.h"
#include "core/ActionFactory.h"
#include "core/Version.h"
#include "core/Session.h"
#include "core/SessionFocus.h"
#include "core/Storage.h"
#include "core/VersionTree.h"
#include "core/Vistrail.h"
#include "core/XmlStorage.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>

VT_USING_NAMESPACE

class OvalAction : public Action
{
  VT_DECLARE_ACTION(OvalAction, "Oval");
public:
  OvalAction() : Action() { this->radius = 0.0; }
  OvalAction(float radius) : Action() { this->radius = radius; }

  virtual void load(vt::PList *plist)
  {
    this->radius = plist->get(0)->asFloat();
  }

  virtual void save(vt::PList *plist) const
  {
    plist->addFloat(radius);
  }
  
  float radius;
};

class RectAction : public Action
{
  VT_DECLARE_ACTION(RectAction, "Rectangle");
public:
  RectAction() : Action() { this->width = 0.0; this->height = 0.0; }
  RectAction(float width, float height) : Action() { this->width = width; this->height = height; }
  
  virtual void load(vt::PList *plist)
  {
    PDict *dict = plist->get(0)->asDict();
    this->width = dict->get("width")->asFloat();
    this->height = dict->get("height")->asFloat();
  }
  
  virtual void save(vt::PList *plist) const
  {
    PDict *dict = plist->addDict();
    dict->setFloat("width", this->width);
    dict->setFloat("height", this->height);
  }

  float width, height;
};

class TestAction : public Action
{
  VT_DECLARE_ACTION(TestAction, "TestAction");
public:
  TestAction() : Action() 
  {
    this->bool_test = true;
    this->date_test = time(NULL);
    this->double_test = 0.3214;
    this->float_test = 56.123;
    this->int_test = 6516;
    this->int64_test = 654613231;
    this->str_test = "a;lskdjf";
  }
  
  virtual void save(vt::PList *plist) const
  {
    plist->addBool(this->bool_test);
    plist->addDateTime(this->date_test);
    plist->addDouble(this->double_test);
    plist->addFloat(this->float_test);
    plist->addInt(this->int_test);
    plist->addInt64(this->int64_test);
    plist->addString(this->str_test);

    PDict* dict_test = plist->addDict();
    dict_test->setBool("bool", this->bool_test);

    PList* list_test = plist->addList();
    list_test->addBool(this->bool_test);
  }
  
  virtual void load(vt::PList *plist)
  {
    this->bool_test = plist->get(0)->asBool();
    this->date_test = plist->get(1)->asDateTime()->toTime();
    this->double_test = plist->get(2)->asDouble();
    this->float_test = plist->get(3)->asFloat();
    this->int_test = plist->get(4)->asInt();
    this->int64_test = plist->get(5)->asInt64();
    this->str_test = plist->get(6)->asChar();

    PDict* dict_test = plist->get(7)->asDict();
    this->bool_test = dict_test->get("bool");

    PList* list_test = plist->get(8)->asList();
    this->bool_test = list_test->get(0)->asBool();
  }

  bool bool_test;
  int32_t int_test;
  int64_t int64_test;
  float float_test;
  double double_test;
  const char* str_test;
  time_t date_test;
};

void walk_tree(const VersionId &parent, Vistrail *vistrail)
{
  VersionTree* tree = vistrail->versionTree();

  VersionId child = tree->getFirstChild(parent);
  while(child != VERSION_INVALID)
  {
    std::cout << child.toString().asChar() << std::endl;
    walk_tree(child, vistrail);
    child = tree->getNextSibling(child);
  }
}

int main(int argc, char **argv)
{  
  ActionFactory *factory = ActionFactory::globalFactory();  

  factory->registerAction(RectAction::creator());
  factory->registerAction(OvalAction::creator());
  factory->registerAction(TestAction::creator());
  Session session;
  SessionFocus focus(&session);
  XmlStorage * in_storage = new XmlStorage("test_vt.xml");
  session.load(in_storage);
  delete in_storage;
  
  // walk the loaded tree
  walk_tree(VERSION_ROOT, session.vistrail());

  Version *version = new Version();
  ActionList * performedActions = version->performed();
  performedActions->add(new OvalAction(5.0));
  performedActions->add(new RectAction(3.0, 4.0));
  performedActions->add(new TestAction());
  focus.record(version);

  Version *version2 = new Version();
  performedActions = version2->performed();
  performedActions->add(new OvalAction(8.0));
  focus.record(version2);
  
  XmlStorage out_storage("test_vt_out.xml");
  session.save(&out_storage);

  return 0;
}
