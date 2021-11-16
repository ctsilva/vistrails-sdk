/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef GALLERYVIEW_HPP
#define GALLERYVIEW_HPP

#include <QGraphicsView>
#include <QtGui>

struct ScaledImageInfo
{
    QImage iScaledImage;
    QString scaledImagePath;
};

class imgGallery : public QGraphicsView
{
    Q_OBJECT
public:
    imgGallery(QWidget *parent = 0);
    virtual ~imgGallery();
    void loadImages(QStringList list);
    enum zoomFocus { IN, OUT };

public slots:
    void changeZoomLevelByFactor(zoomFocus zoom);
    void cleanAllResults();

private slots:
    void showImageOnWall(int num);
protected:
    virtual void wheelEvent(QWheelEvent *);
    virtual void mousePressEvent(QMouseEvent *);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);

private:
    int iRow;
    int iColumn;
    int iMouseDeltaValueForPressedEvent;
    bool bMouseButtonPressed;
    QFutureWatcher<ScaledImageInfo> *imageScaling;
    QGraphicsScene graphicsScene;
    qreal zoomFactor;
};

#endif


