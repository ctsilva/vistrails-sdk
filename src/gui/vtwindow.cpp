/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "vtwindow.hpp"

#include <QtGui>
#include <QtCore>
#include "ui_vistrailsui.h"
#include "ui_about.h"
#include "preferencesdialog.hpp"
#include "info.hpp"
#include "delegate.hpp"
#include "vtmodel.hpp"
#include "vtnode.hpp"
#include "vttableproxy.hpp"
#include "vtsortfilterproxy.hpp"

PreferencesDialog* PreferencesDialog::instance = NULL;
VisTrailsWindow* VisTrailsWindow::instance = NULL;

// helper function, return default value if property doesn't exist
inline QString getVal(vt::PDict* dict, const char *key, 
                      const char *deflt)
{
  vt::Property *p = dict->get(key);
  if(p)
    return QString(p->toString().asChar());
  return QString(deflt);
}

/*
  VisTrailsWindow(QWidget *parent)
  Constructor of VisTrailsWindow
*/

VisTrailsWindow::VisTrailsWindow(QWidget *parent) : 
  QMainWindow(parent), 
  ui(new Ui::VisTrailsMainWindow)
{ 
  ui->setupUi(this);
  initModel();
  this->setThumbnailLength(80);
  initListView();
  initTableView();
  initTreeView();
  createMenuActions();
  this->playbackTimer = new QTimer(this);
  bFiltering = false;
  unsavedWork = false;
  ignoreChanged = false;
  loadFileCallBack = NULL;
  saveFileCallBack = NULL;
  newFileCallBack = NULL;
  settings = new QSettings("VisTrails", "VisTrails SDK", this);
  loadSettings();

  viewActionGroup = new QActionGroup(this);
  viewActionGroup->addAction(ui->actionTable);
  viewActionGroup->addAction(ui->actionInfo);
  viewActionGroup->addAction(ui->actionDir);
  viewActionGroup->addAction(ui->actionTree);
  viewActionGroup->setExclusive(true);

  this->versionContextMenu = new VersionContextMenu(this);
  ui->filterGroup->setVisible(false);
  ui->sortGroup->setVisible(false);
  ui->playGroup->setVisible(false);

  this->playbackPath = new vt::VersionPath();
  this->playbackSelectionMode = NotSelectingPlayback;

  this->about = new QDialog(this);
  Ui::dlgAbout about_ui = Ui::dlgAbout();
  about_ui.setupUi(this->about);
  
  createConnections();
}

void VisTrailsWindow::loadSettings()
{  
  resize(settings->value("windowSize", QSize(515, 641)).toSize());
  move(settings->value("windowPos", QPoint(0, 0)).toPoint());     
}


/*
  getInstance()
  Initialize singleton function
*/
VisTrailsWindow* VisTrailsWindow::getInstance()
{
  if(!instance)
    {
      instance = new VisTrailsWindow();
    }
  return instance;
}

imgGallery * VisTrailsWindow::getGraphicsView()
{
  return ui->graphicsView;
}

QStackedWidget * VisTrailsWindow::getTabWidget()
{
  return ui->stackedWidget;
}

QListView * VisTrailsWindow::getListView()
{
  return ui->listView;
}

QTableView *VisTrailsWindow::getTableView()
{
  return ui->tbView;
}


int VisTrailsWindow::getCurrentViewIndex()
{
  return ui->stackedWidget->currentIndex();
}

/*
  initModel()
  Initialize vistrail model
*/
void VisTrailsWindow::initModel()
{
  vtModel = new VtModel(NULL);
  // vtModel->setHorizontalHeaderLabels(QStringList() << tr("Thumb") 
  //       			     << tr("Tag") 
  //       			     << tr("Action") << tr("Date") 
  //       			     << tr("User"));  
  vtTableProxy = new VtTableProxyModel(vtModel);
  vtSortFilterProxy = new VtSortFilterProxy(vtTableProxy);
  vtSelectionModel = new QItemSelectionModel(vtSortFilterProxy);
  connect(vtSelectionModel,
	  SIGNAL(currentRowChanged(QModelIndex, QModelIndex)),
	  this,
	  SLOT(currentRowChanged(QModelIndex, QModelIndex)));    
}


/*
  initListView()
  Initialize list view
*/
void VisTrailsWindow::initTableView()
{
  ui->tbView->setModel(vtSortFilterProxy);
  ui->tbView->setSelectionModel(vtSelectionModel);
  ui->tbView->setIconSize(QSize(thumbnailLength(),thumbnailLength()));
  ui->tbView->verticalHeader()->setDefaultSectionSize(thumbnailLength());
  ui->tbView->verticalHeader()->setVisible(false);
  ui->tbView->horizontalHeader()->setResizeMode(0, QHeaderView::Fixed);
  ui->tbView->horizontalHeader()->resizeSection(0, thumbnailLength()+10);
  ui->tbView->setItemDelegate(new TableDelegate());
}


