/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef DATATYPES_H
#define DATATYPES_H

#include <memory.h>
#include <QtAlgorithms>
#include <QtGlobal>
#include <QList>

namespace AS {

/*!
  \class Coordinate
  \brief Datatype
*/
class Coordinate
{
public:
    // All arrays have components x, y, z in that order.

    // Acceleration, m/s2
    qreal acc[3];

    // Speed, m/s
    qreal sp[3];

    // Absolute position, m
    qreal pos[3];

    // Relative position, m
    qreal relPos[3];

    // For analyzing purposes:
    // how much we have travelled from the previous point
    qreal travelledDistance;

    // Timestamp microseconds, don't know what based
    // this value is but it isn't really important for us
    qulonglong time;

    // Relative timestamp, s
    qreal relTime;


    Coordinate( qreal x, qreal y, qreal z ) {
        clear();
        pos[0] = x;
        pos[1] = y;
        pos[2] = z;
    }

    Coordinate() {
        // Initial values 0 for all members.
        clear();
    }

    // Checks if two Coordinates are really close to each other
    bool isSamePos(const Coordinate &m) const {
        for(int i=0; i<3; i++) {
            if(qFuzzyCompare(pos[i], m.pos[i]) == false) {
                return false;
            }
        }
        return true;
    }

    // Zeroes all of the member variables
    void clear() {
        memset(this, 0, sizeof(Coordinate));
    }
};


/*!
  \class Shape
  \brief Datatype
*/
class Shape
{
public:
    // Shorten the writing of type later
    typedef QList<Coordinate> Coordinates;

    // List of the coordinates
    Coordinates coordinates;

    // Difficulty level of the shape
    int level;

    // Time in seconds how long to show the shape
    qreal timeToShow;

    // Will be marked true when analyzer has preanalyzed this shape.
    // NOTE, if the shape changes, this flag MUST be set to false.
    bool shapePreAnalyzed;

    // Total distance (in abstract units) travelled in this shape.
    qreal totalDistanceTravelled;

    // Bounds, will be filled by the formanalyzer
    float minx, maxx, miny, maxy;

    Shape()
        : level(0),
          timeToShow(0.0),
          shapePreAnalyzed(false),
          totalDistanceTravelled(0.0f),
          minx(0.0f),
          maxx(0.0f),
          miny(0.0f),
          maxy(0.0f)
    {
        // Constructor
    }

    /*!
      Sorts the coordinates by z value, by this way we
      can make fake 3D model of the coordinates in drawing.
    */
    void sortByZ() {
        qSort(coordinates.begin(), coordinates.end(), sortByZFunction);
    }

    /*!
      Flips the shape around y-axis.
    */
    void flipHorizontally() {
        Shape::Coordinates::Iterator it;
        for (it = coordinates.begin(); it != coordinates.end(); it++) {
            it->pos[0] *= -1.0f;
        }
    }

    /*!
      Flips the shape around x-axis.
    */
    void flipVertically() {
        Shape::Coordinates::Iterator it;
        for (it = coordinates.begin(); it != coordinates.end(); it++) {
            it->pos[1] *= -1.0f;
        }
    }

    /*!
      Rotates the shape 90 degrees.
    */
    void rotate90Degrees() {
        Shape::Coordinates::Iterator it;
        for (it = coordinates.begin(); it != coordinates.end(); it++) {
            float temp = it->pos[0];
            it->pos[0] = -it->pos[1];
            it->pos[1] = temp;
        }
    }

protected:
    static bool sortByZFunction(const Coordinate &c1, const Coordinate &c2)
    {
        if(c1.pos[3] > c2.pos[3]) {
            return true;
        }
        return false;
    }
};

} //namespace as


#endif // DATATYPES_H
