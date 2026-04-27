/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

// """ This file describe a widget for keeping version notes,info and tag
// name

// QVersionProp
// QVersionNotes
// QVersionPropOverlay
// QExpandButton
// QNotesDialog

#ifndef __VERSIONPROP_H__
#define __VERSIONPROP_H__

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialog>
#include <QDateTime>
#include <QComboBox>
#include "VistrailController.h"
#include "CommonWidgets.h"

class QVersionNotes;
class QVersionEmbed;
class QExpandButton;
class QNotesDialog;

// ################################################################################

// QVersionProp is a widget holding property of a version including
// tagname and notes
class QVersionProp : public QWidget, public QToolWindowInterface
{
  Q_OBJECT

public:
  // Initialize the main layout
  QVersionProp(QWidget *parent = NULL);
  ~QVersionProp();

  // Assign the controller to the property page
  void updateController(VistrailController *controller);

  // Update the property page of the version
  void updateVersion(const vt::VersionId &id);

public slots:
  // Update the new tag to vistrail
  void tagFinished();

  // Update the button state if there is text
  void tagChanged(QString text);

  // Remove the tag
  void tagCleared();

  // resetSearch() -> None
  void resetSearch(bool emit_signal = true);

  void executeSearch(QString text);

  void refineMode(bool on);

private:
  QVBoxLayout *vLayout;
  QGridLayout *gLayout;
  QLabel *tagLabel;
  QHBoxLayout *editLayout;
  QLabel *userLabel;
  QLabel *dateLabel;

  QSearchBox *searchBox;
  QLineEdit *tagEdit;
  QToolButton *tagReset;
  QLabel *userEdit;
  QLabel *dateEdit;
  QLabel *notesLabel;
  QVersionNotes *versionNotes;
  QVersionEmbed *versionEmbed;
  VistrailController *controller;
  vt::VersionId id;
};

// QVersionNotes is text widget that update/change a version notes
class QVersionNotes: public QTextEdit
{
public:
  // Initialize control variables
  QVersionNotes(QWidget *parent = NULL);

  // Update the text to be the notes of the vistrail versionNumber
  void updateVersion(const vt::VersionId &id);

  void commit_changes();

  void reset_changes();

  // Update the version notes if the text has been modified
  void focusOutEvent(QFocusEvent *event);

  void trim_first_paragraph();

  VistrailController *controller;
  
  bool update_on_focus_out;

private:
  vt::VersionId id;
};


// ################################################################################

// QVersionPropOverlay is a transparent widget that sits on top of the version
// view.  It displays properties of a version: tag, user, date, and notes.
class QVersionPropOverlay: public QFrame, public vt::HNotify
{
  Q_OBJECT

public:        
  // setup layout
  QVersionPropOverlay(QWidget *parent = NULL, QWidget *mouseWidget = NULL);
  ~QVersionPropOverlay();

  // Keep in upper left of screen
  void updateGeometry();

  // Assign the controller to the properties
  void updateController(VistrailController *controller);

  // Update the text items
  void updateVersion(const vt::VersionId &id);

  // Remove HTML tags and newlines
  QString convertHtmlToText(QString str);

  // Shorten string to fit in smaller space
  QString truncate(QString str);

  //int extract_file(QString filename, QString extractDir, QString zipFilename, QString mediaType);

  //void openMultimedia();

  //void deleteAudio();

  //void loadPhonon();

  //void loadAudio();

  //void playAudio();