/*
  initListView()
  Initialize directory view
*/
void VisTrailsWindow::initListView()
{
  ui->listView->setModel(vtSortFilterProxy);
  ui->listView->setSelectionModel(vtSelectionModel);
  ui->listView->setItemDelegate(new ListDelegate());
  ui->listView->setUpdatesEnabled(true);
}

void VisTrailsWindow::initTreeView()
{
    
}

/*
  createMenuActions()
  Create menu actions
*/
void VisTrailsWindow::createMenuActions()
{
  ui->actionExit->setShortcut(QKeySequence::Close);
  ui->actionExit->setStatusTip(tr("Exit"));

  ui->menuFile->addAction(ui->actionExit);

  ui->actionOpen->setShortcut(QKeySequence::Open);
  ui->actionOpen->setStatusTip(tr("Open"));

  QMenu *sortMenu = new QMenu();
  this->sortAscending = sortMenu->addAction("Sort Descending");
  sortMenu->addSeparator();
  this->sortActionGroup = new QActionGroup(this);
  QAction *sortAction;

  sortAction = new QAction("", sortMenu);
  sortAction->setCheckable(true);
  sortAction->setChecked(true);
  this->sortActionGroup->addAction(sortAction);
  sortMenu->addAction(sortAction);

  this->updateSortIndicator(-1, Qt::AscendingOrder);

  sortAction = new QAction("Tag", sortMenu);
  sortAction->setCheckable(true);
  this->sortActionGroup->addAction(sortAction);
  sortMenu->addAction(sortAction);

  sortAction = new QAction("Action", sortMenu);
  sortAction->setCheckable(true);
  this->sortActionGroup->addAction(sortAction);
  sortMenu->addAction(sortAction);

  sortAction = new QAction("Date", sortMenu);
  sortAction->setCheckable(true);
  this->sortActionGroup->addAction(sortAction);
  sortMenu->addAction(sortAction);

  sortAction = new QAction("User", sortMenu);
  sortAction->setCheckable(true);
  this->sortActionGroup->addAction(sortAction);
  sortMenu->addAction(sortAction);

  ui->actionSort->setMenu(sortMenu);
  QToolButton * sortButton = (QToolButton *) ui->toolBar->widgetForAction(ui->actionSort);
  sortButton->setPopupMode(QToolButton::InstantPopup);

  //Playback speed selection menu
  QMenu *speedMenu = new QMenu(this);
  this->playSpeedGroup = new QActionGroup(this);
  QAction *speedAction;

  speedAction = new QAction("Slow", speedMenu);
  speedAction->setCheckable(true);
  this->playSpeedGroup->addAction(speedAction);
  speedMenu->addAction(speedAction);

  speedAction = new QAction("Medium", speedMenu);
  speedAction->setCheckable(true);
  speedAction->setChecked(true);
  this->playSpeedGroup->addAction(speedAction);
  speedMenu->addAction(speedAction);

  speedAction = new QAction("Fast", speedMenu);
  speedAction->setCheckable(true);
  this->playSpeedGroup->addAction(speedAction);
  speedMenu->addAction(speedAction);

  ui->speedSelectButton->setMenu(speedMenu);
}


/*
  createConnections()
  Create signals here
*/
void VisTrailsWindow::createConnections()
{
  connect(this->sortActionGroup, SIGNAL(triggered(QAction*)), this, SLOT(sortTypeChanged(QAction*)));
  connect(this->sortAscending, SIGNAL(triggered(bool)), this, SLOT(sortAscendingChanged(bool)));
  connect(ui->tbView->horizontalHeader(), 
	  SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this,
	  SLOT(tableSortChanged(int,Qt::SortOrder)));
  //connect(ui->actionSort, SIGNAL(triggered()), this, SLOT(showSort()));
  //connect(ui->cbSort, SIGNAL(currentIndexChanged(int)), this, SLOT(sortComboChanged()));
  //connect(ui->checkAscending, SIGNAL(clicked()), this, SLOT(sortComboChanged()));
  connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));
  connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openFile()));
  connect(ui->actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
  connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(save()));
  connect(ui->actionNext, SIGNAL(triggered()), this, SLOT(nextItem()));
  connect(ui->actionPrevious, SIGNAL(triggered()), this, SLOT(previousItem()));
  connect(ui->filterButton, SIGNAL(clicked()), this, SLOT(filterClicked()));
  connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetFilter()));
  connect(ui->resetButton, SIGNAL(clicked()), ui->treeView->getTreeScene(), 
    SLOT(setupScene()));
  connect(ui->filterTypeCombo, SIGNAL(currentIndexChanged(int)), this,
	  SLOT(updateFilterType(int)));
  connect(ui->actionFind,SIGNAL(triggered()),this,SLOT(filterChecked()));
  connect(ui->actionPreferences, SIGNAL(triggered()), this, SLOT(openPreferences()));
  connect(ui->versionTag, SIGNAL(editingFinished()), this, SLOT(tagChanged()));
  connect(ui->actionPlayback, SIGNAL(toggled(bool)), this, SLOT(playbackToggled(bool)));
  connect(ui->closePlaybackButton, SIGNAL(clicked()), ui->actionPlayback, SLOT(toggle()));
  connect(ui->playPauseButton, SIGNAL(clicked(bool)), this, SLOT(playPauseToggled(bool)));
  connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(stopPlayback()));
  connect(ui->selectVersionsButton, SIGNAL(clicked(bool)), this, SLOT(selectPlaybackVersions(bool)));
  connect(this->playbackTimer, SIGNAL(timeout()), this, SLOT(playbackNextVersion()));
  connect(ui->playbackSlider, SIGNAL(valueChanged(int)), this, SLOT(playbackSliderChanged(int)));
  connect(ui->cancelSelectButton, SIGNAL(clicked()), this, SLOT(cancelPlaybackSelection()));
  connect(ui->actionZoomIn, SIGNAL(triggered()), ui->treeView, SLOT(zoomIn()));
  connect(ui->actionZoomOut, SIGNAL(triggered()), ui->treeView, SLOT(zoomOut()));
  connect(ui->actionAbout, SIGNAL(triggered()), this->about, SLOT(show()));
}

