/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#ifndef GALLERYPROXY_HPP
#define GALLERYPROXY_HPP

#include <QGraphicsProxyWidget>
#include <QTimeLine>

class ImageProxyWidget : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    ImageProxyWidget(QGraphicsItem *parent = nullptr, Qt::WindowFlags wFlags = {});
    virtual ~ImageProxyWidget();
    QRectF boundingRect() const;
    void setDefaultItemGeometry(const QRectF &geometry);

protected:
    void paintWindowFrame(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void updateStep(int step);
    void stateChanged(QTimeLine::State);
    void zoomIn();
    void zoomOut();
    void animationFinished();

signals:
    void imageZoomedIn();
    void imageZoomedOut();

private:
    QTimeLine *timelineImageAnimation;
    QRectF itemGeometry;
    bool popupShown;
};

#endif
