/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include <QtGui>
#include "swypeelement.h"

SwypeElement::SwypeElement(QDeclarativeItem *parent)
    : QDeclarativeItem(parent)
{
    m_pixmapIndex = 0;
    m_drawCounter = 0;
    m_pixmapStartIndex = 0;

    // Important, otherwise the paint method is never called
    setFlag(QGraphicsItem::ItemHasNoContents, false);

    QPixmap pixmap;
    pixmap.load(":/gfx/cloud_small1.png");
    m_pixmapList.append(pixmap);

    pixmap.load(":/gfx/cloud_small2.png");
    m_pixmapList.append(pixmap);

    pixmap.load(":/gfx/cloud_small3.png");
    m_pixmapList.append(pixmap);

    pixmap.load(":/gfx/cloud_small4.png");
    m_pixmapList.append(pixmap);

    pixmap.load(":/gfx/cloud_small3.png");
    m_pixmapList.append(pixmap);

    pixmap.load(":/gfx/cloud_small2.png");
    m_pixmapList.append(pixmap);

    m_pixmap.load(":/gfx/cloud_small1.png");
}

void SwypeElement::paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                         QWidget *)
{
    Q_UNUSED(option);
    painter->setRenderHint(QPainter::Antialiasing, true);

    if (m_state == SwypeElement::EDatabaseSwype) {
        paintDbSwype(painter);
    } else {
        paintSensorSwype(painter);
    }
}

void SwypeElement::paintDbSwype(QPainter *painter)
{
    // Draw lines
    QPen pen = QPen(Qt::red);
    pen.setStyle(Qt::DashLine);
    pen.setWidth(10);
    painter->setPen(pen);
    for (int i=0;i<m_Polygon.count()-1;i++)
    {
        painter->drawLine(m_Polygon.at(i),m_Polygon.at(i+1));
    }

    // Draw start circle
    pen.setStyle(Qt::SolidLine);
    painter->setPen(pen);
    if (m_circleIndex < m_filledPolygon.count())
        painter->drawEllipse(m_filledPolygon.at(m_circleIndex),15,15);

    if (m_circleCount < 0) {
        m_circleIndex++;
        if (m_circleIndex > m_filledPolygon.count()-1)
            m_circleIndex = 0;
        m_circleCount = 1;
    }
    m_circleCount--;

}

void SwypeElement::paintSensorSwype(QPainter *painter)
{
    if (m_drawCounter==10) {
        m_pixmapStartIndex++;
    }
    m_pixmapIndex = m_pixmapStartIndex;
    if (m_pixmapStartIndex > m_pixmapList.count()-1){
        m_pixmapStartIndex = 0;
    }

    const QPainter::CompositionMode mode = painter->compositionMode();

    // Draw cluds
    for (int i=0;i<m_Polygon.count();i++)
    {
        QPointF pos = m_Polygon.at(i);
        if (m_pixmapIndex > m_pixmapList.count()-1){
            m_pixmapIndex = 0;
        }
        QPixmap pixmap = m_pixmapList[m_pixmapIndex];
        QPoint pixPoint = QPoint(pos.x()-pixmap.width() * 0.5, pos.y()-pixmap.height() * 0.5);
        painter->setCompositionMode(QPainter::CompositionMode_Plus);
        painter->drawPixmap(pixPoint,pixmap);
        m_pixmapIndex++;
    }
    painter->setCompositionMode(mode);

    m_drawCounter++;
    if (m_drawCounter>10){
        m_drawCounter = 0;
    }

}