int VisTrailsWindow::getSortIndex() {
  return ui->tbView->horizontalHeader()->sortIndicatorSection();
}

Qt::SortOrder VisTrailsWindow::getSortOrder() {
  return ui->tbView->horizontalHeader()->sortIndicatorOrder();  
}

void VisTrailsWindow::updateSortIndicator(int idx, Qt::SortOrder order) {
  ui->tbView->horizontalHeader()->setSortIndicator(idx, order);
}

void VisTrailsWindow::updateAscendingAction(Qt::SortOrder order) {
  if (order == Qt::AscendingOrder) {
    this->sortAscending->setText("Sort Descending");
    ui->actionSort->setIcon(QIcon(":/images/sort_az.png"));
  } else {
    this->sortAscending->setText("Sort Ascending");
    ui->actionSort->setIcon(QIcon(":/images/sort_za.png"));
  }
}

void VisTrailsWindow::updateActionIndex(int idx) {
  this->sortActionGroup->checkedAction()->setChecked(false);
  if (idx < 1) {
    this->sortActionGroup->actions()[0]->setChecked(true);
  } else {
    this->sortActionGroup->actions()[idx]->setChecked(true);
  }
}

void VisTrailsWindow::tableSortChanged(int idx, Qt::SortOrder order) {
  this->updateActionIndex(idx);
  this->updateAscendingAction(order);
}

void VisTrailsWindow::sortTypeChanged(QAction *action) {
  int index = -1;
  if (action->text() == "") {
    index = -1;
  } else if (action->text() == "Tag") {
    index = 1;
  } else if (action->text() == "Action") {
    index = 2;
  } else if (action->text() == "Date") {
    index = 3;
  } else if (action->text() == "User") {
    index = 4;
  }
  this->updateSortIndicator(index, this->getSortOrder());
}

void VisTrailsWindow::sortAscendingChanged(bool checked=false) {
  Qt::SortOrder order;
  if (this->getSortOrder() == Qt::AscendingOrder) {
    order = Qt::DescendingOrder;
  } else {
    order = Qt::AscendingOrder;
  }
  this->updateAscendingAction(order);
  this->updateSortIndicator(this->getSortIndex(), order);
}

// void VisTrailsWindow::sortComboChanged() { 
//   int index = ui->cbSort->currentIndex();
//   Qt::SortOrder order;
//   if (ui->checkAscending->isChecked())
//     order = Qt::AscendingOrder;
//   else
//     order= Qt::DescendingOrder;    
//   if (index == 0)
//     vtSortFilterProxy->sort(3, Qt::AscendingOrder);
//   else
//     vtSortFilterProxy->sort(index, order);
// }

// void VisTrailsWindow::showSort() {
//   if (ui->actionSort->isChecked()) {
//     ui->sortGroup->setVisible(true);
//     ui->cbSort->setFocus();
//   } else {
//     ui->sortGroup->setVisible(false);
//   }
// }

/*
  setImageViewer(QString filename)
  @args filename
  Controls the mini-image visualizer on the top right
*/
void VisTrailsWindow::setImageViewer(QString filename)
{
  QPixmap dirPixImage(filename);
  if (!dirPixImage.isNull()) {
    double aspectRatio = (double) dirPixImage.width() / dirPixImage.height();
    ui->dirImage->setAspectRatio(aspectRatio);
    ui->dirImage->setMaximumSize(dirPixImage.width(), dirPixImage.height());
    ui->dirImage->setMinimumSize(240, 240 / aspectRatio);
    ui->dirImage->setPixmap(dirPixImage);
  }
}

