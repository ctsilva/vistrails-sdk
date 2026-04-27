/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QApplication>

#include "mainwindow.h"

//! [0]
int main(int argv, char *args[])
{

    QApplication app(argv, args);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
//! [0]
