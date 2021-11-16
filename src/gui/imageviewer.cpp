/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include <QtGui>
#include "imageviewer.hpp"
#include "ui_imageviewer.h"
#include "vtwindow.hpp"

imageViewer::imageViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageViewer)
{
    ui->setupUi(this);
    scn = new QGraphicsScene(ui->graphicsView);
    ui->graphicsView->setScene(scn);

}

void imageViewer::setImageName(QString name)
{
    this->imageName = name;
}

void imageViewer::changeWidgetSize()
{

}

void imageViewer::changePixmap()
{
    VisTrailsWindow *mw = VisTrailsWindow::getInstance();
    QPixmap pix(this->imageName);
    int w = pix.width();
    int h = pix.height();
    int x = mw->geometry().x()+mw->geometry().width();
    int y = mw->geometry().y();
    scn->addPixmap(pix);
    scn->setSceneRect(0,0,w,h);
    ui->graphicsView->resize(w+10,h+10);
    this->setGeometry(x,y,w+10,h+10);
}

imageViewer::~imageViewer()
{
    delete ui;
}
