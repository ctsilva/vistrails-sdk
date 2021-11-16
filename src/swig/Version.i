%module vt_Version

#ifdef SWIGJAVA
%rename(toPString) vt::VersionId::toString;
#endif

%{
#include "core/Version.h"
%}

namespace vt {
  class VersionId
  {
  public:
    VersionId();
    VersionId(const char *str);
    VersionId(const PString & pstr);
    PString toString() const;
    void fromString(const PString & pstr);

    static VersionId root();
    static VersionId invalid();
    static VersionId gen();

    uint8_t data[16];
  };

  const VersionId VERSION_ROOT = VersionId::root();
  const VersionId VERSION_INVALID = VersionId::invalid();

  class Version
  {
  public:
    Version();
    const VersionId & id() const;
    const VersionId & parent() const;
    ActionList *      performed();
    ActionList *      inverse();
    ActionList *      checkpoint();
    PDict *           stamp();
    PDict *           annotations();

    void              setId(const VersionId &id);
    void              setParent(const VersionId &id);
  };

  class VersionList
  {
  public:
    VersionList();
    size_t    count() const;
    void      clear();
    void      set(size_t index, Version *version) const;
    void      add(Version * version);
    Version * get(size_t idx) const;
  };

  class VersionPath: public VersionList
  {
  public:
    VersionPath();
    void      setRootIndex(size_t idx);
    void      addRoot(Version * version);
    size_t    getRootIndex();
    Version * getRoot();
  };

  inline bool operator==(VersionId const &lhs, VersionId const &rhs)
  {
    return std::equal(lhs.data, lhs.data+sizeof(lhs.data)/sizeof(lhs.data[0]), rhs.data);
  }

  inline bool operator!=(VersionId const &lhs, VersionId const &rhs)
  {
    return !(lhs==rhs);
  }

  inline bool operator<(VersionId const &lhs, VersionId const &rhs)
  {
    return std::lexicographical_compare(lhs.data, lhs.data+sizeof(lhs.data)/sizeof(lhs.data[0]),
                                        rhs.data, rhs.data+sizeof(rhs.data)/sizeof(rhs.data[0]));
  }

  inline bool operator>(VersionId const &lhs, VersionId const &rhs)
  {
    return rhs<lhs;
  }

  inline bool operator<=(VersionId const &lhs, VersionId const &rhs)
  {
    return !(rhs<lhs);
  }

  inline bool operator>=(VersionId const &lhs, VersionId const &rhs)
  {
    return !(lhs<rhs);
  }

  inline void swap(VersionId &lhs, VersionId &rhs)
  {
    std::swap_ranges(lhs.data, lhs.data+sizeof(lhs.data)/sizeof(lhs.data[0]), rhs.data);
  }

  inline size_t hash_value(VersionId const& u)
  {
    size_t seed = 0;
    for(int i=0; i<sizeof(u.data)/sizeof(u.data[0]); i++)
        seed ^= static_cast<size_t>(u.data[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    return seed;
  }
}
