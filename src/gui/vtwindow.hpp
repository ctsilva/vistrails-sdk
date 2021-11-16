/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VISTRAILSWINDOW_HPP
#define VISTRAILSWINDOW_HPP

#include <iostream>
#include <QtGui/QMainWindow>
#include <QtGui>
#include "VisTrails.h"
#include "tree/VersionView.h"
#include "versionedit.hpp"

#if defined (_WIN32) 
#if defined(VisTrailsGUI_EXPORTS)
#define  VT_EXPORT __declspec(dllexport)
#else
#define  VT_EXPORT __declspec(dllimport)
#endif 
#else 
#define VT_EXPORT
#endif

class imgGallery;
class VtModel;
class VtTableProxyModel;
class VtSortFilterProxy;

typedef void (*CallBackString)(const char *);
typedef void (*CallBackVoid)(void);

namespace Ui {
  class VisTrailsMainWindow;
}

enum SavePreference
{
  AlwaysPrompt,
  AlwaysInclude,
  NeverInclude
};

enum PlaySpeed
{
  Slow,
  Medium,
  Fast
};

enum PlaybackSelectionMode
{
  NotSelectingPlayback,
  SelectPlaybackStart,
  SelectPlaybackEnd
};

class VT_EXPORT VisTrailsWindow : public QMainWindow, public vt::HNotify
{
  Q_OBJECT

    public:

  explicit VisTrailsWindow(QWidget *parent =0);
  void setImageViewer(QString filename);

  //void setVersionSelectedCallBack(CallBackString function);
  void setLoadFileCallBack(CallBackString function);
  void setSaveFileCallBack(CallBackString function);
  void setNewFileCallBack(CallBackVoid function);
  // set focus, set session user name from settings, init tree view
  void setSessionFocus(vt::SessionFocus *focus);
  void setThumbnailDir(QString dir);
  QString thumbnailDir();
  void setThumbnailLength(int length);
  int thumbnailLength();

  void saveFile(const char * filePath);
  void loadFile(const char * filePath);
  void displayDirItem(const QModelIndex &index);
  //void selectVersion(const vt::VersionId &id);

  vt::SessionFocus *getSessionFocus();
  QSettings *getSettings();
  // void reportVersion(vt::Version *version);
    
  imgGallery * getGraphicsView();
  QStackedWidget * getTabWidget();
  QListView * getListView();
  QTableView *getTableView();
  int getCurrentViewIndex();
  bool dateInRange(QDateTime date);
    
  ~VisTrailsWindow();

  bool bFiltering;
  QImage *streamImage;
  static VisTrailsWindow* getInstance();    

  // Returns true if users select "Save" or "Don't Save", false if 
  // includeCancelButton is true and they select "Cancel".
  bool promptForSave(bool includeCancelButton = true);

  VtModel *getModel();
  QVersionTreeScene *getTreeScene();
  VersionContextMenu *getVersionContextMenu();

    
  void versionSelected(const vt::VersionId &prev, 
		       const vt::VersionId &id);

  void versionAdded(vt::Version *version);

  void versionChanged(const vt::VersionId &id);
  void vistrailChanged();

  void setPlaybackStart(vt::VersionId id);
  void setPlaybackEnd(vt::VersionId id);
  const vt::VersionId & getPlaybackStart();
  const vt::VersionId & getPlaybackEnd();
  PlaybackSelectionMode getPlaybackSelectionMode();
  void setPlaybackSelectionMode(PlaybackSelectionMode mode);
  bool playback();

  void pruneVersion(const vt::VersionId &id);
				    
 private slots:
  void zoomIn();
  void zoomOut();
  void loadFile();
  void openFile();
  void newFile();
  bool save();
  void nextItem();
  void previousItem();
  void updateFilter();
  void updateFilterType(int type);
  void resetFilter();
  void resetViews();
  void filterChecked();
  void filterClicked();
  void openPreferences();
  void setViewToTable();
  void setViewToDir();
  void setViewToTree();
  void setViewToInfo();
  void currentRowChanged(QModelIndex current, QModelIndex previous);
  void itemChanged(QStandardItem *item);
  void tagChanged();
  //void showSort();
  //void sortComboChanged();
  void sortTypeChanged(QAction * action);
  void sortAscendingChanged(bool ascending);
  void tableSortChanged(int idx, Qt::SortOrder order);
  void playbackToggled(bool checked = false);
  void playPauseToggled(bool checked = false);
  void pausePlayback();
  void stopPlayback();
  void selectPlaybackVersions(bool select);
  void playbackNextVersion();
  void playbackSliderChanged(int value);
  void cancelPlaybackSelection();
 private:
  void createMenuActions();
  void createConnections();

  void initModel();
  void initTableView();
  void initListView();
  void initTreeView();
  void adjustScrollBar(QScrollBar *scrollBar, double factor);
  void scaleImage(double factor);

  void loadViewerImages();
  void loadSettings();

  void updateInfoView();

  int getSortIndex();
  Qt::SortOrder getSortOrder();
  void updateSortIndicator(int idx, Qt::SortOrder order);
  void updateAscendingAction(Qt::SortOrder order);
  void updateActionIndex(int idx);
  
  void updatePlaybackPath();

  QPixmap dirPixImage;
  QAction *dirZoomInAct;
  QAction *dirZoomOutAct;
  QVBoxLayout *dirVerticalLayout;
  QScrollArea *dirScrollArea;
  QActionGroup *viewActionGroup;
 
  QActionGroup *sortActionGroup;
  QAction *sortAscending;

  QActionGroup *playSpeedGroup;

  vt::SessionFocus *sessionFocus;

  double scaleFactor;

  static VisTrailsWindow * instance;

  bool unsavedWork;

  VtModel *vtModel;
  VtTableProxyModel *vtTableProxy;
  VtSortFilterProxy *vtSortFilterProxy;
  QItemSelectionModel *vtSelectionModel;
	
  CallBackString loadFileCallBack;
  CallBackString saveFileCallBack;
  CallBackVoid newFileCallBack;

  Ui::VisTrailsMainWindow *ui;

  QSettings *settings;
  QVersionTreeView *treeView;

  QTimer *playbackTimer;
  vt::VersionId playbackStart;
  vt::VersionId playbackEnd;
  vt::VersionPath *playbackPath;

  bool ignoreChanged;
  VersionContextMenu *versionContextMenu;

  PlaybackSelectionMode playbackSelectionMode;

  QDialog *about;

 protected:
  void closeEvent(QCloseEvent *e);
};

#endif
