/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QtGui>

#include "mainwindow.h"
#include "diagramscene.h"
#include "diagramitem.h"
//#include "commands.h"
#include "actions.h"
#include "gui/vtwindow.hpp"
#include "core/XmlStorage.h"

// #include "core/global.h"
// #include "db/vt_stream.h"
// #include "db/vt_stream_xml.h"
// #include "db/vt_data_xml.h"

static DiagramScene *globalScene = NULL;
MainWindow * instance = NULL;

// void versionSelected(vt::ActionPath * path)
// {
//   printf("calling versionSelected callback\n");
//   instance->newVersionSelected(path);
// }

//! [0]
MainWindow::MainWindow()
{
  //undoStack = new QUndoStack();

  createActions();
  createMenus();

  //createUndoView();

  diagramScene = new DiagramScene();
  globalScene = diagramScene;
  QBrush pixmapBrush(QPixmap(":/images/cross.png").scaled(30, 30));
  diagramScene->setBackgroundBrush(pixmapBrush);
  diagramScene->setSceneRect(QRect(0, 0, 500, 500));

  connect(diagramScene, SIGNAL(itemMoved(DiagramItem*,QPointF)),
          this, SLOT(itemMoved(DiagramItem*,QPointF)));

  setWindowTitle("Shapes");
  QGraphicsView *view = new QGraphicsView(diagramScene);
  setCentralWidget(view);
  resize(800, 600);

  this->initVisTrails();
  instance = this;
}
//! [0]

MainWindow::~MainWindow()
{
}


//! [1]
void MainWindow::createUndoView()
{
  undoView = new QUndoView(undoStack);
  undoView->setWindowTitle(tr("Command List"));
  undoView->show();
  undoView->setAttribute(Qt::WA_QuitOnClose, false);
}
//! [1]

//! [2]
void MainWindow::createActions()
{
  deleteAction = new QAction(tr("&Delete Item"), this);
  deleteAction->setShortcut(tr("Del"));
  connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteItem()));
  //! [2] //! [3]

  //! [3] //! [4]
  addBoxAction = new QAction(tr("Add &Box"), this);
  //! [4]
  addBoxAction->setShortcut(tr("Ctrl+O"));
  connect(addBoxAction, SIGNAL(triggered()), this, SLOT(addBox()));

  addTriangleAction = new QAction(tr("Add &Triangle"), this);
  addTriangleAction->setShortcut(tr("Ctrl+T"));
  connect(addTriangleAction, SIGNAL(triggered()), this, SLOT(addTriangle()));

  chkptBoxAction = new QAction(tr("Add Box and Checkpoint"), this);
  connect(chkptBoxAction, SIGNAL(triggered()), this, SLOT(checkpointAddBox()));

  chkptTriangleAction = new QAction(tr("Add Triangle and Checkpoint"), this);
  connect(chkptTriangleAction, SIGNAL(triggered()), this, 
	  SLOT(checkpointAddTriangle()));

  undoAction = new QAction(tr("Undo"), this);
  undoAction->setShortcuts(QKeySequence::Undo);

  redoAction = new QAction(tr("Redo"), this);
  redoAction->setShortcuts(QKeySequence::Redo);
  
  //! [5]
  // undoAction = undoStack->createUndoAction(this, tr("&Undo"));
  // undoAction->setShortcuts(QKeySequence::Undo);

  // redoAction = undoStack->createRedoAction(this, tr("&Redo"));
  // redoAction->setShortcuts(QKeySequence::Redo);
  //! [5]

  exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);
  connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

  openAction = new QAction(tr("&Open"), this);
  openAction->setShortcuts(QKeySequence::Open);
  connect(openAction, SIGNAL(triggered()), this, SLOT(loadFile()));

  saveAction = new QAction(tr("&Save"), this);
  saveAction->setShortcuts(QKeySequence::Save);
  connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

  aboutAction = new QAction(tr("&About"), this);
  QList<QKeySequence> aboutShortcuts;
  aboutShortcuts << tr("Ctrl+A") << tr("Ctrl+B");
  aboutAction->setShortcuts(aboutShortcuts);
  connect(aboutAction, SIGNAL(triggered()), this, SLOT(about()));
}

