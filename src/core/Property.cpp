/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "Property.h"
#include "global_private.h"
#include "Handler.h"
#include <string>
#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/tokenizer.hpp>
#include <vector>

VT_BEGIN_NAMESPACE

Property * Property::makeCopy() const
{
  return new Property();
}

PString Property::toString() const
{
  return PString("NULL");
}

void Property::fromString(const PString &pstr)
{
  // doing nothing
}

#define DEFINE_PRIMITIVE_PROPERTY(PTYPE, CTYPE, DEFAULT)                \
  class PTYPE::PrivateImpl { public: CTYPE value; };                    \
  VT_DEFINE_PIMPL(PTYPE);                                               \
  PTYPE::PTYPE(): self(new PrivateImpl)                                 \
  { this->self->value = DEFAULT; }                                      \
  PTYPE::PTYPE(CTYPE value): self(new PrivateImpl)                      \
  { this->self->value = value; }                                        \
  CTYPE PTYPE::value() const { return this->self->value; }              \
  void PTYPE::setValue(CTYPE value) { this->self->value = value; }      \
  Property * PTYPE::makeCopy() const                                    \
  {                                                                     \
    return new PTYPE(this->self->value);                                \
  }                                                                     \
  PString PTYPE::toString() const                                       \
  {                                                                     \
    return boost::lexical_cast<std::string>(this->self->value).c_str(); \
  }                                                                     \
  void PTYPE::fromString(const PString &pstr)                           \
  {                                                                     \
    this->self->value = boost::lexical_cast<CTYPE>(pstr.asChar());      \
  }

DEFINE_PRIMITIVE_PROPERTY(PBool, bool, false);
DEFINE_PRIMITIVE_PROPERTY(PInt, int32_t, 0);
DEFINE_PRIMITIVE_PROPERTY(PInt64, int64_t, 0);
DEFINE_PRIMITIVE_PROPERTY(PFloat, float, 0.);
DEFINE_PRIMITIVE_PROPERTY(PDouble, double, 0.);

class PString::PrivateImpl
{
public:
  std::string value;
};

VT_DEFINE_PIMPL(PString);

PString::PString(): self(new PrivateImpl) {}

PString::PString(const char *str): self(new PrivateImpl)
{
  this->setValue(str);
}

PString::PString(const char *str, size_t n): self(new PrivateImpl)
{
  this->setValue(str, n);
}

PString PString::toString() const
{
  return *this;
}

void PString::setValue(const char *str)
{
  this->self->value.assign(str);
}

void PString::setValue(const char *str, size_t n)
{
  this->self->value.assign(str, n);
}

const char * PString::asChar() const
{
  return this->self->value.c_str();
}

size_t PString::length() const
{
  return this->self->value.size();
}

int PString::compare(const char *str) const
{
  return this->self->value.compare(str);
}

int PString::compare(const char *str, size_t n) const
{
  return this->self->value.compare(0, this->self->value.size(), str, n);
}

Property * PString::makeCopy() const
{
  return new PString(this->self->value.c_str());
}

class DataFetcher::PrivateImpl
{
public:
  HFetchData * fetcher;
  void       * dataId;
  size_t       size;
};

VT_DEFINE_PIMPL(DataFetcher);

DataFetcher::DataFetcher(): self(new PrivateImpl)
{
  this->self->fetcher = 0;
  this->self->dataId = 0;
  this->self->size = 0;
}

void DataFetcher::fetch(void *data) const
{
  if (this->self->fetcher)
    this->self->fetcher->fetch(this->self->dataId, data);
}

size_t DataFetcher::size() const
{
  return this->self->size;
}

void * DataFetcher::dataId() const
{
  return this->self->dataId;
}

HFetchData * DataFetcher::handler() const
{
  return this->self->fetcher;
}

void DataFetcher::setSize(size_t size)
{
  this->self->size = size;
}

void DataFetcher::setDataId(void * dataId)
{
  this->self->dataId = dataId;
}

void DataFetcher::setHandler(HFetchData *handler)
{
  this->self->fetcher = handler;
}

