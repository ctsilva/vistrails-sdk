/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */
// Description:

#ifndef PROPERTY_H
#define PROPERTY_H

#include "global.h"
#include <time.h>

#ifdef _MSC_VER
typedef signed __int8     int8_t;
typedef signed __int16    int16_t;
typedef signed __int32    int32_t;
typedef signed __int64    int64_t;
typedef unsigned __int8   uint8_t;
typedef unsigned __int16  uint16_t;
typedef unsigned __int32  uint32_t;
typedef unsigned __int64  uint64_t;
#else
#include <stdint.h>
#endif

VT_BEGIN_NAMESPACE

class Property;
class PBool;
class PData;
class PDateTime;
class PDict;
class PList;
class PDouble;
class PFloat;
class PInt;
class PInt64;
class PString;
class HFetchData;

enum PropertyType {
  PROPERTY_NULL,
  PROPERTY_BOOL,
  PROPERTY_DATA,
  PROPERTY_DATETIME,
  PROPERTY_DICTIONARY,
  PROPERTY_DOUBLE,
  PROPERTY_FLOAT,
  PROPERTY_INT,
  PROPERTY_INT64,
  PROPERTY_LIST,
  PROPERTY_STRING,
  PROPERTY_USER
};

#define DECLARE_PROPERTY_TYPE(PTYPE) virtual PropertyType type() const { return PTYPE; }

class VT_EXPORT Property
{
public:
  virtual ~Property() {}

  DECLARE_PROPERTY_TYPE(PROPERTY_NULL);
  virtual Property * makeCopy() const;
  virtual PString    toString() const;
  virtual void       fromString(const PString & pstr);

  bool               asBool() const;
  int32_t            asInt() const;
  int64_t            asInt64() const;
  float              asFloat() const;
  double             asDouble() const;
  const char *       asChar() const;

  PString *          asString();
  PData *            asData();
  PDateTime *        asDateTime();
  PDict *            asDict();
  PList *            asList();

};

typedef Property PNull;

#define DECLARE_PRIMITIVE_PROPERTY(PTYPE, CTYPE, PCONST)        \
  class VT_EXPORT PTYPE : public Property {                     \
    VT_DECLARE_PIMPL(PTYPE);                                    \
  public:                                                       \
    DECLARE_PROPERTY_TYPE(PCONST);                              \
    PTYPE();                                                    \
    PTYPE(CTYPE value);                                         \
    void setValue(CTYPE value);                                 \
    CTYPE value() const;                                        \
    Property * makeCopy() const;                                \
    PString    toString() const;                                \
    void       fromString(const PString & pstr);                \
  };
DECLARE_PRIMITIVE_PROPERTY(PBool, bool, PROPERTY_BOOL);
DECLARE_PRIMITIVE_PROPERTY(PInt, int32_t, PROPERTY_INT);
DECLARE_PRIMITIVE_PROPERTY(PInt64, int64_t, PROPERTY_INT64);
DECLARE_PRIMITIVE_PROPERTY(PFloat, float, PROPERTY_FLOAT);
DECLARE_PRIMITIVE_PROPERTY(PDouble, double, PROPERTY_DOUBLE);
#undef DECLARE_PRIMITIVE_PROPERTY

class VT_EXPORT PString : public Property
{
  VT_DECLARE_PIMPL(PString);
public:
  DECLARE_PROPERTY_TYPE(PROPERTY_STRING);
  PString();
  PString(const char *str);
  PString(const char *str, size_t n);
  PString      toString() const;
  const char * asChar() const;
  size_t       length() const;  
  int          compare(const char *str) const;
  int          compare(const char *str, size_t n) const;
  void         setValue(const char *str);
  void         setValue(const char *str, size_t n);
  Property *   makeCopy() const;
};

class VT_EXPORT DataFetcher
{
  VT_DECLARE_PIMPL(DataFetcher);
public:
  DataFetcher();
  void         fetch(void *data) const;
  size_t       size() const;
  void *       dataId() const;
  HFetchData * handler() const;

  void         setSize(size_t size);
  void         setDataId(void *dataId);
  void         setHandler(HFetchData *handler);
};

class VT_EXPORT PData : public Property
{
  VT_DECLARE_PIMPL(PData);
public:
  DECLARE_PROPERTY_TYPE(PROPERTY_DATA);

  PData();
  PData(size_t size);
  PData(void *data, size_t size, bool passOwnership);
  
  void        createData(size_t size);
  void        setData(void *data, size_t size, bool passOwnership);

  bool        onDemand() const;
  void        setOnDemand(bool hint);

  DataFetcher dataFetcher() const;
  void        setDataFetcher(DataFetcher fetcher);

  void *      data();
  size_t      size() const;
  void        removeOwnership();
  void *      takeData();
  
