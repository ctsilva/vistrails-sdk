/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "galleryproxy.hpp"
#include "gallerylabel.hpp"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QScrollBar>
#include <QtCore>

ImageProxyWidget::ImageProxyWidget(QGraphicsItem * parent, Qt::WindowFlags wFlags)
    : QGraphicsProxyWidget(parent,wFlags),popupShown(false)
{
    timelineImageAnimation = new QTimeLine(100, this);
    connect(timelineImageAnimation, SIGNAL(frameChanged(int)),
            this, SLOT(updateStep(int)));
    connect(timelineImageAnimation, SIGNAL(stateChanged(QTimeLine::State)),
            this, SLOT(stateChanged(QTimeLine::State)));
    connect(timelineImageAnimation, SIGNAL(finished ()),
            this, SLOT(animationFinished()));

    timelineImageAnimation->setFrameRange(20,90);
}


ImageProxyWidget::~ImageProxyWidget()
{
    delete timelineImageAnimation;
}


QRectF ImageProxyWidget::boundingRect() const
{
    return QGraphicsProxyWidget::boundingRect().adjusted(0, 0, 0, 0);
}


void ImageProxyWidget::paintWindowFrame(QPainter *, const QStyleOptionGraphicsItem *,
					QWidget *)
{ }


/*
  keyPressEvent(QKeyEvent)
*/
void ImageProxyWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
	if (popupShown && (timelineImageAnimation->direction() != QTimeLine::Backward || timelineImageAnimation->currentValue() != 0))
	    zoomOut();
    }
}


/*
  closeEvent(QCloseEvent*)
*/
void ImageProxyWidget::closeEvent(QCloseEvent * event)
{
    event->ignore();
}


/*
  mousePressEvent(QGraphicsSceneMouseEvent)
  Handles left click for the gallery-view animation
*/
void ImageProxyWidget::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() != Qt::LeftButton)
        return;
    scene()->setActiveWindow(this);
    if(!popupShown) {
        if (timelineImageAnimation->currentValue() != 1)
            zoomIn();
    }
    else
        zoomOut();
}


/*
  hoverLeaveEvent(QGraphicsSceneHoverEvent)
  Handles move out of image space event
*/
void ImageProxyWidget::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    // QGraphicsProxyWidget::hoverLeaveEvent(event);
    // if (popupShown && (timelineImageAnimation->direction() != QTimeLine::Backward || timelineImageAnimation->currentValue() != 0))
    //   zoomOut();
}


/*
  setDefaultItemGeometry(QRectF&)
  Set each item geometry
*/
void ImageProxyWidget::setDefaultItemGeometry(const QRectF &geometry)
{
    itemGeometry = geometry;
}


/*
  updateStep(int)
  Handles size of each pixmap geometry on update
*/
void ImageProxyWidget::updateStep(int step)
{
    qreal newWidth = this->scene()->views().at(0)->geometry().width() * step / 90;
    qreal newHeight = this->scene()->views().at(0)->geometry().height() * step / 90 ;

    if(timelineImageAnimation->direction() == QTimeLine::Backward) {
        if(this->scene()->views().at(0)->verticalScrollBar() != NULL) {
            this->scene()->views().at(0)->verticalScrollBar()->setValue(this->scene()->views().at(0)->verticalScrollBar()->value());
        }

	this->setGeometry(QRectF(this->geometry().x()+30,
				 this->geometry().y(),
				 newWidth,
				 newHeight));

        emit imageZoomedOut();
    }
    else {
        this->setGeometry(QRectF(this->geometry().x() - 30.5,
				 this->geometry().y(), 
				 newWidth, newHeight));
    }

    this->scene()->setSceneRect(this->scene()->itemsBoundingRect());
    this->scene()->views().at(0)->ensureVisible(this,0,0);
}


/*
  stateChanged(QTimeline)
  Check the current state of the selected/clicked image
*/
void ImageProxyWidget::stateChanged(QTimeLine::State state)
{
    if (state == QTimeLine::Running)
    {
        if (timelineImageAnimation->direction() == QTimeLine::Forward)
            setCacheMode(DeviceCoordinateCache);
    }
    else if (state == QTimeLine::NotRunning)
    {
        if (timelineImageAnimation->direction() == QTimeLine::Backward)
            setCacheMode(DeviceCoordinateCache);
    }
}


/*
  zoomIn()
  Check if image is zoomedIn
*/
void ImageProxyWidget::zoomIn()
{
    popupShown =true;
    if (timelineImageAnimation->direction() != QTimeLine::Forward)
        timelineImageAnimation->setDirection(QTimeLine::Forward);
    if (timelineImageAnimation->state() == QTimeLine::NotRunning)
        timelineImageAnimation->start();

}


/*
  zoomOut()
  Check if image is zoomedIn
*/
void ImageProxyWidget::zoomOut()
{
    popupShown =false;
    if (timelineImageAnimation->direction() != QTimeLine::Backward)
        timelineImageAnimation->setDirection(QTimeLine::Backward);
    if (timelineImageAnimation->state() == QTimeLine::NotRunning)
        timelineImageAnimation->start();

}


/*
  animationFinished()
  Check if small animation is done performing
*/
void ImageProxyWidget::animationFinished()
{
    if(timelineImageAnimation->direction() == QTimeLine::Backward)
    {
        if(this->scene()->views().at(0)->verticalScrollBar() != NULL)
        {
            this->scene()->views().at(0)->verticalScrollBar()->setValue(this->scene()->views().at(0)->verticalScrollBar()->maximum());
        }
        emit imageZoomedOut();
        this->setGeometry(itemGeometry);
    }

    if(timelineImageAnimation->direction() == QTimeLine::Forward)
    {
        this->scene()->setSceneRect(this->scene()->itemsBoundingRect());
        this->scene()->views().at(0)->ensureVisible(this,0,0);
        emit imageZoomedIn();
    }
}
