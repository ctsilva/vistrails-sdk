#IFDEF SWIG<python>
%typemap(out) vt::VersionTree * {
  vt::VersionTree * tree = $1;
  PyObject * tree_d = PyDict_New();
  std::vector<std::string> nodes;
  nodes.push_back(vt::VERSION_ROOT);
  while (! nodes.empty()) {
    PyObject * child_list = PyList_New(0);
    std::string cur_node = nodes.back();
    nodes.pop_back();
    for(std::string version=tree->getFirstChild(cur_node); version != ""; 
	version=tree->getNextSibling(version)) {
      PyList_Append(child_list, PyString_FromString(version.c_str()));
      nodes.push_back(version);
    }
    PyDict_SetItem(tree_d, PyString_FromString(cur_node.c_str()), child_list);
  }
  Py_INCREF(tree_d);
  $result = tree_d;
}
#ENDIF