//! [6]
void MainWindow::createMenus()
{
  //! [6]
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addAction(exitAction);

  //! [7]
  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(undoAction);
  editMenu->addAction(redoAction);
  editMenu->addSeparator();
  editMenu->addAction(deleteAction);
  connect(editMenu, SIGNAL(aboutToShow()),
          this, SLOT(itemMenuAboutToShow()));
  connect(editMenu, SIGNAL(aboutToHide()),
          this, SLOT(itemMenuAboutToHide()));

  //! [7]
  itemMenu = menuBar()->addMenu(tr("&Item"));
  itemMenu->addAction(addBoxAction);
  itemMenu->addAction(addTriangleAction);
  itemMenu->addAction(chkptBoxAction);
  itemMenu->addAction(chkptTriangleAction);

  helpMenu = menuBar()->addMenu(tr("&About"));
  helpMenu->addAction(aboutAction);
  //! [8]
}
//! [8]

//! [9]
void MainWindow::itemMoved(DiagramItem *movedItem,
                           const QPointF &oldPosition)
{
  MoveAction * action = new MoveAction(movedItem, movedItem->pos());
  MoveAction * inverse = new MoveAction(movedItem, oldPosition);
  //std::string inverse_str = "inverse";
  action->execute(diagramScene);

  vt::Version *version = new vt::Version();
  version->performed()->add(vt::ActionList(action));
  version->inverse()->add(vt::ActionList(inverse));
  this->Focus->record(version);
}
//! [9]

//! [10]
void MainWindow::deleteItem()
{
  if (diagramScene->selectedItems().isEmpty())
    return;
  QList<QGraphicsItem *> list = diagramScene->selectedItems();
  list.first()->setSelected(false);
  DiagramItem * item = static_cast<DiagramItem *>(list.first());
  int id = item->id();

  DeleteAction * action = new DeleteAction(id);
  AddAction * inverse = new AddAction(item->diagramType(), item->pos(),
				      diagramScene, 0, id);
  action->execute(diagramScene);

  vt::Version *version = new vt::Version();
  version->performed()->add(vt::ActionList(action));
  version->inverse()->add(vt::ActionList(inverse));
  this->Focus->record(version);
}
//! [10]

//! [11]
void MainWindow::itemMenuAboutToHide()
{
  deleteAction->setEnabled(true);
}
//! [11]

//! [12]
void MainWindow::itemMenuAboutToShow()
{
  deleteAction->setEnabled(!diagramScene->selectedItems().isEmpty());
}
//! [12]

static int itemCount = 0;
void MainWindow::addItem(DiagramItem::DiagramType itemType, 
			 bool make_checkpoint)
{
  QPointF pos = QPointF((itemCount * DIAGRAM_ITEM_SCALE/2) % 
			int(diagramScene->width()),
                        (itemCount * DIAGRAM_ITEM_SCALE/2) % 
			int(diagramScene->height()));
  AddAction * action = new AddAction(itemType, pos, diagramScene, 
				     itemCount++);
  DeleteAction * inverse = new DeleteAction(itemCount-1);
  action->execute(diagramScene);

  vt::Version *version = new vt::Version();
  version->performed()->add(vt::ActionList(action));
  version->inverse()->add(vt::ActionList(inverse));

  if (make_checkpoint) {
    SceneAction * checkpoint = new SceneAction(diagramScene);
    version->checkpoint()->add(vt::ActionList(checkpoint));
  }

  this->Focus->record(version);
  std::cout << "ADDING VERSION: " << version->id().toString().asChar() << std::endl;
}

//! [13]
void MainWindow::addBox()
{
  addItem(DiagramItem::Box, false);
}
//! [13]

//! [14]
void MainWindow::addTriangle()
{
  addItem(DiagramItem::Triangle, false);
}
//! [14]

void MainWindow::checkpointAddBox()
{
  addItem(DiagramItem::Box, true);
}

