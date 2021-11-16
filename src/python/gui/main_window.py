
#
# Copyright (c) 2009-2011 VisTrails, Inc..
#
# This file is part of VisTrails SDK
# (see https://github.com/vistrails/sdk).
#
# License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
#


from PyQt4 import QtCore, QtGui

import sys
sys.path.append('/vistrailsinc/svn/sdk/capi/src/build/python')

import vistrails
import testing

from version_view import QVersionTreeView
from version_prop import QVersionProp
from theme import initializeCurrentTheme

class VisTrailsMain(QtGui.QMainWindow):
    def __init__(self, session=None, parent=None):
        QtGui.QMainWindow.__init__(self, parent)

        vistrails.initialize()
        session = vistrails.Session()
        session.setUser("dakoop@gmail.com")
        focus = vistrails.SessionFocus(session)

        self.session = session
        self.focus = focus
        self.build_layout()
        self.set_signals()

    def set_session(self, session):
        self.session = session

    def set_session_focus(self, focus):
        self.focus = focus

    def build_layout(self):
        self.version_view = QVersionTreeView()
        self.version_prop = QVersionProp()
        self.setCentralWidget(self.version_view)
        self.addDockWidget(QtCore.Qt.RightDockWidgetArea, 
                           self.version_prop.toolWindow())
    
        file_menu = self.menuBar().addMenu("File");
        openAction = QtGui.QAction("Open File...", self)
        self.connect(openAction, QtCore.SIGNAL("triggered()"), self.open_file)
        file_menu.addAction(openAction)
        saveAction = QtGui.QAction("Save File...", self)
        self.connect(saveAction, QtCore.SIGNAL("triggered()"), self.save_file)
        file_menu.addAction(saveAction)

    def set_signals(self):
        self.connect(self.version_view.scene(),
                     QtCore.SIGNAL("versionSelected(QString *, bool, bool, bool)"),
                     self.version_selected)
        vistrails.set_py_vt_changed_cb(self.vistrail_changed)
        vistrails.set_py_add_version_cb(self.add_version)

    def unset_signals(self):
        vistrails.set_py_vt_changed_cb(None)
        vistrails.set_py_add_version_cb(None)

    def version_selected(self, node_id, by_click, do_validate, from_root):
        old_id = self.focus.getCurrentVersion()
        if node_id == old_id:
            return
        if node_id == "":
            self.version_prop.updateVersion(None)
            self.focus.selectVersion("")
            return
        
        node_id = str(node_id)
        print "got version selected", node_id
        tree = self.session.getVistrail().getVersionTree()
        self.version_prop.updateVersion(tree.getVersion(node_id))
        self.focus.selectVersion(node_id)

        path = vistrails.ActionPath()
        self.session.getVistrail().getMaterializationPath(node_id, old_id, 
                                                          path)        
        for i in xrange(path.count()):
            print i, path.action(i)
        vistrails.version_selected(path)

    def do_add_version(self):
        self.add_version(testing.RectAction(), testing.RectActionInverse())

    def do_add_version_chkpt(self):
        self.add_version(testing.RectAction(), testing.RectActionInverse(),
                         testing.RectActionCheckpoint())
        
    def add_version(self, *actions):
        print "ADDING VERSION", actions
        if len(actions) < 1:
            return
        else:
            self.focus.record(*actions)
        print "AFTER ADD", self.focus.getCurrentVersion()
        # self.focus.record(testing.RectAction())

    def vistrail_changed(self):
        print "VISTRAIL CHANGED"
        tree = self.session.getVistrail().getVersionTree()
        print "CURRENT VERSION:", self.focus.getCurrentVersion()
        self.version_view.setupScene(tree, self.focus.getCurrentVersion())

    def open_file(self):
        fname = QtGui.QFileDialog.getOpenFileName(self, "Open History")
        storage = vistrails.XMLStorage(str(fname))
        self.session.clear()
        self.session.bind(storage)
        self.session.update()
        self.vistrail_changed()

    def save_file(self):
        fname = QtGui.QFileDialog.getSaveFileName(self, "Save History")
        storage = vistrails.XMLStorage(str(fname))
        storage.save(self.session.getVistrail())

main_window = None

def run():
    global main_window
    print ")*()*( calling run"
    app = QtCore.QCoreApplication.instance()
    needsExec = False
    if app is None:
        print "app is None!"
        app = QtGui.QApplication([])
        needsExec = True

    initializeCurrentTheme()
    print "starting main_window!!!"
    main_window = VisTrailsMain()
    main_window.setAttribute(QtCore.Qt.WA_QuitOnClose)

    # action_list = vistrails.ActionList()
    # action_list.add(testing.RectAction())
    # print "calling add_version"
    # vistrails.add_version(action_list)
    # vistrails.add_version(testing.OvalAction(), testing.OvalActionInverse())
    # print "done with add_version"
    main_window.vistrail_changed()

    # (session, focus) = testing.run(main_window.vistrail_changed)
    # main_window.set_session(session)
    # main_window.set_session_focus(focus)
    # main_window.vistrail_changed()
    main_window.show()
    print "done with show!"
    if needsExec:
        print "doing exec"
        app.exec_()
    # return main_window
        
def stop():
    global main_window
    print ")*()*( calling stop"
    app = QtCore.QCoreApplication.instance()
    main_window.unset_signals()
    # main_window.deleteLater()
    # app.processEvents()
    # print "refcount:", sys.getrefcount(main_window)
    # del main_window
    # main_window = None
    print "done with stop"

if __name__ == '__main__':
    run()