QPolygonF SwypeElement::fillVectors(QPolygonF &polygon)
{
    QPolygonF newFilledPolygon;

    if (polygon.count()>1)
    {
        for(int i=0;i<polygon.count()-1;i++)
        {
            QPointF p0 = polygon[i];
            QPointF p1 = polygon[i+1];
            QPointF directionVector(p1.x() - p0.x(), p1.y() - p0.y());
            float multiplier = 0;
            // =  1 / 40 = 0.025
            // =  1 / 30 = 0.03333
            // =  1 / 20 = 0.05
            for(int j=0;j<20;j++)
            {
                QPointF newP = p0 + multiplier * directionVector;
                newFilledPolygon.append(newP);
                multiplier += 0.05;
            }
        }
        return newFilledPolygon;
    }
    else {
        newFilledPolygon = polygon;
        return newFilledPolygon;
    }
}


void SwypeElement::setShapeFromDb(const Shape &shape, float timeToShow)
{
    Q_UNUSED(timeToShow);

    m_state = SwypeElement::EDatabaseSwype;

    // Create QPolygon from Shape
    QPolygonF polygon;
    Shape::Coordinates::ConstIterator it;
    for(it = shape.coordinates.begin(); it != shape.coordinates.end(); it++) {
        // NOTE: no need to invert y-axis here
        polygon.push_back(QPointF(it->pos[0]*100, it->pos[1]*-100));
    }

    // Fill for circle animation
    m_filledPolygon = fillVectors(polygon);
    setPolygon(m_filledPolygon);
    m_filledPolygon = m_Polygon;
    m_circleIndex = 0;
    m_circleCount = 1;

    // Used polygon for red line and smoke
    setPolygon(polygon);

    // Draw polygon
    update();
}


void SwypeElement::setShape(const Shape &shape)
{
    m_state = SwypeElement::ESensorSwype;

    // Create QPolygon from Shape
    QPolygonF polygon;
    Shape::Coordinates::ConstIterator it;
    for(it = shape.coordinates.begin(); it != shape.coordinates.end(); it++) {
        // Invert the y-axis
        polygon.push_back(QPointF(it->pos[0]*100, it->pos[1]*-100));
    }

    // Used polygon for white smoke line
    setPolygon(polygon);

    // Draw polygon
    update();
}


void SwypeElement::setPolygon(const QPolygonF &polygon)
{
    m_Polygon = polygon;
    m_polygonRect = m_Polygon.boundingRect();
    m_scale = 1.0;

    qreal widthScale = 1.0;
    qreal heightScale = 1.0;

    QRectF rect = this->boundingRect();
    rect.adjust(m_pixmap.width()*0.5,
                m_pixmap.height()*0.5,
                m_pixmap.width()*-0.5,
                m_pixmap.height()*-0.5);

    // Scale?
    if (m_polygonRect.width() > 0 && m_polygonRect.height() > 0)
    {
        widthScale = rect.width() / m_polygonRect.width();
        heightScale = rect.height() / m_polygonRect.height();
        widthScale < heightScale ? m_scale = widthScale : m_scale = heightScale;
    }
    else {
        if (m_polygonRect.width() > 0)
        {
            m_scale = rect.width() / m_polygonRect.width();
        }
        else {
            m_scale = rect.height() / m_polygonRect.height();
        }
    }

    // Scale
    QMatrix matrix;
    matrix = matrix.translate(m_polygonRect.width()*0.5,m_polygonRect.height()*0.5);
    matrix.scale(m_scale,m_scale);
    matrix = matrix.translate(m_polygonRect.width()*-0.5,m_polygonRect.height()*-0.5);
    m_Polygon = matrix.map(m_Polygon);
    m_polygonRect = m_Polygon.boundingRect();

    // Move polygon to center of the screen
    m_polygonCenter = m_polygonRect.center();
    QPointF widgetCenter = rect.center();
    int transX = (widgetCenter.x() - m_polygonCenter.x());
    int transY = (widgetCenter.y() - m_polygonCenter.y());
    m_Polygon.translate(transX,transY);

    m_polygonRect = m_Polygon.boundingRect();
    m_polygonCenter = m_polygonRect.center();
}


QML_DECLARE_TYPE(SwypeElement)