/*
  versionAdded()
  This function is called when a new version is added to the vistrail
*/
void VisTrailsWindow::versionAdded(vt::Version *version)
{
  this->pausePlayback();
  this->resetFilter();
  this->unsavedWork = true;
  vtModel->addVersion(version);

  //only update the current view, others are updated upon being viewed
  switch(this->getCurrentViewIndex())
    {
    case 0: break; //tree view handles versionAdded on its own
    case 1: ui->tbView->update(); break;
    case 2: ui->listView->update(); break;
    case 3: this->updateInfoView(); break;
    }

  this->versionSelected(vt::VERSION_INVALID, version->id());
}

void VisTrailsWindow::setLoadFileCallBack(CallBackString function)
{
  this->loadFileCallBack = function;
}


void VisTrailsWindow::setSaveFileCallBack(CallBackString function)
{
  this->saveFileCallBack = function;
}


void VisTrailsWindow::setNewFileCallBack(CallBackVoid function)
{
  this->newFileCallBack = function;
}


void VisTrailsWindow::setSessionFocus(vt::SessionFocus *focus)
{
  this->sessionFocus = focus;
  focus->addHandler(this);

  QString user = settings->value("username", "<unknown>").toString();
  focus->session()->info()->setString("user", user.toUtf8().constData());

  ui->treeView->setSessionFocus(focus);
  this->sessionFocus->addHandler(ui->treeView->getTreeScene());
  this->sessionFocus->addHandler(ui->treeView->getVersionProp());

  this->vtModel->setVistrail(focus->session()->vistrail());

  //this->initTreeView();
}

void VisTrailsWindow::setThumbnailDir(QString dir)
{
  this->vtModel->setThumbnailDir(dir);
}

QString VisTrailsWindow::thumbnailDir()
{
  return this->vtModel->thumbnailDir();
}

void VisTrailsWindow::setThumbnailLength(int length)
{
  //this->vtModel->setThumbnailLength(length);
}

int VisTrailsWindow::thumbnailLength()
{
  //return this->vtModel->thumbnailLength();
  return 100;
}

vt::SessionFocus *VisTrailsWindow::getSessionFocus() 
{
  return this->sessionFocus;
}

QSettings *VisTrailsWindow::getSettings() 
{
  return this->settings;
}

void VisTrailsWindow::versionSelected(const vt::VersionId &prev, 
                                 const vt::VersionId &id)
{
  this->ignoreChanged = true;
  vt::Vistrail* vistrail = this->sessionFocus->session()->vistrail();
  QModelIndex idx = this->vtModel->index(vistrail->getVersion(id));

  if (idx.isValid()) {
    QModelIndex tableIdx = this->vtTableProxy->mapFromSource(idx);
    QModelIndex sortIdx = this->vtSortFilterProxy->mapFromSource(tableIdx);
    if (sortIdx.isValid()) {
      vtSelectionModel->setCurrentIndex(sortIdx, 
					QItemSelectionModel::Clear |
					QItemSelectionModel::Select |
					QItemSelectionModel::Rows);
    }
  }

  this->ignoreChanged = false;

  // DAK not sure why we only update the view when it is visible...
  // if(this->getCurrentViewIndex() == 4) // info view
  this->updateInfoView();
}

void VisTrailsWindow::updateInfoView()
{
  vt::VersionId id = this->sessionFocus->currentVersion();
  vt::Vistrail* vistrail = this->sessionFocus->session()->vistrail();
  vt::Version * version = vistrail->getVersion(id);
  if(version)
    {
      ui->versionId->setText(QString(id.toString().asChar()));
      QString parent(version->parent().toString().asChar());
      ui->versionParent->setText(parent);
      QString tag = getVal(version->annotations(), "tag", "");
      ui->versionTag->setText(tag);
      ui->versionTag->setEnabled(true);
      QString act = getVal(version->annotations(), "label", "<unknown>");
      QString usr = getVal(version->stamp(), "user", "<unknown>");
      ui->versionUser->setText(usr);

      time_t timet = version->stamp()->get("date")->asDateTime()->toTime();
      QDateTime qtime = QDateTime::fromTime_t(timet);
      qtime.setTimeSpec(Qt::UTC);
      QString time = qtime.toLocalTime().toString("yyyy-M-d hh:mm:ss ap");
      ui->versionDate->setText(time);

      QString tmpThumbnail = getVal(version->annotations(), "thumb", "");
      tmpThumbnail = thumbnailDir()+tmpThumbnail;
      this->setImageViewer(tmpThumbnail);
    }
  else
    {
      ui->versionId->setText(QString(id.toString().asChar()));
      ui->versionParent->setText(
				 QString(vt::VERSION_INVALID.toString().asChar()));
      ui->versionUser->setText("");
      ui->versionTag->setText("");
      ui->versionTag->setEnabled(false);
      ui->versionDate->setText("");
    }
}

void VisTrailsWindow::tagChanged()
{
  vt::VersionId id = this->sessionFocus->currentVersion();

  if(id != vt::VERSION_INVALID && id != vt::VERSION_ROOT)
  {
    vt::Vistrail* vistrail = this->sessionFocus->session()->vistrail();      
    QModelIndex idx = vtModel->index(vistrail->getVersion(id));
    vtModel->setData(vtModel->index(idx.row(), VersionData::Tag, idx.parent()),
                     ui->versionTag->text());
    this->sessionFocus->versionChanged(id);
  }
}