  void versionAdded(vt::Version *version);
  void versionSelected(const vt::VersionId &previous,
    const vt::VersionId &current);
  void versionChanged(const vt::VersionId &id);

public slots:
  void openNotes();

private:
  QWidget *propagatingWidget;
  QPalette *palette;
  QGridLayout *layout;
  QLabel *tag_label;
  QLabel *tag;
  QHBoxLayout *tag_layout;
  QLabel *description_label;
  QLabel *description;
  QHBoxLayout *description_layout;
  QLabel *user_label;
  QLabel *user;
  QHBoxLayout *user_layout;
  QLabel *date_label;
  QLabel *date;
  QHBoxLayout *date_layout;
  QLabel *notes_label;
  QLabel *notes;
  QHBoxLayout *notes_layout;
  QExpandButton *notes_button;
  QLabel *details_label;
  QLabel *details;
  QHBoxLayout *details_layout;
  QExpandButton *details_button;
  QNotesDialog *notes_dialog;
  VistrailController *controller;
  int pos_x;
  int pos_y;
};

// ################################################################################
// A transparent button type with a + draw in
class QExpandButton: public QLabel
{
  Q_OBJECT

public:
  QExpandButton(QWidget *parent = NULL);

  QSize sizeHint();

  // Capture mouse press event on the frame to move the widget
  void mousePressEvent(QMouseEvent *e);

  void mouseReleaseEvent(QMouseEvent *e);

  // Custom draw function
  void drawButton(bool down);

signals:
  void pressed();

private:
  QPicture picture;
};

//// ###############################################################################
//class QOpenVideo: public QExpandButton
//{
//public:
//  QOpenVideo();
//};
//
//// ################################################################################
//class QOpenMultimediaButton: public QExpandButton
//{
//public:
//  QOpenMultimediaButton();
//};
//
//class QPlayButton: public QExpandButton
//{
//public:
//  QPlayButton();
//
//  // Custom draw function
//  void drawButton(bool down);
//};
//
//// ################################################################################
//class QDeleteButton: public QExpandButton
//{
//public:
//  QDeleteButton();
//
//  // Custom draw function
//  void drawButton(bool down);
//};
//
//class QDeleteVideoButton: public QDeleteButton
//{
//public:
//  QDeleteVideoButton();
//};
//
//// """ Just for the paper """
//// # class QCaptureCameraMovement(QExpandButton):
//// #     def __init__(self):
//// #         QExpandButton.__init__(self)
//// #         self.setToolTip('Capture Keyboard Interation')
//
//class QStopButton: public QExpandButton
//{
//public:
//  QStopButton();
//
//  // Custom draw function
//  void drawButton(bool down);
//};
//
//class QPauseButton: public QExpandButton
//{
//public:
//  QPauseButton();
//
//  // Custom draw function
//  void drawButton(bool down);
//};

// ################################################################################
// A small non-modal dialog with text entry to modify and view notes
class QNotesDialog: public QDialog
{
  Q_OBJECT

public:
  QNotesDialog(QWidget *parent = NULL);
  ~QNotesDialog();

  void updateController(VistrailController *controller);

  void updateVersion(const vt::VersionId &id);

  QSize sizeHint();
  
public slots:
  void apply_pressed();

  void ok_pressed();

  void cancel_pressed();

private:
  QVersionNotes *notes;
  QVBoxLayout *layout;
  QPushButton *apply_button;
  QPushButton *ok_button;
  QPushButton *cancel_button;
  QHBoxLayout *buttonLayout;
  VistrailController *controller;
};

// ################################################################################
class QVersionEmbed: public QWidget
{
  Q_OBJECT

public:
  QVersionEmbed(QWidget *parent = NULL);
  ~QVersionEmbed();

  // Only vistrails on a database are allowed to embed a tag
  bool check_version();

  // this checks if the controller has saved the latest changes 
  bool check_controller_status();

  void updateEmbedText();

  void updateVersion(const vt::VersionId &id);
  
  VistrailController *controller;

public slots:
  void copy_to_clipboard();

  void change_embed_type(QString text);

private:
  QLabel *label1;
  QComboBox *cbtype;
  QLineEdit *embededt;
  QLabel *copylabel;
  QGridLayout *layout;

  QString wikitag;
  QString latextag;
  vt::VersionId id;
};

#endif
