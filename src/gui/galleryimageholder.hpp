/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef GALLERYIMAGE_HPP
#define GALLERYIMAGE_HPP

#include <QWidget>
#include "ui_imagecontainer.h"

class ImageHolder : public QWidget, protected Ui::ImageContainerFrame
{
Q_OBJECT
public:
    ImageHolder(QWidget *parent = 0);
    virtual ~ImageHolder();
    void setPixmap(const QPixmap &imageItem);
    QPixmap pixmap() const;
    void setOriginalPixmap();
    void setImagePath(const QString& path);

private:
    QString sImagePath;
};

#endif