/*
  Update image
*/
void VisTrailsWindow::displayDirItem(const QModelIndex &index)
{ }


void VisTrailsWindow::saveFile(const char * filePath)
{
  if(this->saveFileCallBack != NULL)
    this->saveFileCallBack(filePath);
}


void VisTrailsWindow::loadFile(const char * filePath)
{
  if(this->loadFileCallBack != NULL)
    this->loadFileCallBack(filePath);
}


/*
  zoomOut()
  Currently this functions enables zooming out
  of image viewer on the directory view.
*/
void VisTrailsWindow::zoomOut()
{
  scaleImage(0.8);
}


/*
  zoomIn()
  Currently this functions enables zooming out
  of image viewer on the directory view.
*/
void VisTrailsWindow::zoomIn()
{
  scaleImage(1.25);
}


/*
  adjustScrollBar()
  @param scrollBar
  @param factor
  The directory view image viewer zoom is based on the scrollbar
*/
void VisTrailsWindow::adjustScrollBar(QScrollBar *scrollBar, double factor)
{
  scrollBar->setValue(int(factor * scrollBar->value()
			  + ((factor - 1) * scrollBar->pageStep()/2)));
}


/*
  scaleImage()
  @param factor
  Scale image according to the scale factor var.
*/
void VisTrailsWindow::scaleImage(double factor)
{
  scaleFactor *= factor;
  adjustScrollBar(dirScrollArea->horizontalScrollBar(), factor);
  adjustScrollBar(dirScrollArea->verticalScrollBar(), factor);
}


/*
  newFile()
  Opens new file
*/
void VisTrailsWindow::newFile()
{
  if(!this->promptForSave())
    return;

  this->resetViews();

  if(this->newFileCallBack != NULL)
    this->newFileCallBack();

  this->sessionFocus->vistrailChanged();
}


/*
  save()
  Save current File
*/
bool VisTrailsWindow::save()
{
  QString filename;
  QString directory(filename.isEmpty() ? QString(".") : QFileInfo(filename).canonicalPath());
  filename = QFileDialog::getSaveFileName(this,
					  tr("%1 - Save").arg(QApplication::applicationName()),
					  directory,
					  tr("%1 (*.vt)").arg(QApplication::applicationName()));

  if(!filename.isEmpty())
  {
    if(QFileInfo(filename).suffix() != "vt")
      filename.append(".vt");
    this->saveFile(filename.toUtf8().constData());
    this->unsavedWork = false;
    return true;
  }
  else
    return false;
}

VtModel *VisTrailsWindow::getModel()
{
  return this->vtModel;
}

QVersionTreeScene *VisTrailsWindow::getTreeScene()
{
  return ui->treeView->getTreeScene();
}

/*
  promptForSave()
*/
bool VisTrailsWindow::promptForSave(bool includeCancelButton /* = true */)
{
  if(!this->unsavedWork)
    return true;

  QFlags<QMessageBox::StandardButton> buttons
    = QMessageBox::Save | QMessageBox::Discard;
  if(includeCancelButton)
    buttons |= QMessageBox::Cancel;

  QMessageBox msgBox(
		     QMessageBox::Question, 
		     "Save?", 
		     "Do you want to save the changes you've made to this vistrail?",
		     buttons);
  int result = msgBox.exec();

  if(result == QMessageBox::Save)
    {
      if(!this->save())
	return false;
    }
  else if(result == QMessageBox::Cancel)
    return false;

  this->unsavedWork = false;
  return true;
}

void VisTrailsWindow::currentRowChanged(QModelIndex current, QModelIndex previous)
{
  if(this->ignoreChanged)
    return;

  if (current.isValid()) {
    QModelIndex tableIdx = vtSortFilterProxy->mapToSource(current);
    if (tableIdx.isValid()) {
      VtNode *node = static_cast<VtNode*>(tableIdx.internalPointer());
      this->sessionFocus->selectVersion(node->getVersion()->id());
      versionSelected(vt::VERSION_INVALID, node->getVersion()->id());
    }
  }
}

/*
  nextItem()
  Selects next Item
*/
void VisTrailsWindow::nextItem()
{
  this->sessionFocus->selectRedoChild();
}


/*
  previousItem()
  Selects previous item
*/
void VisTrailsWindow::previousItem()
{
  this->sessionFocus->selectParent();
}


/*
  loadFile()
  Current I load a *.vt file here
*/
void VisTrailsWindow::loadFile()
{
  if(!this->promptForSave())
    return;

  QString directory(".");
  QString filename = QFileDialog::getOpenFileName(this,
						  tr("%1 - Open").arg(QApplication::applicationName()),
						  directory,
						  tr("%1 (*.vt)").arg(QApplication::applicationDirPath()));
  if(!filename.isEmpty())
    {
      this->resetViews();
      this->loadFile(filename.toUtf8().constData());
      this->unsavedWork = false;
      this->sessionFocus->vistrailChanged();
    }    
}

