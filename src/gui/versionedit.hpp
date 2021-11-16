/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef VERSIONEDIT_HPP
#define VERSIONEDIT_HPP

#include <QtGui>
#include "VisTrails.h"

class VersionEdit : public QDialog
{
  Q_OBJECT

public:
  VersionEdit(QWidget *parent = NULL);
  void setVersion(vt::Version *version);
  const vt::VersionId &getVersion() const;

public slots:
  virtual void accept();

protected:
  virtual void showEvent(QShowEvent *e);
  virtual void closeEvent(QCloseEvent *e);

private:
  QLineEdit *tagLine;
  QPlainTextEdit *detailsEdit;
  QPushButton *okBtn;
  vt::Version *version;
};

class VersionContextMenu: public QMenu
{
  Q_OBJECT

public:
  VersionContextMenu(QWidget *parent = NULL);
  void setVersion(const vt::VersionId &id);
  VersionEdit *getVersionEdit();  

public slots:
  void editVersion();
  void pruneVersion();

private:
  QAction *editAction;
  QAction *pruneAction;
  VersionEdit *versionEdit;
};

#endif
