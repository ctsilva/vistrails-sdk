%module vt_Property

#ifdef SWIGJAVA
%rename(toPString) *::toString;
#endif

%{
#include "core/Property.h"
  namespace vt {
    class PDictIterator : public PDict::Iterator {};
    class PListIterator : public PList::Iterator {};
  }
%}

#ifdef SWIGPYTHON
%typemap(in) int32_t {
  $1 = PyInt_AsLong($input);
}

%typemap(out) int32_t {
  $result = PyInt_FromLong($1);
}

%typemap(in) int64_t {
  $1 = PyInt_AsLong($input);
}

%typemap(out) int64_t {
  $result = PyInt_FromLong($1);
}
#endif

namespace vt {

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
  
  %define DECLARE_PROPERTY_TYPE(PTYPE) virtual PropertyType type() const { return PTYPE; }
  %enddef
  
  class Property
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

  class PString : public Property
  {
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

  %define DECLARE_PRIMITIVE_PROPERTY(PTYPE, CTYPE, PCONST)     
  class PTYPE : public Property {                                
  public:                                                      
    DECLARE_PROPERTY_TYPE(PCONST);                             
    PTYPE();                                                   
    PTYPE(CTYPE value);                                        
    void setValue(CTYPE value);                                
    CTYPE value() const;                                       
    Property * makeCopy() const;                               
    PString    toString() const;                               
    void       fromString(const PString & pstr);               
  };
  %enddef
  
  DECLARE_PRIMITIVE_PROPERTY(PBool, bool, PROPERTY_BOOL);
  DECLARE_PRIMITIVE_PROPERTY(PInt, int32_t, PROPERTY_INT);
  DECLARE_PRIMITIVE_PROPERTY(PInt64, int64_t, PROPERTY_INT64);
  DECLARE_PRIMITIVE_PROPERTY(PFloat, float, PROPERTY_FLOAT);
  DECLARE_PRIMITIVE_PROPERTY(PDouble, double, PROPERTY_DOUBLE);

  class DataFetcher
  {
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

  class PData : public Property
  {
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

  class PDateTime : public Property
  {
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

  class PDict : public Property
  {
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

  };

  class PDictIterator {
    public:
      PDictIterator();
      virtual     ~PDictIterator();
      void         init(PDict *dict);
      void         restart();
      void         next();
      bool         hasNext();
      const char * key();
      Property   * property();
  };
    

  class PList : public Property
  {
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

  };

  class PListIterator
  {
    public:
      PListIterator();
      virtual     ~PListIterator();
      void       init(PList *list);
      void       restart();
      void       next();
      bool       hasNext();
      int        index();
      Property * property();
  };
  
  %define PROPERTY_AS_VALUE_INLINE(NAME, TYPE)            
    inline TYPE Property::as##NAME() const                  
    {                                                       
      return (dynamic_cast<const P##NAME*>(this))->value(); 
    }
  %enddef
  PROPERTY_AS_VALUE_INLINE(Bool, bool);
  PROPERTY_AS_VALUE_INLINE(Int, int32_t);
  PROPERTY_AS_VALUE_INLINE(Int64, int64_t);
  PROPERTY_AS_VALUE_INLINE(Float, float);
  PROPERTY_AS_VALUE_INLINE(Double, double);
  
  inline const char * Property::asChar() const
  {
    return (dynamic_cast<const PString*>(this))->asChar();
  }

  %define PROPERTY_AS_CAST_INLINE(TYPE)      
    inline P##TYPE * Property::as##TYPE()    
    {                                        
      return dynamic_cast<P##TYPE*>(this);   
    }
  %enddef
  PROPERTY_AS_CAST_INLINE(String);
  PROPERTY_AS_CAST_INLINE(Data);
  PROPERTY_AS_CAST_INLINE(DateTime);
  PROPERTY_AS_CAST_INLINE(Dict);
  PROPERTY_AS_CAST_INLINE(List);
}