/*
  openFile()
  Temporary calls load file here
*/
void VisTrailsWindow::openFile()
{
  loadFile();
}

void VisTrailsWindow::resetViews()
{
  this->ignoreChanged = true;

  this->vtSelectionModel->clearSelection();
  this->vtSortFilterProxy->clearModel(); // this clears source models as well
  ui->treeView->getTreeScene()->clear();
  unsavedWork = false;
    
  this->ignoreChanged = false;
}

bool VisTrailsWindow::dateInRange(QDateTime date) {    
    if ((ui->dtFrom->dateTime() <= date) && (ui->dtTo->dateTime() >= date))
	return true;
    return false;
}

void VisTrailsWindow::updateFilter() 
{
  this->vtSelectionModel->clearSelection();
  int comboIdx = ui->filterTypeCombo->currentIndex();
  if (comboIdx == 0) {
    this->vtSortFilterProxy->setFilterKeyColumn(-1);
  } else {
    this->vtSortFilterProxy->setFilterKeyColumn(comboIdx);
  }
  this->vtSortFilterProxy->setFilterWildcard(ui->filterText->text());
}

void VisTrailsWindow::updateFilterType(int index)
{
  if (index == 3) {
      vtSortFilterProxy->setDate(true);
      ui->lblFrom->show();
      ui->lblTo->show();
      ui->dtFrom->show();
      ui->dtTo->show();
      ui->filterText->hide();
  }
  else {
      vtSortFilterProxy->setDate(false);
      ui->filterText->show();
      ui->lblFrom->hide();
      ui->lblTo->hide();
      ui->dtFrom->hide();
      ui->dtTo->hide();
  }
  //this->updateFilter();
}

void VisTrailsWindow::resetFilter()
{
  vtSortFilterProxy->setDate(false);
  this->bFiltering = false;
  ui->filterText->setText("");
  ui->filterTypeCombo->setCurrentIndex(0);
  this->updateFilter();
}

/*
  VisTrailsWindow::filterChecked()
  This function is called when user
  selects/clicks on the filter option.
*/
void VisTrailsWindow::filterChecked()
{
  if (ui->actionFind->isChecked()) {
    ui->filterGroup->setVisible(true);
    this->updateFilterType(ui->filterTypeCombo->currentIndex());
    ui->filterText->setFocus();
  } else {
    ui->filterGroup->setVisible(false);
  }
}

// called when the filter button is clicked
void VisTrailsWindow::filterClicked()
{
  this->bFiltering = true;
  VistrailController *controller = this->getTreeScene()->controller;
  controller->clearFilter();
  QString text = ui->filterText->text();
  switch(ui->filterTypeCombo->currentIndex())
  {
  case 0: //filter all
    controller->filterAnnotation("tag", text);
    controller->filterAnnotation("label", text);
    controller->filterStamp("user", text);
    break;
  case 1: //filter tag
    controller->filterAnnotation("tag", text);
    break;
  case 2: //filter action
    controller->filterAnnotation("label", text);
    break;
  case 3: //filter date
    controller->filterDate(ui->dtFrom->dateTime(), ui->dtTo->dateTime());
    break;
  case 4: //filter user
    controller->filterStamp("user", text);
    break;
  }
  this->getTreeScene()->setupScene();
  this->updateFilter();
}

/*
  openPreferences()
  Open the preferences dialog
*/
void VisTrailsWindow::openPreferences()
{
  PreferencesDialog* preferences = PreferencesDialog::getInstance();
  preferences->show();
}

void VisTrailsWindow::setViewToTable()
{
  ui->tbView->update();
  ui->stackedWidget->setCurrentIndex(1);
}

void VisTrailsWindow::setViewToDir()
{
  ui->listView->update();
  ui->stackedWidget->setCurrentIndex(2);
}

void VisTrailsWindow::setViewToTree()
{
  ui->treeView->getTreeScene()->setupScene();
  ui->treeView->getVersionProp()
    ->updateVersion(this->sessionFocus->currentVersion());
  ui->stackedWidget->setCurrentIndex(0);
}

void VisTrailsWindow::setViewToInfo()
{
  this->updateInfoView();
  ui->stackedWidget->setCurrentIndex(3);
  if (ui->actionSort->isChecked())
    ui->actionSort->setChecked(false);
  if (ui->actionFind->isChecked())
    ui->actionFind->setChecked(false);
  filterChecked();
  //showSort();
}

void VisTrailsWindow::closeEvent(QCloseEvent *e)
{
  // check if user wants to save unsaved work
  if(!promptForSave())
    {
      e->ignore();
      return;
    }

  // save window position and size
  settings->setValue("windowSize", size());
  settings->setValue("windowPos", pos());

  e->accept();
}

