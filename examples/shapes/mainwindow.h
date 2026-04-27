/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <VisTrails.h>
#include "diagramitem.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
QT_END_NAMESPACE
class DiagramScene;

class MainWindow : public QMainWindow, public vt::HNotify
{
  Q_OBJECT

  public:
  MainWindow();
  ~MainWindow();
  void closeEvent(QCloseEvent * event);

  void vistrailChanged();
  void versionChanged(const vt::VersionId &id);
  void versionAdded(vt::Version *version);
  void versionSelected(const vt::VersionId &previous,
                       const vt::VersionId &current);

public slots:
  void itemMoved(DiagramItem *movedDiagram, const QPointF &moveStartPosition);

private slots:
  void deleteItem();
  void addBox();
  void addTriangle();
  void checkpointAddBox();
  void checkpointAddTriangle();
  void about();
  void itemMenuAboutToShow();
  void itemMenuAboutToHide();
  void openFile();
  void saveFile();

private:
  void createActions();
  void createMenus();
  void initVisTrails();
  void addItem(DiagramItem::DiagramType itemType, bool make_checkpoint);

  QAction *deleteAction;
  QAction *addBoxAction;
  QAction *addTriangleAction;
  QAction *chkptBoxAction;
  QAction *chkptTriangleAction;
  QAction *exitAction;
  QAction *aboutAction;
  QAction *openAction;
  QAction *saveAction;

  QMenu *fileMenu;
  QMenu *editMenu;
  QMenu *itemMenu;
  QMenu *helpMenu;

  DiagramScene *diagramScene;

  vt::SessionFocus *Focus;
  vt::Session *Session;
};

#endif