void MainWindow::checkpointAddTriangle()
{
  addItem(DiagramItem::Triangle, true);
}


//! [15]
void MainWindow::about()
{
  QMessageBox::about(this, tr("About Undo"),
                     tr("The <b>Undo</b> example demonstrates how to "
                        "use Qt's undo framework."));
}
//! [15]

// void MainWindow::openFile()
// {
//   QString fname = QFileDialog::getOpenFileName(this, "Open History");
//   // vt::XMLStorage storage(fname.toStdString().c_str());
//   vt::XmlStorage storage(fname.toStdString().c_str());
//   // session->clear();
//   // session->bind(&storage);
//   // session->update();
//   this->diagramScene->clear();
//   // this->versionScene->refresh();
//   // this->focus->setAutoSelectVersion(false);
//   // this->focus->selectVersion(vt::VERSION_ROOT);
//   // this->versionScene->selectVersion(this->focus->getCurrentVersion());

// }

// void MainWindow::saveFile()
// {
//   QString fname = QFileDialog::getSaveFileName(this, "Save History");
//   std::cout << "saving " << fname.toStdString() << std::endl;
//   vt::XmlStorage storage(fname.toStdString().c_str());
//   // storage.save(session->getVistrail());
// }

// void MainWindow::indexChanged(int index)
// {

// }

void MainWindow::closeEvent(QCloseEvent * event)
{
  // endVistrails();
  // vt::XMLStorage storage("/tmp/undo_vt2.xml");
  // storage.save(session->getVistrail());
  QWidget::closeEvent(event);
}

void loadFileCb(const char * filePath)
{
  instance->loadFile(filePath);
}

void saveFileCb(const char * filePath)
{
  instance->saveFile(filePath);
}

void newFileCb()
{
  instance->newFile(); 
}

void MainWindow::loadFile(const char * filename)
{
  QString fname(filename);
  if (fname.isEmpty())
    return;
  
  this->diagramScene->clear();
  vt::XmlStorage storage(filename);
  this->Session->discard();
  delete this->Session->vistrail();
  this->Session->setVistrail(new vt::Vistrail);
  this->Session->load(&storage);
  this->Focus->reset();
}

void MainWindow::saveFile(const char * filename)
{
  QString fname(filename);
  if (fname.isEmpty())
    return;

  vt::Storage * storage = new vt::XmlStorage(filename);
  this->Session->save(storage);
  delete storage;
}

void MainWindow::newFile()
{
  std::cout << "STARTING NEW FILE" << std::endl;
  this->diagramScene->clear();
  this->Session->discard();
  delete this->Session->vistrail();
  this->Session->setVistrail(new vt::Vistrail);
  this->Focus->reset();
}

void MainWindow::initVisTrails()
{
  vt::ActionFactory *factory = vt::ActionFactory::globalFactory();  
  factory->registerAction(AddAction::creator());
  factory->registerAction(MoveAction::creator());
  factory->registerAction(DeleteAction::creator());
  factory->registerAction(SceneAction::creator());

  //set up our vt session and focus
  this->Session = new vt::Session();
  this->Session->info()->setString("user", "<unknown>");
  this->Focus = new vt::SessionFocus(this->Session);
  this->Focus->addHandler(this);

  this->vtWindow = new VisTrailsWindow();
  this->vtWindow->show();
  this->vtWindow->setSessionFocus(this->Focus);
  this->vtWindow->setLoadFileCallBack(&loadFileCb);
  this->vtWindow->setSaveFileCallBack(&saveFileCb);
  this->vtWindow->setNewFileCallBack(&newFileCb);

  //vtWindow = VisTrailsWindow::getInstance();

  /*
  startVistrails();
  vt::set_version_selected_cb(&versionSelected);
  vt::set_data_proxy(AddAction::createProxy(), 
		     "com.vistrails.provexp.example.addaction");
  vt::set_data_proxy(MoveAction::createProxy(),
		     "com.vistrails.provexp.example.moveaction");
  vt::set_data_proxy(DeleteAction::createProxy(), 
		     "com.vistrails.provexp.example.deleteaction");
  vt::set_data_proxy(SceneAction::createProxy(), 
		     "com.vistrails.provexp.example.sceneaction");
  */
  this->addTriangle();
  this->addBox();
  // this->undoStack->undo();
  this->addTriangle();
  this->addBox();
}