// this is called when an item in vtmodel is changed
// we update the value in the version and signal versionChanged
void VisTrailsWindow::itemChanged(QStandardItem *item)
{
  QModelIndex index = item->index();
  vt::Version *version = this->vtModel->getVersion(item->index());
  QString data = item->text();
  if(data.isEmpty())
    return; 

  QDateTime qtime;
  switch(item->column())
    {
    case 0:
      version->annotations()->setString("thumb", data.toUtf8().constData());
      break;
    case 1:
      version->annotations()->setString("tag", data.toUtf8().constData());
      break;
    case 2:
      version->annotations()->setString("label", data.toUtf8().constData());
      break;
    case 3: 
      qtime = QDateTime::fromString(data, "yyyy-M-d hh:mm:ss ap");
      qtime.setTimeSpec(Qt::LocalTime);
      version->stamp()->setDateTime("date", qtime.toUTC().toTime_t());
      break;
    case 4:
      version->stamp()->setString("user", data.toUtf8().constData());
      break;
    }

  this->ignoreChanged = true;
  VisTrailsWindow::getInstance()->getSessionFocus()
    ->versionChanged(version->id());
  this->ignoreChanged = false;
}

void VisTrailsWindow::versionChanged(const vt::VersionId &id)
{
  if(!this->ignoreChanged)
    {
/*
      vt::Vistrail *vistrail = this->sessionFocus()->session()->vistrail();
      vt::Version *version = vistrail->getVersion(id);
      QModelIndex index = this->vtModel->index(version);

      vt::PDict *anns = version->annotations();
      vt::PDict *stamp = version->stamp();

      QString versionId(version->id().toString().asChar());
      QString thumbnail = getVal(anns, "thumb", "");
      QString tag = getVal(anns, "tag", "<untagged>");
      QString action = getVal(anns, "label", "<unknown>");
      QString user = getVal(stamp, "user", "<unknown>");

      time_t timet = version->stamp()->get("date")->asDateTime()->toTime();
      QDateTime qtime = QDateTime::fromTime_t(timet);
      qtime.setTimeSpec(Qt::UTC);
      QString time = qtime.toLocalTime().toString("yyyy-M-d hh:mm:ss ap");
      ui->versionDate->setText(time);
    
      QIcon thumbIcon;
      if (!thumbnail.isEmpty()) {
	QPixmap pixThumb(this->vtModel->thumbnailDir() + thumbnail);
	pixThumb = pixThumb.scaled(this->vtModel->thumbnailLength(), 
				   this->vtModel->thumbnailLength(), 
				   Qt::KeepAspectRatio, 
				   Qt::SmoothTransformation);
	int height = pixThumb.height();
	int width = pixThumb.width();
	int x_offset = 0;
	int y_offset = 0;
	if (height > width) {
	  x_offset = (this->vtModel->thumbnailLength() - width)/2;
	} else {
	  y_offset = (this->vtModel->thumbnailLength() - height)/2;
	}
	QPixmap pixCenter(this->vtModel->thumbnailLength(), 
			  this->vtModel->thumbnailLength());
	pixCenter.fill(Qt::transparent);
	QPainter painter(&pixCenter);
	painter.drawPixmap(x_offset, y_offset, pixThumb);
	thumbIcon.addPixmap(pixCenter);
      } 

      QList<QStandardItem *> items;
      items << new QStandardItem(thumbIcon, "")
	    << new QStandardItem(tag)
	    << new QStandardItem(action) 
	    << new QStandardItem(time) 
	    << new QStandardItem(user);

      items[0]->setData(versionId, VtModel::UUID_ROLE);

      for(int i = 0; i < 5; i++)
	{
	  QModelIndex colIdx = 
	    this->vtModel->index(index.row(), i, index.parent());
	  QStandardItem *item = this->vtModel->itemFromIndex(colIdx);
	  item->setText(items[i]->text());
	}
*/
    }
}

// rebuilds model from vistrail
void VisTrailsWindow::vistrailChanged()
{
  this->ignoreChanged = true;

  // clear model
  this->vtSelectionModel->clearSelection();
  this->vtSortFilterProxy->clearModel(); // clears source models as well

  vt::Vistrail *vistrail = this->sessionFocus->session()->vistrail();
  vtModel->setVistrail(vistrail);

  // set selection
  this->versionSelected(vt::VERSION_INVALID, 
    this->sessionFocus->currentVersion());
    
  this->ignoreChanged = false;
}

VersionContextMenu *VisTrailsWindow::getVersionContextMenu()
{
  return this->versionContextMenu;
}

void VisTrailsWindow::selectPlaybackVersions(bool select)
{
  if(select)
  {
    this->playbackSelectionMode = SelectPlaybackStart;
    ui->lblSelectStart->setVisible(true);
    ui->cancelSelectButton->setVisible(true);
    ui->playbackSlider->setVisible(false);
  }
  else
    this->cancelPlaybackSelection();
}

