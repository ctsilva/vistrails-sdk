/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QAction>
#include <QFileDialog>
#include <QGraphicsView>
#include <QKeySequence>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <iostream>

#include "mainwindow.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include "actions.h"
#include "core/XmlStorage.h"

MainWindow::MainWindow()
{
  createActions();
  createMenus();

  diagramScene = new DiagramScene();
  diagramScene->setBackgroundBrush(QBrush(QColor(245, 245, 245)));
  diagramScene->setSceneRect(QRect(0, 0, 800, 600));

  connect(diagramScene, &DiagramScene::itemMoved,
          this, &MainWindow::itemMoved);

  setWindowTitle("Shapes");
  QGraphicsView *view = new QGraphicsView(diagramScene);
  setCentralWidget(view);
  resize(900, 700);

  this->initVisTrails();
}

MainWindow::~MainWindow()
{
}

void MainWindow::createActions()
{
  deleteAction = new QAction(tr("&Delete Item"), this);
  deleteAction->setShortcut(QKeySequence::Delete);
  connect(deleteAction, &QAction::triggered, this, &MainWindow::deleteItem);

  addBoxAction = new QAction(tr("Add &Box"), this);
  addBoxAction->setShortcut(tr("Ctrl+B"));
  connect(addBoxAction, &QAction::triggered, this, &MainWindow::addBox);

  addTriangleAction = new QAction(tr("Add &Triangle"), this);
  addTriangleAction->setShortcut(tr("Ctrl+T"));
  connect(addTriangleAction, &QAction::triggered, this, &MainWindow::addTriangle);

  chkptBoxAction = new QAction(tr("Add Box and Checkpoint"), this);
  connect(chkptBoxAction, &QAction::triggered, this, &MainWindow::checkpointAddBox);

  chkptTriangleAction = new QAction(tr("Add Triangle and Checkpoint"), this);
  connect(chkptTriangleAction, &QAction::triggered,
          this, &MainWindow::checkpointAddTriangle);

  exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);
  connect(exitAction, &QAction::triggered, this, &MainWindow::close);

  openAction = new QAction(tr("&Open..."), this);
  openAction->setShortcuts(QKeySequence::Open);
  connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

  saveAction = new QAction(tr("&Save..."), this);
  saveAction->setShortcuts(QKeySequence::Save);
  connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

  aboutAction = new QAction(tr("&About"), this);
  connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createMenus()
{
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(openAction);
  fileMenu->addAction(saveAction);
  fileMenu->addSeparator();
  fileMenu->addAction(exitAction);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(deleteAction);
  connect(editMenu, &QMenu::aboutToShow,
          this, &MainWindow::itemMenuAboutToShow);
  connect(editMenu, &QMenu::aboutToHide,
          this, &MainWindow::itemMenuAboutToHide);

  itemMenu = menuBar()->addMenu(tr("&Item"));
  itemMenu->addAction(addBoxAction);
  itemMenu->addAction(addTriangleAction);
  itemMenu->addAction(chkptBoxAction);
  itemMenu->addAction(chkptTriangleAction);

  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction);
}

void MainWindow::itemMoved(DiagramItem *movedItem,
                           const QPointF &oldPosition)
{
  MoveAction * action = new MoveAction(movedItem, movedItem->pos());
  MoveAction * inverse = new MoveAction(movedItem, oldPosition);
  action->execute(diagramScene);

  vt::Version *version = new vt::Version();
  version->performed()->add(vt::ActionList(action));
  version->inverse()->add(vt::ActionList(inverse));
  this->Focus->record(version);
}

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

void MainWindow::itemMenuAboutToHide()
{
  deleteAction->setEnabled(true);
}

void MainWindow::itemMenuAboutToShow()
{
  deleteAction->setEnabled(!diagramScene->selectedItems().isEmpty());
}

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

void MainWindow::addBox()
{
  addItem(DiagramItem::Box, false);
}

void MainWindow::addTriangle()
{
  addItem(DiagramItem::Triangle, false);
}

void MainWindow::checkpointAddBox()
{
  addItem(DiagramItem::Box, true);
}

void MainWindow::checkpointAddTriangle()
{
  addItem(DiagramItem::Triangle, true);
}

void MainWindow::about()
{
  QMessageBox::about(this, tr("About Shapes"),
                     tr("<b>Shapes</b> demonstrates the VisTrails provenance SDK "
                        "by recording every add/move/delete as a versioned action."));
}

void MainWindow::openFile()
{
  QString fname = QFileDialog::getOpenFileName(this, tr("Open History"),
                                               QString(), tr("VisTrails XML (*.xml)"));
  if (fname.isEmpty())
    return;

  this->diagramScene->clear();
  vt::XmlStorage storage(fname.toStdString().c_str());
  this->Session->discard();
  delete this->Session->vistrail();
  this->Session->setVistrail(new vt::Vistrail);
  this->Session->load(&storage);
  this->Focus->reset();
}

void MainWindow::saveFile()
{
  QString fname = QFileDialog::getSaveFileName(this, tr("Save History"),
                                               QString(), tr("VisTrails XML (*.xml)"));
  if (fname.isEmpty())
    return;

  vt::XmlStorage storage(fname.toStdString().c_str());
  this->Session->save(&storage);
}

void MainWindow::closeEvent(QCloseEvent * event)
{
  QWidget::closeEvent(event);
}

void MainWindow::initVisTrails()
{
  vt::ActionFactory *factory = vt::ActionFactory::globalFactory();
  factory->registerAction(AddAction::creator());
  factory->registerAction(MoveAction::creator());
  factory->registerAction(DeleteAction::creator());
  factory->registerAction(SceneAction::creator());

  this->Session = new vt::Session();
  this->Session->info()->setString("user", "<unknown>");
  this->Focus = new vt::SessionFocus(this->Session);
  this->Focus->addHandler(this);

  this->addTriangle();
  this->addBox();
  this->addTriangle();
  this->addBox();
}

void MainWindow::vistrailChanged() {}
void MainWindow::versionChanged(const vt::VersionId &id) {}
void MainWindow::versionAdded(vt::Version *version) {}
void MainWindow::versionSelected(const vt::VersionId &previous,
                                 const vt::VersionId &current)
{
  std::cout << "VERSION SELECTED: " << previous.toString().asChar() << " "
            << current.toString().asChar() << std::endl;
  if (current == vt::VERSION_ROOT) {
    this->diagramScene->clear();
  } else if (previous != current) {
    vt::VersionPath* path = new vt::VersionPath();
    this->Session->vistrail()->getVersionPath(previous, current, path);
    size_t pathRootIndex = path->getRootIndex();

    for (size_t i = 0; i < path->count(); i++) {
      if (i == pathRootIndex)
        continue;

      vt::Action* action = nullptr;
      if (i < pathRootIndex) {
        action = path->get(i)->inverse()->get(0);
      } else if (i > pathRootIndex) {
        action = path->get(i)->performed()->get(0);
      }
      DiagramAction * dAction = reinterpret_cast<DiagramAction *>(action);
      if (dAction != NULL) {
        dAction->execute(this->diagramScene);
      }
    }
    delete path;
  }
}
