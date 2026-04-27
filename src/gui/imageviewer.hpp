/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef IMAGEVIEWER_HPP
#define IMAGEVIEWER_HPP

#include <QWidget>
#include <QGraphicsScene>

namespace Ui {
    class imageViewer;
}

class imageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit imageViewer(QWidget *parent = 0);
    void changePixmap();
    void changeWidgetSize();
    void setImageName(QString name);
    ~imageViewer();

private:
    QString imageName;
    QGraphicsScene *scn;
    Ui::imageViewer *ui;
};

#endif
