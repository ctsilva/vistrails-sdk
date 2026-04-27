/*
 * Copyright (c) 2009-2011 VisTrails, Inc..
 *
 * This file is part of VisTrails SDK
 * (see https://github.com/vistrails/sdk).
 *
 * License: 3-clause BSD, see https://opensource.org/licenses/BSD-3-Clause
 */

#include "galleryview.hpp"

#include "gallerylabel.hpp"
#include "galleryproxy.hpp"

#include <QUrl>
#include <QFileInfo>
#include <QScrollBar>
#include <QtConcurrent/QtConcurrent>

int mainWidth = 0;
int mainHeight = 0;

int ImageWidth = 0;
int ImageHeight = 0;

ScaledImageInfo MyScale(const QString &imageFileName)
{
    ImageWidth = (int)(mainWidth*120) / 100;
    ImageHeight = (int)(mainHeight*80) /100;

    ScaledImageInfo result;
    QImage image(imageFileName);
    result.iScaledImage = image.scaled(QSize(ImageWidth, ImageHeight), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    result.scaledImagePath = imageFileName;
    return result;
}


imgGallery::imgGallery(QWidget *parent)
    : QGraphicsView(parent),iRow(0),iColumn(0)
{
    imageScaling = new QFutureWatcher<ScaledImageInfo>(this);

    zoomFactor = 1;
    bMouseButtonPressed = false;

    this->setScene(&graphicsScene);

    this->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform | QPainter::TextAntialiasing);
    connect(imageScaling, SIGNAL(resultReadyAt(int)), SLOT(showImageOnWall(int)));
    
}


/*
  cleanAllResults()
  Clear current gallery view
*/
void imgGallery::cleanAllResults()
{
    imageScaling->cancel();
    imageScaling->waitForFinished();
    graphicsScene.clear();
    if (this->horizontalScrollBar() != NULL)
    {
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->minimum());
    }

    graphicsScene.setSceneRect(QRectF(0,0,0,0));

    iRow = 0;
    iColumn = 0;
}


/*
  loadImages()
  At the moment I'm loading a local folder for the gallery-view
  The idea is to have this view, and show the children of the
  current selected item here. With the ability to zoom-in/zoom-out
*/
void imgGallery::loadImages(QStringList fileList)
{
    cleanAllResults();

    //Estimation
    mainWidth = this->geometry().width()-50;
    mainHeight = this->geometry().height()/7;

    if (fileList.isEmpty())
        return;

    imageScaling->setFuture(QtConcurrent::mapped(fileList, MyScale));
}


/*
  showImageOnWall(int)
  Display each pixmap on the galleryview wall
*/
void imgGallery::showImageOnWall(int num)
{
    ImageProxyWidget *pImageProxyWidgetInstance = new ImageProxyWidget(0, Qt::Window);
    CustomLabel *imageItem = new CustomLabel();

    ScaledImageInfo result = imageScaling->resultAt(num);

    QPixmap pix = QPixmap::fromImage(result.iScaledImage);
    QString pixLoc = result.scaledImagePath;

    pix = pix.scaled(QSize(50,45));

    imageItem->setPixmap(pix);
    imageItem->setImagePath(pixLoc);

    if (iRow >= 5)
    {
        iRow = 0;
        iColumn++;
    }

    QRectF rect = pImageProxyWidgetInstance->boundingRect();
    rect.setWidth(ImageWidth+10);
    rect.setHeight(ImageHeight+15);

    pImageProxyWidgetInstance->setWidget(imageItem);
    pImageProxyWidgetInstance->setPos(iColumn * rect.width() * 0.31, iRow * rect.height() * 1.1);
    pImageProxyWidgetInstance->setDefaultItemGeometry(pImageProxyWidgetInstance->geometry());
    graphicsScene.addItem(pImageProxyWidgetInstance);
    graphicsScene.setSceneRect(graphicsScene.itemsBoundingRect());

    QObject::connect(pImageProxyWidgetInstance,SIGNAL(imageZoomedIn()),imageItem,SLOT(imageZoomedIn()));
    QObject::connect(pImageProxyWidgetInstance,SIGNAL(imageZoomedOut()),imageItem,SLOT(imageZoomedOut()));

    iRow++;
}


/*
  changeZoomLevelByFactor
  TODO: The zoom in-zoom out is not complete yet, but it works.
*/
void imgGallery::changeZoomLevelByFactor(zoomFocus zoom)
{
    if (zoom == IN)
    {
        zoomFactor = zoomFactor + 0.25;
        if (zoomFactor > 5)
        {
            zoomFactor = 1;
        }
    }
    else
    {
        zoomFactor = zoomFactor - 0.25;
        if ((zoomFactor - 0.25) < 0.25)
        {
            zoomFactor = 0.25;
        }
    }
    this->setTransform(QTransform()
                       .translate(this->contentsRect().width(), this->contentsRect().height())
                       .scale(zoomFactor, zoomFactor)
                       .translate(-this->contentsRect().width(), -this->contentsRect().height()));
}


/*
  wheelEvent(QWheelEvent)
  Handles mouse wheel -> ZoomedIn/ZoomedOut
*/
void imgGallery::wheelEvent(QWheelEvent *e)
{
    int numDegrees = e->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;

    if (numSteps > 0)
    {
        changeZoomLevelByFactor(imgGallery::IN);
    }
    else if (numSteps < 0)
    {
        changeZoomLevelByFactor(imgGallery::OUT);
    }

    QGraphicsView::wheelEvent(e);
}


/*
  mousePressEvent(QMouseEvent)
  Handles mouse left button click
*/
void imgGallery::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        iMouseDeltaValueForPressedEvent = e->position().x();
        bMouseButtonPressed = true;
    }
    QGraphicsView::mousePressEvent(e);
}


/*
  mouseReleaseEvent(QMouseEvent)
  Handles mouse release event
*/
void imgGallery::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        bMouseButtonPressed = false;
    }

    QGraphicsView::mouseReleaseEvent(e);
}


/*
  mouseMoveEvent(QMouseEvent)
  Handles mouse-move event
*/
void imgGallery::mouseMoveEvent(QMouseEvent *e)
{
    int direction;
    this->setCursor(Qt::ArrowCursor);

    if (bMouseButtonPressed)
    {
        direction = iMouseDeltaValueForPressedEvent - e->position().x();
        this->horizontalScrollBar()->setValue(this->horizontalScrollBar()->value() + direction);
        iMouseDeltaValueForPressedEvent = e->position().x();
    }

    QGraphicsView::mouseMoveEvent(e);
}


imgGallery::~imgGallery()
{
    delete imageScaling;
}