  Property *  makeCopy() const;
};

class VT_EXPORT PDateTime : public Property
{
  VT_DECLARE_PIMPL(PDateTime);
public:
  DECLARE_PROPERTY_TYPE(PROPERTY_DATETIME);

  PDateTime();
  PDateTime(time_t t);

  int        day() const;
  int        month() const;
  int        year() const;
  int        hour() const;
  int        minute() const;
  int        second() const;

  void       setDay(int v);
  void       setMonth(int v);
  void       setYear(int v);
  void       setHour(int v);
  void       setMinute(int v);
  void       setSecond(int v);
  
  int64_t    toInt64() const;
  void       fromInt64(int64_t t);
  time_t     toTime() const;
  void       fromTime(time_t t);
  void       toTimeStruct(struct tm *ts) const;
  void       fromTimeStruct(struct tm *ts);

  Property * makeCopy() const;
  PString    toString() const;
  void       fromString(const PString & pstr);
};

class VT_EXPORT PDict : public Property
{
  VT_DECLARE_PIMPL(PDict);
public:
  DECLARE_PROPERTY_TYPE(PROPERTY_DICTIONARY);
  
  PDict();

  PBool *     setBool(const char *key, bool value);
  PFloat *    setFloat(const char *key, float value);
  PDouble *   setDouble(const char *key, double value);
  PInt *      setInt(const char *key, int32_t value);
  PInt64 *    setInt64(const char *key, int64_t value);
  PData *     setData(const char *key, void *data, size_t size, bool passOwnership);
  PDateTime * setDateTime(const char *key, time_t t);
  PString *   setString(const char *key, const char *value);
  PDict *     setDict(const char *key);
  PList *     setList(const char *key);
  void        set(const char *key, Property *prop);
  void        merge(PDict *dict);
  void        remove(const char *key);
  Property *  get(const char *key) const;
  size_t      count() const;
  void        clear();
  Property *  makeCopy() const;

  class Iterator
  {
  public:
    Iterator();
    void         init(PDict *dict);
    void         restart();
    void         next();
    bool         hasNext();
    const char * key();
    Property   * property();
    virtual     ~Iterator();
  private:
    class PrivateImpl;
    PrivateImpl *self;
    friend class PrivateImpl;
  };
};

class VT_EXPORT PList : public Property
{
  VT_DECLARE_PIMPL(PList);
public:
  DECLARE_PROPERTY_TYPE(PROPERTY_LIST);
  
  PList();

  PBool *     addBool(bool value);
  PFloat *    addFloat(float value);
  PDouble *   addDouble(double value);
  PInt *      addInt(int32_t value);
  PInt64 *    addInt64(int64_t value);
  PData *     addData(void *data, size_t size, bool byref=true);
  PDateTime * addDateTime(time_t t);
  PString *   addString(const char *str);
  PDict *     addDict();
  PList *     addList();
  void        add(Property *prop);
  void        append(PList *list);  
  void        remove(size_t index);
  Property *  get(size_t index);
  size_t      count() const;
  void        clear();
  Property *  makeCopy() const;

  class Iterator
  {
  public:
    Iterator();
    void       init(PList *list);
    void       restart();
    void       next();
    bool       hasNext();
    int        index();
    Property * property();
    virtual     ~Iterator();
  private:
    class PrivateImpl;
    PrivateImpl *self;
    friend class PrivateImpl;
  };
};

#define PROPERTY_AS_VALUE_INLINE(NAME, TYPE)               \
  inline TYPE Property::as##NAME() const                   \
  {                                                        \
    return (dynamic_cast<const P##NAME*>(this))->value();  \
  }
PROPERTY_AS_VALUE_INLINE(Bool, bool);
PROPERTY_AS_VALUE_INLINE(Int, int32_t);
PROPERTY_AS_VALUE_INLINE(Int64, int64_t);
PROPERTY_AS_VALUE_INLINE(Float, float);
PROPERTY_AS_VALUE_INLINE(Double, double);
#undef PROPERTY_AS_VALUE_INLINE
inline const char * Property::asChar() const
{
  return (dynamic_cast<const PString*>(this))->asChar();
}

#define PROPERTY_AS_CAST_INLINE(TYPE)      \
  inline P##TYPE * Property::as##TYPE()    \
  {                                        \
    return dynamic_cast<P##TYPE*>(this);   \
  }
PROPERTY_AS_CAST_INLINE(String);
PROPERTY_AS_CAST_INLINE(Data);
PROPERTY_AS_CAST_INLINE(DateTime);
PROPERTY_AS_CAST_INLINE(Dict);
PROPERTY_AS_CAST_INLINE(List);
#undef PROPERTY_AS_CAST_INLINE

VT_END_NAMESPACE

#endif
