/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "VersionProp.h"
#include "Theme.h"
#include "vtwindow.hpp"
#include <QApplication>
#include <QClipboard>
#include <QTextDocumentFragment>

//propagatingEvent = set([
//  QtCore.QEvent.MouseButtonDblClick,
//  QtCore.QEvent.MouseButtonPress,
//  QtCore.QEvent.MouseButtonRelease,
//  QtCore.QEvent.MouseMove,
//  QtCore.QEvent.Wheel,
//])

inline QString getVal(vt::PDict *d, const char *key, const char *_default)
{
  vt::Property *p = d->get(key);
  if(p)
    return QString(p->asChar());
  return QString(_default);
}

// Initialize the main layout
QVersionProp::QVersionProp(QWidget *parent) : QWidget(parent)
{
  this->setWindowTitle("Properties");

  this->vLayout = new QVBoxLayout();
  vLayout->setContentsMargins(0, 0, 0, 0);
  vLayout->setSpacing(5);
  this->setLayout(vLayout);

  this->searchBox = new QSearchBox(this);
  vLayout->addWidget(this->searchBox);

  this->gLayout = new QGridLayout();
  gLayout->setContentsMargins(0, 0, 0, 0);
  gLayout->setSpacing(5);
  gLayout->setColumnMinimumWidth(1,5);
  gLayout->setRowMinimumHeight(0,24);
  gLayout->setRowMinimumHeight(1,24);
  gLayout->setRowMinimumHeight(2,24);
  gLayout->setRowMinimumHeight(3,24);
  vLayout->addLayout(gLayout);

  this->tagLabel = new QLabel("Tag:", this);
  gLayout->addWidget(tagLabel, 0, 0, 1, 1);

  this->editLayout = new QHBoxLayout();
  editLayout->setContentsMargins(0, 0, 0, 0);
  editLayout->setSpacing(2);
  this->tagEdit = new QLineEdit();
  tagLabel->setBuddy(this->tagEdit);
  editLayout->addWidget(this->tagEdit);
  this->tagEdit->setEnabled(false);

  this->tagReset = new QToolButton(this);
  QIcon icon = QIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
  this->tagReset->setIcon(icon);
  this->tagReset->setIconSize(QSize(12,12));
  this->tagReset->setAutoRaise(true);
  this->tagReset->setEnabled(false);
  editLayout->addWidget(this->tagReset);

  gLayout->addLayout(editLayout, 0, 2, 1, 1);

  this->userLabel = new QLabel("User:", this);
  gLayout->addWidget(userLabel, 1, 0, 1, 1);

  this->userEdit = new QLabel("", this);
  gLayout->addWidget(this->userEdit, 1, 2, 1, 1);

  this->dateLabel = new QLabel("Date:", this);
  gLayout->addWidget(dateLabel, 2, 0, 1, 1);

  this->dateEdit = new QLabel("", this);
  gLayout->addWidget(this->dateEdit, 2, 2, 1, 1);

  this->notesLabel = new QLabel("Notes:");
  gLayout->addWidget(this->notesLabel, 3, 0, 1, 1);

  this->versionNotes = new QVersionNotes();
  vLayout->addWidget(this->versionNotes);
  this->versionNotes->setEnabled(false);

  this->versionEmbed = new QVersionEmbed();
  vLayout->addWidget(this->versionEmbed);
  this->versionEmbed->setVisible(false);

  this->connect(this->tagEdit, SIGNAL(editingFinished()),
    SLOT(tagFinished()));

  this->connect(this->tagEdit, SIGNAL(textChanged(QString)),
    SLOT(tagChanged()));

  this->connect(this->tagReset, SIGNAL(clicked()),
    SLOT(tagCleared()));

  this->connect(this->searchBox, SIGNAL(resetSearch()),
    SLOT(resetSearch()));

  this->connect(this->searchBox, SIGNAL(executeSearch(QString)),
    SLOT(executeSearch()));

  this->connect(this->searchBox, SIGNAL(refineMode(bool)),
    SLOT(refineMode()));

  this->controller = NULL;
  this->id = vt::VERSION_INVALID;
  this->refineMode(false);
}

QVersionProp::~QVersionProp()
{
  delete this->vLayout;
  delete this->gLayout;
  delete this->tagLabel;
  delete this->editLayout;
  delete this->userLabel;
  delete this->dateLabel;
  delete this->searchBox;
  delete this->tagEdit;
  delete this->tagReset;
  delete this->userEdit;
  delete this->dateEdit;
  delete this->notesLabel;
  delete this->versionNotes;
  delete this->versionEmbed;

  this->vLayout = NULL;
  this->gLayout = NULL;
  this->tagLabel = NULL;
  this->editLayout = NULL;
  this->userLabel = NULL;
  this->dateLabel = NULL;
  this->searchBox = NULL;
  this->tagEdit = NULL;
  this->tagReset = NULL;
  this->userEdit = NULL;
  this->dateEdit = NULL;
  this->notesLabel = NULL;
  this->versionNotes = NULL;
  this->versionEmbed = NULL;
}

// Assign the controller to the property page
void QVersionProp::updateController(VistrailController *controller)
{
  this->controller = controller;
  this->versionNotes->controller = controller;
  this->versionEmbed->controller = controller;
}

