/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "galleryimageholder.hpp"
#include <QFileInfo>

ImageHolder::ImageHolder(QWidget *parent)
    : QWidget(parent)
{
    setupUi(this);
}

ImageHolder::~ImageHolder()
{

}

/*
  setPixmap(QPixmap)
  Add pixmap to label
*/
void ImageHolder::setPixmap(const QPixmap &imageItem)
{
    lblPixmapHolder->setPixmap(imageItem);
}

/*
  setImagePath(QString&)
  Add path to pixmap
*/
void ImageHolder::setImagePath(const QString& path)
{
    sImagePath = path;
}

/*
  pixmap()
  Return current pixmap
*/
QPixmap ImageHolder::pixmap() const
{
    return lblPixmapHolder->pixmap();
}

/*
  setOriginalPixmap
  Set pixmap
*/
void ImageHolder::setOriginalPixmap()
{
    QImage image(sImagePath);
    lblPixmapHolder->setPixmap(QPixmap::fromImage(
				   image.scaled (QSize(this->width(), 
						       this->height()), 
						 Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}