class PData::PrivateImpl
{
public:
  PrivateImpl(): onDemand(false), data(NULL), size(0) {}
  bool        onDemand;
  void *      data;
  size_t      size;
  DataFetcher fetcher;
};

VT_DEFINE_PIMPL_COPY(PData);

PData::PData(): self(new PrivateImpl)
{
}

PData::PData(size_t size): self(new PrivateImpl)
{
  this->createData(size);
}

PData::PData(void *data, size_t size, bool passOwnership)
  : self(new PrivateImpl)
{
  this->setData(data, size, passOwnership);
}

void PData::createData(size_t size)
{
  if (size>0) {
    this->self->data = malloc(size);
    this->self->size = this->self->data?size:0;
  }
}

PData::~PData()
{
  this->setData(0, 0, 0);
  delete this->self;
}

void PData::setData(void *data, size_t size, bool passOwnership)
{
  if (this->self->data) {
    free(this->self->data);
    this->removeOwnership();
  } 
  if (passOwnership) {
    this->self->data = data;
    this->self->size = size;
  }
  else {
    this->createData(size);
    if (size>0)
      memcpy(this->self->data, data, size);
  }
}

bool PData::onDemand() const
{
  return this->self->onDemand;
}

void PData::setOnDemand(bool hint)
{
  this->self->onDemand = hint;
}

DataFetcher PData::dataFetcher() const
{
  return this->self->fetcher;
}

void PData::setDataFetcher(DataFetcher fetcher)
{
  this->self->fetcher = fetcher;
}

void * PData::data()
{
  return this->self->data;
}

size_t PData::size() const
{
  return this->self->size;
}

void PData::removeOwnership()
{
  this->self->data = NULL;
  this->self->size = 0;
}

void * PData::takeData()
{
  void * data = this->self->data;
  this->removeOwnership();
  return data;
}

Property * PData::makeCopy() const
{
  PData *copy = new PData(this->self->data, this->self->size, false);
  copy->self->onDemand = this->self->onDemand;
  copy->self->fetcher = this->self->fetcher;
  return copy;
}

class PDateTime::PrivateImpl
{
public:
  union {
    struct {
      uint8_t  second;
      uint8_t  minute;
      uint8_t  hour;
      uint8_t  day;
      uint8_t  month;
      int16_t  year;
      int8_t   dst;
    } fields;
    int64_t blob;
  } value;
};

VT_DEFINE_PIMPL(PDateTime);

PDateTime::PDateTime(): self(new PrivateImpl)
{
}

PDateTime::PDateTime(time_t t): self(new PrivateImpl)
{
  this->fromTime(t);
}

time_t PDateTime::toTime() const
{
  struct tm info;
  this->toTimeStruct(&info);
  return mktime(&info);
}

void PDateTime::fromTime(time_t t)
{
  struct tm info = boost::posix_time::to_tm(boost::posix_time::from_time_t(t));
  this->fromTimeStruct(&info);
}


void PDateTime::toTimeStruct(struct tm *info) const
{
  info->tm_sec   = this->self->value.fields.second;
  info->tm_min   = this->self->value.fields.minute;
  info->tm_hour  = this->self->value.fields.hour;
  info->tm_mday  = this->self->value.fields.day;
  info->tm_mon   = this->self->value.fields.month-1;
  info->tm_year  = this->self->value.fields.year-1900;
  info->tm_isdst = this->self->value.fields.dst;
}
  
void PDateTime::fromTimeStruct(struct tm *info)
{
  this->self->value.fields.second = info->tm_sec;
  this->self->value.fields.minute = info->tm_min;
  this->self->value.fields.hour   = info->tm_hour;
  this->self->value.fields.day    = info->tm_mday;
  this->self->value.fields.month  = info->tm_mon+1;
  this->self->value.fields.year   = info->tm_year+1900;
  this->self->value.fields.dst    = info->tm_isdst;
}

int64_t PDateTime::toInt64() const
{
  return this->self->value.blob;
}

void PDateTime::fromInt64(int64_t t)
{
  this->self->value.blob = t;
}

Property * PDateTime::makeCopy() const
{
  return new PDateTime(*this);
}

