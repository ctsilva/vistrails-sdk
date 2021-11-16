/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef PREFERENCESDIALOG_HPP
#define PREFERENCESDIALOG_HPP

#include <QDialog>

namespace Ui {
    class PreferencesDialog;
}

class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreferencesDialog(QWidget *parent = 0);
    ~PreferencesDialog();

    bool bColorChecked;
    static PreferencesDialog* getInstance();

private slots:
    void on_buttonBox_accepted();
    void applyButton();
    void savePreferencesToggled();

private:
    static PreferencesDialog* instance;

    QPushButton* psApply;
    Ui::PreferencesDialog *ui;

    void loadSettings();
};

#endif
