%include "stl.i"
%module vt_TiXmlElement

%{
#include "tinyxml/tinyxml.h"
%}

class TiXmlElement {
public:
  TiXmlElement(const std::string & value);
};