#define DEFINE_PDATETIME_ACCESS(GET,SET,FIELD)              \
  int PDateTime::GET() const { return this->self->value.fields.FIELD; }  \
  void PDateTime::SET(int v) { this->self->value.fields.FIELD = v; }
DEFINE_PDATETIME_ACCESS(day, setDay, day);
DEFINE_PDATETIME_ACCESS(month, setMonth, month);
DEFINE_PDATETIME_ACCESS(year, setYear, year);
DEFINE_PDATETIME_ACCESS(hour, setHour, hour);
DEFINE_PDATETIME_ACCESS(minute, setMinute, minute);
DEFINE_PDATETIME_ACCESS(second, setSecond, second);

PString PDateTime::toString() const
{
  struct tm info;
  this->toTimeStruct(&info);
  std::string str;
  str.reserve(20);
  strftime((char*)str.data(), str.capacity(), "%Y-%m-%d %H:%M:%S", &info);
  return PString(str.data());
}

void PDateTime::fromString(const PString &pstr)
{
  typedef boost::char_separator<char> Separator;
  typedef boost::tokenizer<Separator> Tokenizer;
  Separator sep("- :");
  std::string s(pstr.asChar());
  Tokenizer tokens(s, sep);
  Tokenizer::iterator it = tokens.begin();
  this->setYear  (atoi((*it++).c_str()));
  this->setMonth (atoi((*it++).c_str()));
  this->setDay   (atoi((*it++).c_str()));
  this->setHour  (atoi((*it++).c_str()));
  this->setMinute(atoi((*it++).c_str()));
  this->setSecond(atoi((*it++).c_str()));
}

typedef boost::unordered_map<std::string, Property *> PropertyMap; 

class PDict::PrivateImpl
{
public:
  PropertyMap dict;
};

VT_DEFINE_PIMPL_COPY(PDict);

PDict::PDict(): self(new PrivateImpl)
{
}

PDict::~PDict()
{
  this->clear();
  delete this->self;
}

#define PDICT_SET(NAME, CTYPE)                                \
  P##NAME * PDict::set##NAME(const char *key, CTYPE value)    \
  {                                                           \
    P##NAME *prop = new P##NAME(value);                       \
    this->set(key, prop);                                     \
    return prop;                                              \
  }
PDICT_SET(Bool, bool);
PDICT_SET(Int, int32_t);
PDICT_SET(Int64, int64_t);
PDICT_SET(Float, float);
PDICT_SET(Double, double);
PDICT_SET(DateTime, time_t);
PDICT_SET(String, const char *);

PData * PDict::setData(const char *key, void *data, size_t size, bool passOwnership)
{
  PData *prop = new PData(data, size, passOwnership);
  this->set(key, prop);
  return prop;
}

PDict * PDict::setDict(const char *key)
{
  PDict *prop = new PDict();
  this->set(key, prop);
  return prop;
}

PList * PDict::setList(const char *key)
{
  PList *prop = new PList();
  this->set(key, prop);
  return prop;
}

void PDict::set(const char *key, Property *prop)
{
  this->self->dict[key] = prop;
}

void PDict::merge(PDict *dict)
{
  PropertyMap::iterator it = dict->self->dict.begin();
  while (it!=dict->self->dict.end()) {
    this->set((*it).first.c_str(),  (*it).second->makeCopy());
    it++;
  }
}

void PDict::remove(const char *key)
{
  PropertyMap::iterator it = this->self->dict.find(key);
  if (it!=this->self->dict.end())
    this->self->dict.erase(it);
}

Property * PDict::get(const char *key) const
{
  PropertyMap::iterator it = this->self->dict.find(key);
  if (it!=this->self->dict.end())
    return (*it).second;
  return NULL;
}

size_t PDict::count() const
{
  return this->self->dict.size();
}

void PDict::clear()
{
  PropertyMap::iterator it = this->self->dict.begin();
  while (it!=this->self->dict.end()) {
    delete (*it).second;
    it++;
  }
  this->self->dict.clear();
}