// Update the property page of the version
void QVersionProp::updateVersion(const vt::VersionId &id)
{
  this->id = id;
  this->versionNotes->updateVersion(id);
  this->versionEmbed->updateVersion(id);

  if (this->controller)
  {
    vt::SessionFocus *focus = this->controller->getSessionFocus();
    vt::Version *version = focus->session()->vistrail()->getVersion(id);
    if (version != NULL)
    { 
      time_t timet = version->stamp()->get("date")->asDateTime()->toTime();
      QDateTime qtime = QDateTime::fromSecsSinceEpoch(timet);
      qtime.setTimeSpec(Qt::UTC);
      QString time = qtime.toLocalTime().toString("yyyy-M-d hh:mm:ss ap");

      this->tagEdit->setText(getVal(version->annotations(), "tag", ""));
      this->userEdit->setText(getVal(version->stamp(), "user", "<unknown>"));
      this->dateEdit->setText(time);
      this->tagEdit->setEnabled(true);
      this->versionEmbed->setVisible(this->versionEmbed->check_version());
      return;
    }
    else
    {
      this->tagEdit->setEnabled(false);
      this->tagReset->setEnabled(false);
    }
  }
  this->tagEdit->setText("");
  this->userEdit->setText("");
  this->dateEdit->setText("");
}

// Update the new tag to vistrail
void QVersionProp::tagFinished()
{
  if (this->controller)
  {
    // FIXME
    //this->controller->update_current_tag(this->tagEdit.text());
  }
}

// Update the button state if there is text
void QVersionProp::tagChanged(QString text)
{
  this->tagReset->setEnabled(!text.isEmpty());
}

// Remove the tag
void QVersionProp::tagCleared()
{
  this->tagEdit->setText("");
  this->tagFinished();
}

void QVersionProp::resetSearch(bool emit_signal)
{
  if (this->controller && emit_signal)
  {
    // FIXME
    //this->controller->set_search(NULL);
    //emit this->textQueryChange(false);
  }
  else
    this->searchBox->clearSearch();

}

void QVersionProp::executeSearch(QString text)
{  
  // FIXME
  //if this->controller:
  //    try:
  //        search = SearchCompiler(text).searchStmt
  //    except SearchParseError, e:
  //        QMessageBox.warning(this,
  //                                  QString("Search Parse Error"),
  //                                  QString(str(e)),
  //                                  QMessageBox.Ok,
  //                                  QMessageBox.NoButton,
  //                                  QMessageBox.NoButton)
  //        search = None
  //    this->controller.set_search(search, s)
  //    this->emit(SIGNAL("textQueryChange(bool)"), s!="")
}

void QVersionProp::refineMode(bool on)
{
  // FIXME
  //if this->controller:
  //    this->controller.set_refine(on)
}

