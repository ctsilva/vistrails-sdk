/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "gallerylabel.hpp"

#include <QIcon>
#include <QStringList>

#include <QHBoxLayout>
#include <QWheelEvent>

CustomLabel::CustomLabel(QWidget *parent)
    : QDialog(parent)
{
    QHBoxLayout *horizontalLayout = new QHBoxLayout(this);
    imageInstance = new ImageHolder(this);
    horizontalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout->addWidget(imageInstance);
}

CustomLabel::~CustomLabel()
{
    delete imageInstance;
}

/*
  setPixmap(QPixmap&)
  Set current pixmap
*/
void CustomLabel::setPixmap(const QPixmap& image)
{
    imageVisibleState = image;
    imageInstance->setPixmap(image);
}

/*
  setImagePath(QString&)
  Set current path to pixmap
*/
void CustomLabel::setImagePath(const QString& path)
{
    imageInstance->setImagePath(path);
}

/*
  imageZoomedIn()
*/
void CustomLabel::imageZoomedIn()
{
    imageInstance->setOriginalPixmap();
}

/*
  imageZoomedOut()
*/
void CustomLabel::imageZoomedOut()
{
    imageInstance->setPixmap(imageVisibleState);
}