Property * PDict::makeCopy() const
{
  PDict *dict = new PDict();
  PropertyMap::iterator it = this->self->dict.begin();
  while (it!=this->self->dict.end()) {
    dict->set((*it).first.c_str(), (*it).second);
    it++;
  }
  return dict;
}

class PDict::Iterator::PrivateImpl
{
public:
  PropertyMap::iterator it;
  PDict *               dict;
};

PDict::Iterator::~Iterator() { delete this->self; }

PDict::Iterator::Iterator(): self(new PrivateImpl)
{
}

void PDict::Iterator::init(PDict *dict)
{
  this->self->dict = dict;
  this->restart();
}


void PDict::Iterator::restart()
{
  this->self->it = this->self->dict->self->dict.begin();
}

void PDict::Iterator::next()
{
  this->self->it++;
}

bool PDict::Iterator::hasNext()
{
  return this->self->it!=this->self->dict->self->dict.end();
}

const char * PDict::Iterator::key()
{
  return (*this->self->it).first.c_str();
}

Property * PDict::Iterator::property()
{
  return (*this->self->it).second;
}

typedef std::vector<Property *> PropertyVector; 

class PList::PrivateImpl
{
public:
  PropertyVector list;
};

VT_DEFINE_PIMPL_COPY(PList);

PList::PList(): self(new PrivateImpl)
{
}

PList::~PList()
{
  this->clear();
  delete this->self;
}

#define PLIST_ADD(NAME, CTYPE)                                \
  P##NAME * PList::add##NAME(CTYPE value)                     \
  {                                                           \
    P##NAME *prop = new P##NAME(value);                       \
    this->add(prop);                                          \
    return prop;                                              \
  }
PLIST_ADD(Bool, bool);
PLIST_ADD(Int, int32_t);
PLIST_ADD(Int64, int64_t);
PLIST_ADD(Float, float);
PLIST_ADD(Double, double);
PLIST_ADD(DateTime, time_t);
PLIST_ADD(String, const char *);

PData * PList::addData(void *data, size_t size, bool passOwnership)
{
  PData *prop = new PData(data, size, passOwnership);
  this->add(prop);
  return prop;
}

PDict * PList::addDict()
{
  PDict *prop = new PDict();
  this->add(prop);
  return prop;
}

PList * PList::addList()
{
  PList *prop = new PList();
  this->add(prop);
  return prop;
}

void PList::add(Property *prop)
{
  this->self->list.push_back(prop);
}

void PList::append(PList *list)
{
  PropertyVector::iterator it = list->self->list.begin();
  while (it!=list->self->list.end()) {
    this->add((*it)->makeCopy());
    it++;
  }
}

void PList::remove(size_t index)
{
  this->self->list.erase(this->self->list.begin()+index);
}

Property * PList::get(size_t index)
{
  return this->self->list[index];
}

size_t PList::count() const
{
  return this->self->list.size();
}

void PList::clear()
{
  PropertyVector::iterator it = this->self->list.begin();
  while (it!=this->self->list.end()) {
    delete (*it);
    it++;
  }
  this->self->list.clear();
}

Property * PList::makeCopy() const
{
  PList *list = new PList();
  PropertyVector::iterator it = this->self->list.begin();
  while (it!=this->self->list.end()) {
    list->add((*it)->makeCopy());
    it++;
  }
  return list;
}

class PList::Iterator::PrivateImpl
{
public:
  PropertyVector::iterator it;
  int                      index;
  PList *                  list;
};

PList::Iterator::~Iterator() { delete this->self; }

PList::Iterator::Iterator(): self(new PrivateImpl)
{
}

void PList::Iterator::init(PList *list)
{
  this->self->list = list;
  this->restart();
}


void PList::Iterator::restart()
{
  this->self->it = this->self->list->self->list.begin();
  this->self->index = 0;
}

void PList::Iterator::next()
{
  this->self->it++;
  this->self->index++;
}

bool PList::Iterator::hasNext()
{
  return this->self->it!=this->self->list->self->list.end();
}

int PList::Iterator::index()
{
  return this->self->index;
}

Property * PList::Iterator::property()
{
  return *this->self->it;
}

VT_END_NAMESPACE
