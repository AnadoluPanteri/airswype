/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef SWYPEELEMENT_H
#define SWYPEELEMENT_H

#include <QDeclarativeItem>
#include "datatypes.h"

using namespace AS;

/*!
  \class SwypeElement
  \brief Custom QML item that shows target and drawed shape
*/
class SwypeElement : public QDeclarativeItem
{
    Q_OBJECT

public:
    /*!
      Constructor
    */
    SwypeElement(QDeclarativeItem *parent = 0);

    /*!
      Custom paint
    */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget);

protected:
    QPolygonF m_Polygon;

public slots:
    /*!
      Sets final shape for drawing to screen
    */
    void setShape(const Shape &shape);

    /*!
      Sets target shape from db for showing to the user
    */
    void setShapeFromDb(const Shape &shape, float timeToShow);

private:
    /*!
      Scales shape to fit in screen
    */
    void setPolygon(const QPolygonF &polygon);

    /*!
      Fills points to vectors
    */
    QPolygonF fillVectors(QPolygonF &polygon);

    /*!
      Paints target shape
    */
    void paintDbSwype(QPainter *painter);

    /*!
      Paints user drawed shape
    */
    void paintSensorSwype(QPainter *painter);

private:
    enum SwypeState {
        EDatabaseSwype = 0,
        ESensorSwype = 1
    };
    SwypeState      m_state;
    QPixmap         m_pixmap;
    QList<QPixmap>  m_pixmapList;
    int             m_pixmapIndex;
    int             m_drawCounter;
    int             m_pixmapStartIndex;

    QPolygonF       m_filledPolygon;
    int             m_circleIndex;
    int             m_circleCount;

    qreal           m_scale;
    QRectF          m_polygonRect;
    QPointF         m_polygonCenter;

};

#endif // SWYPEELEMENT_H