//###################################################################
QVersionNotes::QVersionNotes(QWidget *parent): QTextEdit(parent)
{
  this->controller = NULL;
  this->id = vt::VERSION_INVALID;
  this->update_on_focus_out = true;
  // Reset text to black, for some reason it is grey by default on the mac
  QBrush brush = QBrush(QColor(0,0,0,255));
  QPalette p = QPalette(this->palette());
  p.setBrush(QPalette::Text, brush);
  this->setPalette(p);
  this->setFont(ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
}

// Update the text to be the notes of the vistrail version
void QVersionNotes::updateVersion(const vt::VersionId &id)
{
  this->id = id;
  if (this->controller)
  {    
    vt::SessionFocus *focus = this->controller->getSessionFocus();
    vt::Version *version = focus->session()->vistrail()->getVersion(id);
    if (version != NULL)
    {
      vt::Property *p = version->annotations()->get("notes");
      if(p)
      {
        this->setHtml(QString(p->asChar()));
        // work around a strange bug where an empty new paragraph gets 
        // added every time
        this->trim_first_paragraph();
      }
      else
        this->setHtml("");
      this->setEnabled(true);
      return;
    }
    else
      this->setEnabled(false);
  }
  this->setPlainText("");
}

void QVersionNotes::commit_changes()
{
  vt::SessionFocus *focus = VisTrailsWindow::getInstance()->getSessionFocus();
  vt::Version *version = focus->session()->vistrail()->getVersion(this->id);
  if (version != NULL)
  {
    version->annotations()
      ->setString("notes", this->toPlainText().toUtf8().constData());
    focus->versionChanged(this->id);
  }
}

void QVersionNotes::reset_changes()
{
  this->updateVersion(this->id);
}

// Update the version notes if the text has been modified
void QVersionNotes::focusOutEvent(QFocusEvent *e)
{
  if (this->update_on_focus_out)
    this->commit_changes();

  QTextEdit::focusOutEvent(e);
}

void QVersionNotes::trim_first_paragraph()
{
  QTextCursor cursor = QTextCursor(this->document());
  cursor.select(QTextCursor::LineUnderCursor);
  QString sel = cursor.selection().toPlainText();
  bool all = true;
  Q_FOREACH(QChar c, sel)
  {
    if(c != ' ')
      all = false;
  }
  if (all)
  {
    cursor.removeSelectedText();
    cursor = QTextCursor(this->document());
    cursor.deleteChar();
  }
}

// ################################################################################

// #videoControl = None
// from gui.application import get_vistrails_application
// mayaFlag = get_vistrails_application()

QVersionPropOverlay::QVersionPropOverlay(QWidget *parent, 
                                         QWidget *mouseWidget)
                                         : QFrame(parent)
{
  this->propagatingWidget = mouseWidget;

  this->palette = new QPalette();
  this->palette->setColor(QPalette::Base, QColor(0,0,0,0));
  this->setPalette(*this->palette);

  this->setAutoFillBackground(true);
  this->setFrameStyle(QFrame::NoFrame);
  this->setFrameShadow(QFrame::Plain);

  this->layout = new QGridLayout();
  this->layout->setVerticalSpacing(1);

  bool mayaFlag = false;
  this->tag_label = new QLabel();
  if (!mayaFlag)
  {
    this->tag_label->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
    this->tag_label->setText(QString("Tag:"));
  }
  else
  {
    QPalette p = QPalette(this->tag_label->palette());
    p.setBrush(QPalette::Text,
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN);
    this->tag_label->setPalette(p);
    this->tag_label->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
    this->tag_label->setText(QString("Tag:"));
  }

  this->tag = new QLabel();
  if (!mayaFlag)
  {
    this->tag->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
  }
  else
  {
    this->tag->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
  }

  this->tag_layout = new QHBoxLayout();
  this->tag_layout->addWidget(this->tag);
  this->tag_layout->addStretch();

  this->description_label = new QLabel();
  if (!mayaFlag)
  {
    this->description_label->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
    this->description_label->setText(QString("Action:"));
  }
  else
  {
    QPalette p = QPalette(this->description_label->palette());
    p.setBrush(QPalette::Text,
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN);
    this->description_label->setPalette(p);
    this->description_label->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
    this->description_label->setText(QString("Action:"));
  }

  this->description = new QLabel();
  if (!mayaFlag)
  {
    this->description->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
  }
  else
  {
    this->description->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
  }

  this->description_layout = new QHBoxLayout();
  this->description_layout->addWidget(this->description);
  this->description_layout->addStretch();

  this->user_label = new QLabel();
  if (!mayaFlag)
  {
    this->user_label->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
    this->user_label->setText(QString("User:"));
  }
  else
  {
    QPalette p = QPalette(this->user_label->palette());
    p.setBrush(QPalette::Text,
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN);
    this->user_label->setPalette(p);
    this->user_label->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
    this->user_label->setText(QString("User:"));
  }

  this->user = new QLabel();
  if (!mayaFlag)
  {
    this->user->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
  }
  else
  {
    this->user->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
  }

  this->user_layout = new QHBoxLayout();
  this->user_layout->addWidget(this->user);
  this->user_layout->addStretch();

  this->date_label = new QLabel();
  if (!mayaFlag)
  {
    this->date_label->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
    this->date_label->setText(QString("Date:"));
  }
  else
  {
    QPalette p = QPalette(this->date_label->palette());
    p.setBrush(QPalette::Text,
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN);
    this->date_label->setPalette(p);
    this->date_label->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
    this->date_label->setText(QString("Date:"));
  }

  this->date = new QLabel();
  if (!mayaFlag)
  {
    this->date->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
  }
  else
  {
    this->date->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
  }

  this->date_layout = new QHBoxLayout();
  this->date_layout->addWidget(this->date);
  this->date_layout->addStretch();

  //NOTES
  this->notes_label = new QLabel();
  if (!mayaFlag)
  {
    this->notes_label->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
    this->notes_label->setText(QString("Notes:"));
  }
  else
  {
    QPalette p = QPalette(this->notes_label->palette());
    p.setBrush(QPalette::Text,
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN);
    this->notes_label->setPalette(p);
    this->notes_label->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
    this->notes_label->setText(QString("Notes:"));
  }

  this->notes = new QLabel();
  if (!mayaFlag)
  {
    this->notes->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
  }
  else
  {
    this->notes->setTextFormat(Qt::RichText);
    this->notes->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
  }

  this->notes_button = new QExpandButton();
  this->notes_button->hide();

  this->notes_layout = new QHBoxLayout();
  this->notes_layout->addWidget(this->notes);
  this->notes_layout->addSpacing(5);
  this->notes_layout->addWidget(this->notes_button);
  this->notes_layout->addStretch();

  //Added video here
  //this->videofname = NULL;
  //this->videoDeleted = false
  //this->video_label = QLabel()
  //if (!mayaFlag)
  //    this->video_label->setStyleSheet("QLabel { background-color : white; color : black; }")
  //    this->video_label->setText(QString("Video:"))
  //else
  //    this->video_label->palette()->setBrush(QPalette::Text,
  //                                        ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN)
  //    this->video_label->setFont(ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT)
  //    this->video_label->setText(QString("Video:"))

  //this->video = QLabel()
  //this->video->setTextFormat(Qt->RichText)
  //this->video->setFont(ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT)

  //this->open_video_button = QOpenVideo()
  //this->open_video_button->hide()

  //this->delete_video_button = QDeleteVideoButton()
  //this->delete_video_button->hide()

  //this->video_layout = QHBoxLayout()
  //this->video_layout->addWidget(this->video)
  //this->video_layout->addSpacing(5)
  //this->video_layout->addWidget(this->open_video_button)
  //this->video_layout->addWidget(this->delete_video_button)
  //this->video_layout->addStretch()
  //
  ////Added audio here
  //this->audioMedia = None
  //this->audio_label = QLabel()
  //if (!mayaFlag)
  //    this->audio_label->setStyleSheet("QLabel { background-color : white; color : black; }")
  //    this->audio_label->setText(QString("Audio:"))
  //else
  //    this->audio_label->palette()->setBrush(QPalette::Text,
  //                                        ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN)
  //    this->audio_label->setFont(ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT)
  //    this->audio_label->setText(QString("Audio:"))

  //this->audio = QLabel()
  //this->audio->setTextFormat(Qt->RichText)
  //this->audio->setFont(ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT)

  //this->play_audio_button = QPlayButton()
  //this->play_audio_button->hide()

  //this->pause_audio_button = QPauseButton()
  //this->pause_audio_button->hide()

  //this->stop_audio_button = QStopButton()
  //this->stop_audio_button->hide()

  //this->open_audio_button = QOpenMultimediaButton()
  //this->open_audio_button->hide()

  //this->delete_audio_button = QDeleteButton()
  //this->delete_audio_button->hide()

  //this->audio_layout = QHBoxLayout()
  //this->audio_layout->addWidget(this->audio)
  //this->audio_layout->addSpacing(5)
  //this->audio_layout->addWidget(this->open_audio_button)
  //this->audio_layout->addWidget(this->play_audio_button)
  //this->audio_layout->addWidget(this->pause_audio_button)
  //this->audio_layout->addWidget(this->stop_audio_button)
  //this->audio_layout->addWidget(this->delete_audio_button)
  //this->audio_layout->addStretch()


  //Capture Camera Movement (Only for the paper stuff that we are using)
  // this->camera = QCaptureCameraMovement()
  // this->camera_layout = QHBoxLayout()
  // this->camera_layout->addWidget(this->camera)
  // this->camera_layout->addSpacing(5)
  // this->camera_layout->addStretch()


  //Details
  this->details_label = new QLabel();
  if (!mayaFlag)
  {
    this->details_label->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
    this->details_label->setText(QString("Details:"));
  }
  else
  {
    QPalette p = QPalette(this->details_label->palette());
    p.setBrush(QPalette::Text,
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_PEN);
    this->details_label->setPalette(p);
    this->details_label->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
    this->details_label->setText(QString("Details:"));
  }

  this->details = new QLabel();
  if (!mayaFlag)
  {
    this->details->setStyleSheet(
      "QLabel { background-color : white; color : black; }");
  }
  else
  {
    this->details->setTextFormat(Qt::PlainText);
    this->details->setFont(
      ThemeHolder::getCurrentTheme()->VERSION_PROPERTIES_FONT);
  }

  this->details_layout = new QHBoxLayout();
  this->details_layout->addWidget(this->details);
  this->details_layout->addStretch();

  this->layout->addWidget(this->tag_label, 0, 0);
  this->layout->addLayout(this->tag_layout, 0, 1);
  this->layout->addWidget(this->description_label, 1, 0);
  this->layout->addLayout(this->description_layout, 1, 1);
  this->layout->addWidget(this->user_label, 2, 0);
  this->layout->addLayout(this->user_layout, 2, 1);
  this->layout->addWidget(this->date_label, 3, 0);
  this->layout->addLayout(this->date_layout, 3, 1);

  this->layout->addWidget(this->notes_label, 4, 0);
  this->layout->addLayout(this->notes_layout, 4, 1);


  //this->layout->addWidget(this->audio_label, 5, 0)
  //this->layout->addLayout(this->audio_layout, 5,1)


  //this->layout->addWidget(this->video_label, 6, 0)
  //this->layout->addLayout(this->video_layout, 6,1)


  this->layout->addWidget(this->details_label, 8, 0);
  this->layout->addLayout(this->details_layout, 8, 1);

  //       this->layout->addLayout(this->camera_layout, 9, 0)

  this->layout->setColumnMinimumWidth(0,35);
  this->layout->setColumnMinimumWidth(1,200);
  this->layout->setContentsMargins(2,2,2,2);
  this->layout->setColumnStretch(1,1);
  this->setLayout(this->layout);
  this->updateGeometry();
  this->controller = NULL;

  this->notes_dialog = new QNotesDialog(this);
  this->notes_dialog->hide();

  this->connect(this->notes_button, SIGNAL(pressed()), SLOT(openNotes()));

  //QObject.connect(this->open_audio_button,
  //                       SIGNAL("pressed()"),
  //                       this->openMultimedia)

  //QObject.connect(this->play_audio_button,
  //                       SIGNAL("pressed()"),
  //                       this->playAudio)

  //QObject.connect(this->pause_audio_button,
  //                       SIGNAL("pressed()"),
  //                       this->pauseAudio)

  //QObject.connect(this->stop_audio_button,
  //                       SIGNAL("pressed()"),
  //                       this->stopAudio)

  //QObject.connect(this->delete_audio_button,
  //                      SIGNAL("pressed()"),
  //                      this->deleteAudio)

  //QObject.connect(this->open_video_button,
  //                       SIGNAL("pressed()"),
  //                       this->openVideo)

  //QObject.connect(this->delete_video_button,
  //                       SIGNAL("pressed()"),
  //                       this->deleteVideo)

}

QVersionPropOverlay::~QVersionPropOverlay()
{
  delete this->palette;
  delete this->layout;
  delete this->tag_label;
  delete this->tag;
  //delete this->tag_layout;
  delete this->description_label;
  delete this->description;
  //delete this->description_layout;
  delete this->user_label;
  delete this->user;
  //delete this->user_layout;
  delete this->date_label;
  delete this->date;
  //delete this->date_layout;
  delete this->notes_label;
  delete this->notes;
  //delete this->notes_layout;
  delete this->notes_button;
  delete this->details_label;
  delete this->details;
  //delete this->details_layout;
  //delete this->details_button;
  delete this->notes_dialog;

  this->propagatingWidget = NULL;
  this->palette = NULL;
  this->layout = NULL;
  this->tag_label = NULL;
  this->tag = NULL;
  this->tag_layout = NULL;
  this->description_label = NULL;
  this->description = NULL;
  this->description_layout = NULL;
  this->user_label = NULL;
  this->user = NULL;
  this->user_layout = NULL;
  this->date_label = NULL;
  this->date = NULL;
  this->date_layout = NULL;
  this->notes_label = NULL;
  this->notes = NULL;
  this->notes_layout = NULL;
  this->notes_button = NULL;
  this->details_label = NULL;
  this->details = NULL;
  this->details_layout = NULL;
  this->details_button = NULL;
  this->notes_dialog = NULL;
  this->controller = NULL;
}

void QVersionPropOverlay::updateGeometry()
{
  this->pos_x = 4;
  this->pos_y = 4;
  this->move(this->pos_x, this->pos_y);
}

// Assign the controller to the properties
void QVersionPropOverlay::updateController(VistrailController *controller)
{
  this->controller = controller;
  this->notes_dialog->updateController(controller);
}

// Update the text items
void QVersionPropOverlay::updateVersion(const vt::VersionId &id)
{
  this->notes_dialog->updateVersion(id);
  if (this->controller)
  {    
    vt::SessionFocus *focus = this->controller->getSessionFocus();
    vt::Version *version = focus->session()->vistrail()->getVersion(id);
    if (version != NULL)
    { 
      time_t timet = version->stamp()->get("date")->asDateTime()->toTime();
      QDateTime qtime = QDateTime::fromSecsSinceEpoch(timet);
      qtime.setTimeSpec(Qt::UTC);
      QString time = qtime.toLocalTime().toString("yyyy-M-d hh:mm:ss ap");

      this->tag->setText(getVal(version->annotations(), "tag", "<untagged>"));
      this->description->setText(
        getVal(version->annotations(), "label", "<unknown>"));
      this->details->setText(
        getVal(version->annotations(), "description", ""));
      this->user->setText(getVal(version->stamp(), "user", "<unknown>"));
      this->date->setText(time);

      vt::Property* pnotes = version->annotations()->get("notes");
      if(pnotes)
      {
        QString s = this->convertHtmlToText(QString(pnotes->asChar()));
        this->notes->setText(this->truncate(s));
      }
      else
        this->notes->setText("");
      this->notes_button->show();
      //this->open_audio_button.show()
      //this->open_video_button.show()
      //this->audio_filename = action.audio
      //this->video_filename = action.video
      //if this->controller->videoMediaObject != None:
      //    this->controller->videoMediaObject.filename = this->video_filename
      //else
      //    {
      //    this->controller->videoMediaObject.stopVideo()
      //    this->controller->videoMediaObject.exitVideo()
      //    }
    }          
    else
    {
      this->details->setText("");
      this->tag->setText("");
      this->description->setText("");
      this->user->setText("");
      this->date->setText("");
      this->notes->setText("");
      this->notes_button->hide();
      //this->open_audio_button->hide()
      //this->open_video_button->show()
      //this->audio_filename = None
      //this->video_filename = None
    }
  }
}

// Remove HTML tags and newlines
QString QVersionPropOverlay::convertHtmlToText(QString str)
{
  // Some text we want to ignore lives outside brackets in the header
  str.replace(QRegularExpression("<head>.*</head>"), "");
  // Remove all other tags
  str.replace(QRegularExpression("<[^>]*>"), "");
  // Remove newlines
  str.replace(QRegularExpression("\n"), " ");
  return str;
}

// Shorten string to fit in smaller space
QString QVersionPropOverlay::truncate(QString str)
{
  if (str.size() > 24)
  {
    str.truncate(22);
    str.append("...");
  }
  str.replace(QRegularExpression("\n"), "...\n");
  return str;
}

void QVersionPropOverlay::openNotes()
{
  this->notes_dialog->show();
  this->notes_dialog->activateWindow();
}

void QVersionPropOverlay::versionAdded(vt::Version *version)
{
  // DAK not sure if we need to check this...
  // if(VisTrailsWindow::getInstance()->getCurrentViewIndex() == 0) // tree view
  this->updateVersion(version->id());
}

void QVersionPropOverlay::versionChanged(const vt::VersionId &id)
{
  if(VisTrailsWindow::getInstance()->getCurrentViewIndex() == 0) // tree view
    this->updateVersion(id);
}

void QVersionPropOverlay::versionSelected(const vt::VersionId &previous,
  const vt::VersionId &current)
{
  // DAK not sure if we need to check this...
  // if(VisTrailsWindow::getInstance()->getCurrentViewIndex() == 0) // tree view
  this->updateVersion(current);
}

//int QVersionPropOverlay::extract_file(QString filename, QString extractDir, QString zipFilename, QString mediaType)
//{
//
//}
//
//void QVersionPropOverlay::openMultimedia()
//{
//
//}
//
//void QVersionPropOverlay::deleteAudio()
//{
//
//}
//
//void QVersionPropOverlay::loadPhonon()
//{
//
//}
//
//void QVersionPropOverlay::loadAudio()
//{
//
//}
//
//void QVersionPropOverlay::playAudio()
//{
//
//}

// #userTrack
// import time, gui.application
// app = gui.application.VistrailsApplication
// if app.tracker.closed == False:
// if self.audio_filename != None:
// app.tracker.write('<item><action>audio</action><object>play</object><id>' + self.audio_filename + '</id><timestamp>' + str(int(time.time())) + '</timestamp></item>\r\n')
// app.tracker.flush()

// if self.audioMedia == None:
// self.loadPhonon()
// self.loadAudio()
// self.filename = self.audio_filename
// if self.audioMedia == None:
// self.loadPhonon()
// try:
// if self.filename:
// self.audioMedia.play()
// except Exception:
// raise Exception("Unable to play file format")

// def stopAudio(self):
// self.audioMedia.stop()
// #userTrack
// import time, gui.application
// app = gui.application.VistrailsApplication
// if app.tracker.closed == False:
// if self.audio_filename != None:
// app.tracker.write('<item><action>audio</action><object>stop</object><id>' + self.audio_filename + '</id><timestamp>' + str(int(time.time())) + '</timestamp></item>\r\n')
// app.tracker.flush()

// def pauseAudio(self):
// self.audioMedia.pause()
// #userTrack
// import time, gui.application
// app = gui.application.VistrailsApplication
// if app.tracker.closed == False:
// if self.audio_filename != None:
// app.tracker.write('<item><action>audio</action><object>pause</object><id>' + self.audio_filename + '</id><timestamp>' + str(int(time.time())) + '</timestamp></item>\r\n')
// app.tracker.flush()

// #userTrack
// import time, gui.application
// app = gui.application.VistrailsApplication
// app.tracker.write('<item><action>notes</action><object>add</object><id>none</id><timestamp>' + str(int(time.time())) + '</timestamp></item>\r\n')
// app.tracker.flush()

// def event(self, e):
// """ Propagate all mouse events to the right widget """
// if e.type() in self.propagatingEvent:
// if self.propagatingWidget!=None:
// QtCore.QCoreApplication.sendEvent(self.propagatingWidget, e)
// return False
// return QFrame.event(self, e)


// #Video
// def openVideo(self):
// from gui.application import VistrailsApplication
// self.extract_file(self.video_filename, self.controller.video_dir, self.controller.file_name, "video")

// #If video was deleted by user, and the user is attempting to add another video
// if self.videoDeleted == True and self.video_filename == None:
// if VistrailsApplication.videoControl == None:
// VistrailsApplication.videoControl = VideoMainWindow()
// VistrailsApplication.videoControl.setVideoDir(self.controller.video_dir)
// vstatus, vdir, vfname = VistrailsApplication.videoControl.open()
// if vstatus == True:
// self.video_filename = os.path.join(vdir, "video", vfname)
// self.controller.update_video(vfname)
// VistrailsApplication.videoControl.setVideoFilename(vfname)
// VistrailsApplication.videoControl.show()
// self.delete_video_button.show()
// self.videoDeleted = False

// #If user is attempting to add a video annotation for the first time
// else
// #If VistrailsApplication.videoControl object has not been initialized
// if VistrailsApplication.videoControl == None:
// VistrailsApplication.videoControl = VideoMainWindow()
// if self.controller.video_dir == None:
// pass
// if self.video_filename == None:
// pass
// else
// fname = os.path.join(self.controller.video_dir, "video", self.video_filename)
// self.controller.update_video(os.path.basename(fname))


// #If VistrailsApplication.videoControl object has been initialized and video filename is not set yet
// if self.video_filename == None:
// if self.controller.video_dir != None:
// VistrailsApplication.videoControl.setVideoDir(self.controller.video_dir)
// vstatus, vdir, vfname = VistrailsApplication.videoControl.open()
// if vstatus == True:
// self.controller.video_dir = vdir
// self.video_filename = os.path.join(vdir, "video", vfname)


// #If user already the the videofilename, update controller and VistrailsApplication.videoControl variables
// if self.video_filename != None:
// self.controller.update_video(os.path.basename(self.video_filename))
// VistrailsApplication.videoControl.show()
// VistrailsApplication.videoControl.status = True
// self.controller.setVideoMediaObject(VistrailsApplication.videoControl)
// VistrailsApplication.videoControl.setVideoDir(self.controller.video_dir)
// VistrailsApplication.videoControl.setVideoFilename(self.video_filename)
// VistrailsApplication.videoControl.show()
// self.delete_video_button.show()

// #userTrack
// import time, gui.application
// app = gui.application.VistrailsApplication
// app.tracker.write('<item><action>video</action><object>open</object><id>' + self.video_filename + '</id><timestamp>' + str(int(time.time())) + '</timestamp></item>\r\n')
// app.tracker.flush()

// def deleteVideo(self):
// from gui.application import VistrailsApplication
// fname = os.path.join(self.controller.video_dir, "video", os.path.basename(self.video_filename))
// print "Removed , ", fname
// os.remove(fname)
// self.controller.update_video("")

// #userTrack
// import time, gui.application
// app = gui.application.VistrailsApplication
// app.tracker.write('<item><action>video</action><object>delete</object><id>' + self.video_filename + '</id><timestamp>' + str(int(time.time())) + '</timestamp></item>\r\n')
// app.tracker.flush()

// if os.path.isfile(fname):
// if VistrailsApplication.videoControl == None:
// pass
// if VistrailsApplication.videoControl != None:
// VistrailsApplication.videoControl.exitVideo()
// VistrailsApplication.videoControl.close()

// self.delete_video_button.hide()
// self.video_filename = None
// self.videoDeleted = True


// ################################################################################



QExpandButton::QExpandButton(QWidget *parent): QLabel(parent)
{
  this->drawButton(false);
  this->setToolTip("Edit Notes");
  this->setScaledContents(false);
  this->setFrameShape(QFrame::NoFrame);
}

QSize QExpandButton::sizeHint()
{
  return QSize(10,10);
}

// Capture mouse press event on the frame to move the widget
void QExpandButton::mousePressEvent(QMouseEvent *e)
{
  if (e->buttons() & Qt::LeftButton)
    this->drawButton(true);
}

void QExpandButton::mouseReleaseEvent(QMouseEvent *e)
{
  this->drawButton(0);
  emit pressed();
}

// Custom draw function
void QExpandButton::drawButton(bool down)
{
  this->picture = QPicture();
  QPainter *painter = new QPainter();
  painter->begin(&this->picture);
  painter->setRenderHints(QPainter::Antialiasing, false);
  QPen pen = QPen(Qt::SolidLine);
  pen.setWidth(1);
  pen.setCapStyle(Qt::RoundCap);
  QBrush brush = QBrush(Qt::NoBrush);
  if (down)
    pen.setColor(QColor(150,150,150,150));
  else
    pen.setColor(QColor(0,0,0,255));

  painter->setPen(pen);
  painter->setBrush(brush);
  painter->drawRect(0,0,8,8);
  painter->drawLine(QLine(4,2,4,6));
  painter->drawLine(QLine(2,4,6,4));
  painter->drawLine(QLine(7,1,7,7));
  painter->end();
  this->setPicture(this->picture);
  delete painter;
}

// ###############################################################################

//QOpenVideo::QOpenVideo()
//{
//
//}


// ################################################################################

//QOpenMultimediaButton::QOpenMultimediaButton()
//{
//
//}

//QPlayButton::QPlayButton()
//{
//
//}
//
//// Custom draw function
//void QPlayButton::drawButton(bool down)
//{
//
//}


// ################################################################################

//QDeleteButton::QDeleteButton()
//{
//}
//
//void QDeleteButton::drawButton(bool down)
//{
//}
//
//
//QDeleteVideoButton::QDeleteVideoButton()
//{
//
//}

// """ Just for the paper """
// # class QCaptureCameraMovement(QExpandButton):
// #     def __init__(self):
// #         QExpandButton.__init__(self)
// #         self.setToolTip('Capture Keyboard Interation')

//QStopButton::QStopButton()
//{
//
//}
//
//// Custom draw function
//void QStopButton::drawButton(bool down)
//{
//
//}
//
//
//QPauseButton::QPauseButton()
//{
//
//}

// Custom draw function
//void QPauseButton::drawButton(bool down)
//{
//
//}

// ################################################################################
// A small non-modal dialog with text entry to modify and view notes

QNotesDialog::QNotesDialog(QWidget *parent): QDialog(parent)
{
  this->setModal(false);

  this->notes = new QVersionNotes(this);
  this->notes->update_on_focus_out = false;

  this->layout = new QVBoxLayout(this);
  this->layout->addWidget(this->notes);
  this->layout->setContentsMargins(0, 0, 0, 0);

  this->apply_button = new QPushButton("Apply", this);
  this->apply_button->setDefault(false);
  this->apply_button->setAutoDefault(false);

  this->ok_button = new QPushButton("Ok", this);
  this->ok_button->setDefault(false);
  this->ok_button->setAutoDefault(false);
  this->cancel_button = new QPushButton("Cancel", this);
  this->cancel_button->setDefault(false);
  this->cancel_button->setAutoDefault(false);
  this->buttonLayout = new QHBoxLayout();
  this->layout->addLayout(this->buttonLayout);
  this->buttonLayout->addWidget(this->apply_button);
  this->buttonLayout->addWidget(this->ok_button);
  this->buttonLayout->addWidget(this->cancel_button);

  this->setLayout(this->layout);
  this->controller = NULL;

  this->connect(this->apply_button,
    SIGNAL(released()),
    SLOT(apply_pressed()));

  this->connect(this->ok_button,
    SIGNAL(released()),
    SLOT(ok_pressed()));

  this->connect(this->cancel_button,
    SIGNAL(released()),
    SLOT(cancel_pressed()));
}

QNotesDialog::~QNotesDialog()
{
  delete this->notes;
  delete this->layout;
  delete this->apply_button;
  delete this->ok_button;
  delete this->cancel_button;
  //delete this->buttonLayout;

  this->notes = NULL;
  this->layout = NULL;
  this->apply_button = NULL;
  this->ok_button = NULL;
  this->cancel_button = NULL;
  this->buttonLayout = NULL;
}

void QNotesDialog::apply_pressed()
{
  this->notes->commit_changes();
}

void QNotesDialog::ok_pressed()
{
  this->notes->commit_changes();
  this->close();
}

void QNotesDialog::cancel_pressed()
{
  this->notes->reset_changes();
  this->close();
}

void QNotesDialog::updateController(VistrailController *controller)
{
  this->controller = controller;
  this->notes->controller = controller;
}

void QNotesDialog::updateVersion(const vt::VersionId &id)
{
  this->notes->updateVersion(id);
  if (this->controller)
  {
    vt::SessionFocus *focus = this->controller->getSessionFocus();
    vt::Version *version = focus->session()->vistrail()->getVersion(id);
    if(version)
    {
      vt::Property *p = version->annotations()->get("tag");
      if(!p)
        p = version->annotations()->get("label");
      if(p)
      {
        QString name = QString(p->asChar());
        QString title = QString("Notes: "+name);
        this->setWindowTitle(title);
        return;
      }
    }
    this->setWindowTitle("Notes");
  }
}

QSize QNotesDialog::sizeHint()
{
  return QSize(250,200);
}

// ################################################################################

QVersionEmbed::QVersionEmbed(QWidget *parent): QWidget(parent)
{
  this->id = vt::VERSION_INVALID;

  this->label1 = new QLabel("Embed as:");
  QFont lfont = QFont("Lucida Grande", 11);
  this->label1->setFont(lfont);

  this->cbtype = new QComboBox();
  this->cbtype->setFont(lfont);
  this->cbtype->setEditable(false);
  QStringList items = QStringList() << "Wiki" << "Latex";
  this->cbtype->addItems(items);

  this->controller = NULL;
  this->wikitag = "<vistrail host=\"%s\" db=\"%s\" vtid=\"%s\" version=\"%s\" \
                  tag=\"%s\" showspreadsheetonly=\"true\"/>";
  this->latextag = "\\vistrails[host=%s,\ndb=%s,\nvtid=%s,\nversion=%s,\
                   \ntag=%s,\nshowspreadsheetonly]{}";

  this->embededt = new QLineEdit(this);
  this->embededt->setReadOnly(true);

  this->copylabel = new QLabel("<a href=\"copy\">Copy to Clipboard</a>");
  this->copylabel->setFont(lfont);

  this->layout = new QGridLayout();
  this->layout->addWidget(label1,0,0);
  this->layout->addWidget(this->cbtype,0,1);
  this->layout->addWidget(this->copylabel,0,2,Qt::AlignRight);
  this->layout->addWidget(this->embededt,1,0,2,-1);
  this->setLayout(this->layout);

  //connect signals
  this->connect(this->cbtype,
    SIGNAL(currentIndexChanged(const QString &)),
    SLOT(change_embed_type()));

  this->connect(this->copylabel,
    SIGNAL(linkActivated(const QString &)),
    SLOT(copy_to_clipboard()));

}

QVersionEmbed::~QVersionEmbed()
{
  delete this->label1;
  delete this->cbtype;
  delete this->embededt;
  delete this->copylabel;
  delete this->layout;

  this->label1 = NULL;
  this->cbtype = NULL;
  this->embededt = NULL;
  this->copylabel = NULL;
  this->layout = NULL;
}

// Only vistrails on a database are allowed to embed a tag
bool QVersionEmbed::check_version()
{
  bool result = false;
  //FIXME
  //if (this->controller)
  //    if this->controller->locator:
  //        if hasattr(this->controller->locator,"host"):
  //            result = true;
  return result;
}

// this checks if the controller has saved the latest changes 
bool QVersionEmbed::check_controller_status()
{
  if (this->controller)
    return !this->controller->changed;
  return false;
}

void QVersionEmbed::updateEmbedText()
{
  bool validVersion = 
    (this->id != vt::VERSION_INVALID && this->id != vt::VERSION_ROOT);
  bool ok = (this->check_version() && this->check_controller_status() &&
    validVersion);

  this->embededt->setEnabled(ok);
  this->copylabel->setEnabled(ok);
  this->embededt->setText("");

  //FIXME
  //if (this->controller && validVersion)
  //{
  //    if (this->controller->locator && !this->controller->changed)
  //    {
  //        loc = this->controller->locator;
  //        try:
  //        {
  //            if this->cbtype->currentText() == "Wiki":
  //                tag = this->wikitag;
  //            elif this->cbtype->currentText() == "Latex":
  //                tag = this->latextag;
  //            versiontag = \
  //                this->controller->vistrail->getVersionName(this->versionNumber);

  //            tag = tag % (loc.host,
  //                         loc.db,
  //                         loc.obj_id,
  //                         this->versionNumber,
  //                         versiontag);

  //            this->embededt.setText(tag);
  //            return;
  //        }
  //        except Exception, e:
  //            this->embededt->setText("");
  //    }
  //    else if( this->controller->changed)
  //        this->embededt->setText("Please, save your vistrails first");
  //    else
  //        this->embededt->setText("");
  //}
}

void QVersionEmbed::updateVersion(const vt::VersionId &id)
{
  this->id = id;
  this->updateEmbedText();
}

void QVersionEmbed::copy_to_clipboard()
{
  QClipboard *clipboard = QApplication::clipboard();
  clipboard->setText(this->embededt->text());
}

void QVersionEmbed::change_embed_type(QString text)
{
  this->updateEmbedText();
}
