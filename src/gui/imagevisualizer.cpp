/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "imagevisualizer.hpp"
#include "ui_imagevisualizer.h"
#include <QtGui>
#include <QtCore>


imageVisualizer::imageVisualizer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::imageVisualizer)
{
    ui->setupUi(this);
    imageScene = new QGraphicsScene();

}
void imageVisualizer::resizeEvent(QResizeEvent *event)
{
    this->setDisplayImage(event->size().width(),
                     event->size().height());
    width = event->size().width();
    height = event->size().height();
    if (!dirPixImage.isNull())
        setDisplayImage(width, height);
}

void imageVisualizer::clearPix()
{ }

void imageVisualizer::setDisplayImage(int width, int height)
{
    if (!dirPixImage.isNull()) {
        QPixmap temp = dirPixImage;
        temp = temp.scaled(width, height, Qt::KeepAspectRatioByExpanding,
                                         Qt::SmoothTransformation);
        imageScene->clear();
        imageScene->addPixmap(temp);
        ui->graphicsView->clearFocus();
        ui->graphicsView->setScene(imageScene);
    }
}

void imageVisualizer::setDisplayImage(QString filename)
{
    if (!filename.isEmpty()) {
       this->dirPixImage.load(filename);
       dirPixImage.scaled(width, height);
       imageScene->clear();
       imageScene->addPixmap(dirPixImage);
       ui->graphicsView->setScene(imageScene);
    }
}

imageVisualizer::~imageVisualizer()
{
    delete ui;
}
