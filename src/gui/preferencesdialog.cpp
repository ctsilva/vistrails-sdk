/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QtGui>
#include "preferencesdialog.hpp"
#include "ui_preferencesdialog.h"
#include "vtwindow.hpp"

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog)
{
    ui->setupUi(this);
    
    psApply = ui->buttonBox->button(QDialogButtonBox::Apply);
    connect(psApply, SIGNAL(clicked()), this, SLOT(applyButton()));

    connect(ui->rdAuto, SIGNAL(toggled(bool)), 
      this, SLOT(savePreferencesToggled()));

    loadSettings();
}


PreferencesDialog* PreferencesDialog::getInstance()
{
    if(!instance)
	instance = new PreferencesDialog();
    return instance;
}

void PreferencesDialog::savePreferencesToggled()
{
  ui->sbFileSizeLimit->setEnabled(ui->rdAuto->isChecked());
}

void PreferencesDialog::loadSettings()
{
  QSettings *settings = VisTrailsWindow::getInstance()->getSettings();
  ui->lineUsername->setText(
    settings->value("username", "").toString());
  ui->cbListViewColor->setChecked(
    settings->value("listColorState", true).toBool());
  
  int pref = settings->value("savePreference", 
			     AlwaysPrompt).toInt();
  switch(pref)
  {
  case AlwaysInclude:
    ui->rdAuto->setChecked(true);
    break;
  case NeverInclude:
    ui->rdNever->setChecked(true);
    break;
  default:
    ui->rdPromptMe->setChecked(true);
  }

  ui->sbFileSizeLimit->setValue(
    settings->value("fileSizeLimit", 50).toInt());
}

void PreferencesDialog::on_buttonBox_accepted()
{ }


void PreferencesDialog::applyButton()
{
  QSettings *settings = VisTrailsWindow::getInstance()->getSettings();
  settings->setValue("username", ui->lineUsername->text());
  settings->setValue("listColorState", ui->cbListViewColor->isChecked());

  int savePref;
  if(ui->rdAuto->isChecked())
    savePref = AlwaysInclude;
  else if(ui->rdNever->isChecked())
    savePref = NeverInclude;
  else
    savePref = AlwaysPrompt;

  settings->setValue("savePreference", savePref);
  settings->setValue("fileSizeLimit", ui->sbFileSizeLimit->value());

  // update user in session
  if (!ui->lineUsername->text().isEmpty()) {
    vt::SessionFocus *focus = VisTrailsWindow::getInstance()->getSessionFocus();
    const char * user = ui->lineUsername->text().toStdString().c_str();
    focus->session()->info()->setString("user", user);
  }
}


PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