void VisTrailsWindow::setPlaybackStart(vt::VersionId id)
{
  this->playbackStart = id;
  VistrailController *ctrl = this->getTreeScene()->controller;
  ctrl->setPlaybackStart(id);
  if(this->playbackSelectionMode == SelectPlaybackStart)
  {
    this->playbackSelectionMode = SelectPlaybackEnd;
    ui->lblSelectStart->setVisible(false);
    ui->lblSelectEnd->setVisible(true);
  }
  if(this->playbackEnd != vt::VERSION_INVALID)
    updatePlaybackPath();
}

void VisTrailsWindow::setPlaybackEnd(vt::VersionId id)
{
  this->playbackEnd = id;
  VistrailController *ctrl = this->getTreeScene()->controller;
  ctrl->setPlaybackEnd(id);
  this->cancelPlaybackSelection();
  updatePlaybackPath();
}

void VisTrailsWindow::updatePlaybackPath()
{
  this->sessionFocus->session()->vistrail()->getVersionPath(
    this->playbackStart, this->playbackEnd, this->playbackPath);
  int max = this->playbackPath->count();
  ui->playbackSlider->setRange(1, max);
  ui->lblPlayBackCount->setText(QString("1/%1").arg(max));
}

void VisTrailsWindow::playbackSliderChanged(int value)
{
  if(this->playbackPath->count() >= value)
  {
    vt::Version *version = this->playbackPath->get(value-1);
    if(version)
      this->sessionFocus->selectVersion(version->id());
    else
      this->sessionFocus->selectVersion(vt::VERSION_ROOT);
    ui->lblPlayBackCount->setText(
      QString("%1/%2").arg(value).arg(this->playbackPath->count()));
  }
}

void VisTrailsWindow::playbackNextVersion()
{
  //VisTrailsWindow gets disabled while actions are being replayed, prevent
  // the next step from happening until it is re-enabled
  if(this->isEnabled())
  {
    int value = ui->playbackSlider->value();
    int max = ui->playbackSlider->maximum();
    if(value < max)
      ui->playbackSlider->setValue(value + 1);
    else
      this->pausePlayback();
  }
}

void VisTrailsWindow::playbackToggled(bool checked)
{
  this->getTreeScene()->controller->playback = checked;
  ui->playGroup->setVisible(checked);
  if(checked)
  {
    pausePlayback();
    cancelPlaybackSelection();
  }
}

void VisTrailsWindow::playPauseToggled(bool checked)
{
  if(checked)
  {
    int ms = 0;
    QString actionText = this->playSpeedGroup->checkedAction()->text();
    if(actionText == "Slow") 
      ms = 3000;
    else if(actionText == "Medium") 
      ms = 1000;
    else if(actionText == "Fast")
      ms = 500;

    if(ui->playbackSlider->value() == this->playbackPath->count())
      ui->playbackSlider->setValue(1);

    if(ms)
      this->playbackTimer->start(ms);
  }
  else
    pausePlayback();
}

void VisTrailsWindow::pausePlayback()
{
  this->playbackTimer->stop();
  if(ui->playPauseButton->isChecked())
    ui->playPauseButton->setChecked(false);
}

void VisTrailsWindow::stopPlayback()
{
  pausePlayback();
  ui->playbackSlider->setValue(1);
}

PlaybackSelectionMode VisTrailsWindow::getPlaybackSelectionMode()
{
  return this->playbackSelectionMode;
}

void VisTrailsWindow::setPlaybackSelectionMode(PlaybackSelectionMode mode)
{
  if(mode == NotSelectingPlayback)
    cancelPlaybackSelection();
  else
    this->playbackSelectionMode = mode;
}

void VisTrailsWindow::cancelPlaybackSelection()
{
  this->playbackSelectionMode = NotSelectingPlayback;
  ui->lblSelectStart->setVisible(false);
  ui->lblSelectEnd->setVisible(false);
  ui->cancelSelectButton->setVisible(false);
  ui->playbackSlider->setVisible(true);
  if(ui->selectVersionsButton->isChecked())
    ui->selectVersionsButton->toggle();
}

const vt::VersionId & VisTrailsWindow::getPlaybackStart()
{
  return this->playbackStart;
}

const vt::VersionId & VisTrailsWindow::getPlaybackEnd()
{
  return this->playbackEnd;
}

bool VisTrailsWindow::playback()
{
  return ui->playGroup->isVisible();
}

void VisTrailsWindow::pruneVersion(const vt::VersionId &id)
{
  //if currentversion is descendant, load parent id
  bool found = false;
  vt::VersionId temp = this->sessionFocus->currentVersion();
  vt::Vistrail *vistrail = this->sessionFocus->session()->vistrail();
  vt::VersionTree *tree = vistrail->versionTree();
  while(temp != vt::VERSION_INVALID)
  {
    if(temp == id)
      found = true;
    temp = tree->getParent(temp);
  }
  if(found)
    this->sessionFocus->selectVersion(tree->getParent(id));

  // prune
  this->sessionFocus->prune(id);
}

/*
  ~VisTrailsWindow()
  Destructor
*/
VisTrailsWindow::~VisTrailsWindow()
{ 
  delete vtSortFilterProxy;
  delete vtTableProxy;
  delete vtModel;
  delete ui;
}
