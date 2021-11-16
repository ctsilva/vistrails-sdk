/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "versionedit.hpp"
#include "vtwindow.hpp"

VersionEdit::VersionEdit(QWidget *parent)
  : QDialog(parent)
{
  QLabel *tagLabel = new QLabel("Tag: ", this);
  QLabel *detailsLabel = new QLabel("Notes: ", this);

  this->tagLine = new QLineEdit(this);
  this->detailsEdit = new QPlainTextEdit(this);

  QPushButton *cancelBtn = new QPushButton("&Cancel", this);
  QPushButton *applyBtn = new QPushButton("&Apply", this);
  this->okBtn = new QPushButton("&Ok", this);

  this->connect(cancelBtn, SIGNAL(clicked()), SLOT(hide()));
  this->connect(applyBtn, SIGNAL(clicked()), SLOT(accept()));
  this->connect(okBtn, SIGNAL(clicked()), SLOT(accept()));
  this->connect(okBtn, SIGNAL(clicked()), SLOT(hide()));

  QHBoxLayout *tagLayout = new QHBoxLayout;
  tagLayout->addWidget(tagLabel);
  tagLayout->addWidget(this->tagLine);

  QHBoxLayout *btnLayout = new QHBoxLayout;
  btnLayout->addWidget(applyBtn);
  btnLayout->addWidget(this->okBtn);
  btnLayout->addWidget(cancelBtn);
  this->okBtn->setDefault(true);

  QVBoxLayout *mainLayout = new QVBoxLayout;
  mainLayout->addLayout(tagLayout);
  mainLayout->addSpacerItem(new QSpacerItem(1,6));
  mainLayout->addWidget(detailsLabel);
  mainLayout->addWidget(this->detailsEdit);
  mainLayout->addLayout(btnLayout);

  this->setLayout(mainLayout);
  this->setWindowTitle(tr("Edit Tag/Notes"));
}

void VersionEdit::setVersion(vt::Version *version)
{
  this->version = version;
}

const vt::VersionId &VersionEdit::getVersion() const
{
  return this->version->id();
}

void VersionEdit::accept()
{
  if(this->tagLine->text().isEmpty())
  {
    version->annotations()->remove("tag");
  }
  else
  {
    version->annotations()->setString("tag", 
      this->tagLine->text().toUtf8().constData());
  }

  if(this->detailsEdit->toPlainText().isEmpty())
  {
    version->annotations()->remove("notes");
  }
  else
  {
    version->annotations()->setString("notes", 
      this->detailsEdit->toPlainText().toUtf8().constData());
  }

  VisTrailsWindow::getInstance()
    ->getSessionFocus()->versionChanged(version->id());
}

void VersionEdit::showEvent(QShowEvent *e)
{
  if(this->version)
  {
    this->tagLine->clear();
    vt::Property *p = this->version->annotations()->get("tag");
    if(p)
      this->tagLine->setText(QString(p->asChar()));

    this->detailsEdit->clear();
    p = this->version->annotations()->get("notes");
    if(p)
      this->detailsEdit->appendPlainText(QString(p->asChar()));

    this->okBtn->setFocus();
    this->tagLine->setFocus();
    this->tagLine->selectAll();
  }
  else
    e->ignore();
}


void VersionEdit::closeEvent(QCloseEvent *e)
{
  this->hide();
}

//--------------------------------------------------------------

VersionContextMenu::VersionContextMenu(QWidget *parent)
  : QMenu(parent)
{
  this->versionEdit = new VersionEdit(parent);

  this->editAction = new QAction("&Edit Tag/Notes", this);
  this->pruneAction = new QAction("Prune", this);
  this->addAction(this->editAction);
  this->addAction(this->pruneAction);

  connect(this->editAction,
    SIGNAL(triggered()),
    this,
    SLOT(editVersion()));

  connect(this->pruneAction,
    SIGNAL(triggered()),
    this,
    SLOT(pruneVersion()));
}

void VersionContextMenu::setVersion(const vt::VersionId &id)
{
  vt::SessionFocus *focus = VisTrailsWindow::getInstance()->getSessionFocus();
  this->pruneAction->setEnabled(!focus->inRedoHistory(id));
  this->versionEdit->setVersion(
    focus->session()->vistrail()->getVersion(id));
}

void VersionContextMenu::editVersion()
{
  this->versionEdit->show();
}

void VersionContextMenu::pruneVersion()
{
  QMessageBox warning(
    QMessageBox::Warning, 
    "Prune version", 
    "Are you sure you want to prune this version and all of its children?", 
    QMessageBox::Yes | QMessageBox::Cancel);
  int result = warning.exec();
  if(result == QMessageBox::Yes)
    VisTrailsWindow::getInstance()->pruneVersion(this->versionEdit->getVersion());
}


VersionEdit *VersionContextMenu::getVersionEdit()
{
  return this->versionEdit;
}
