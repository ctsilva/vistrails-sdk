/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef IMAGEVISUALIZER_HPP
#define IMAGEVISUALIZER_HPP
#include "ui_imagevisualizer.h"

#include <QWidget>
#include <QtGui>

//namespace Ui {
    // class imageVisualizer;
//}

class imageVisualizer : public QWidget
{
    Q_OBJECT

public:
    explicit imageVisualizer(QWidget *parent = 0);
    ~imageVisualizer();
    void setDisplayImage(QString filename);

    void clearPix();

protected:
    void resizeEvent(QResizeEvent *event);
private:
    QGraphicsScene *imageScene;
    QPixmap dirPixImage;
    QPixmap originalImage;
    void setDisplayImage(int width, int height);
    Ui::imageVisualizer *ui;
    int width;
    int height;
};

#endif
