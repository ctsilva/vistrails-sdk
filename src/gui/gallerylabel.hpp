/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef GALLERYLABEL_HPP
#define GALLERYLABEL_HPP

#include <QDialog>
#include <QLabel>

#include "galleryimageholder.hpp"

class CustomLabel : public QDialog
{
    Q_OBJECT
public:
    CustomLabel(QWidget *parent = 0 );
    virtual ~CustomLabel();

public slots:
    void setPixmap(const QPixmap& pixmap);
    void setImagePath(const QString& imagePath);
    void imageZoomedIn();
    void imageZoomedOut();
private:
    ImageHolder *imageInstance;
    QPixmap imageVisibleState;
};

#endif