void MainWindow::vistrailChanged() {}
void MainWindow::versionChanged(const vt::VersionId &id) {}
void MainWindow::versionAdded(vt::Version *version) {}
void MainWindow::versionSelected(const vt::VersionId &previous,
				 const vt::VersionId &current)
{
  std::cout << "VERSION SELECTED: " << previous.toString().asChar() << " " << current.toString().asChar() << std::endl;
  if (current == vt::VERSION_ROOT) {
    this->diagramScene->clear();
  } else if(previous != current) {
    // always clear for now
    // this->diagramScene->clear();
    vt::VersionPath* path = new vt::VersionPath();
    this->Session->vistrail()->getVersionPath(previous, current, path);
    size_t pathRootIndex = path->getRootIndex();
    std::cout << "PATH ROOT INDEX:" << pathRootIndex << std::endl;
    vt::Version * pathRoot = path->get(pathRootIndex);
    
    for(size_t i = 0; i < path->count(); i++) {
      if (i == pathRootIndex)
	continue;

      std::cout << "PATH IDX: " << i << std::endl;
      std::cout << "VERSION: " << path->get(i)->id().toString().asChar() << std::endl;
      vt::Action* action;
      if (i < pathRootIndex) {
	std::cout << "DOING INVERSE" << std::endl;
	action = path->get(i)->inverse()->get(0);
      } else if (i > pathRootIndex) {
	std::cout << "DOING FORWARD" << std::endl;
	action = path->get(i)->performed()->get(0);
      }
      DiagramAction * dAction = reinterpret_cast<DiagramAction *>(action);
      if (dAction != NULL) {
    	dAction->execute(this->diagramScene);
      }
    }

    // vt::VersionPath* fromRoot = new vt::VersionPath();
    // this->Session->vistrail()->getVersionPath(vt::VersionId::root(), 
    // 					      pathRoot->id(), fromRoot);
    // for(size_t i = 1; i < fromRoot->count(); i++) {
    //   std::cout << "PATH IDX: " << i << std::endl;
    //   std::cout << "VERSION: " << fromRoot->get(i)->id().toString().asChar() << std::endl;
    //   vt::Action* action = fromRoot->get(i)->performed()->get(0);
    //   DiagramAction * dAction = reinterpret_cast<DiagramAction *>(action);
    //   if (dAction != NULL) {
    // 	dAction->execute(this->diagramScene);
    //   }      
    // }
    // delete fromRoot;
    // std::cout << "DONE WITH FROM ROOT!" << std::endl;
    // for(size_t i = pathRootIndex+1; i < path->count(); i++) {
    //   // all of our versions just have one performed action
    //   std::cout << "F PATH IDX: " << i << std::endl;
    //   std::cout << "F VERSION: " << path->get(i)->id().toString().asChar() << std::endl;
    //   vt::Action* action = path->get(i)->performed()->get(0);
    //   DiagramAction * dAction = reinterpret_cast<DiagramAction *>(action);
    //   if (dAction != NULL) {
    // 	dAction->execute(this->diagramScene);
    //   }
    // }
    // delete path;
  }
}


// void MainWindow::newVersionSelected(vt::ActionPath * path)
// {
//   if (path == NULL || path->needs_reset()) {
//     this->diagramScene->clear();
//     // this->undoStack->clear();
//   }
//   if (path != NULL) {
//     for (size_t i=0; i < path->count(); i++) {
//       DiagramAction * action = 
// 	reinterpret_cast<DiagramAction *>(path->action(i));
//       if (action != NULL) {
// 	printf("FOUND ACTION: %s\n", action->db_get_type().c_str());
// 	action->execute(this->diagramScene);
//       } else {
// 	printf("FOUND NULL ACTION\n");
//       }
//     }
//   }
// }
