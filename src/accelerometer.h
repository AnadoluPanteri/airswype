/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <QAccelerometer>
#include <QPolygonF>

QTM_USE_NAMESPACE

class QElapsedTimer;
namespace AS {
    class Coordinate;
    class Shape;
}

using namespace AS;

/*!
  \class Accelerometer
  \brief Represents the accelerometer sensor. Controls the starting and
         stopping of the sensor. Contains the algorithm to get the absolute
         positions from measurements.
*/
class Accelerometer : public QAccelerometer
{
    Q_OBJECT
public:
    /*!
      Constructor
    */
    Accelerometer(QObject *parent = 0);
    virtual ~Accelerometer();

    /*!
      Stores the coordinate to into the shape. Handles the calibration,
      calculation of the relative and absolute position of the coordinate
      as well as the velocity.
    */
    void storeCoordinate(const Coordinate &measure);

public slots:

    /*!
      Starts the accelerometer sensor. Removes the earlier measured shape.
    */
    virtual bool start();

    /*!
      Stops the accelerometer sensor.
    */
    virtual void stop();

signals:
    /*!
      Emitted after each new coordinate has been processed.
    */
    void shapeData(Shape &shape);

    /*!
      Emitted after the accelerometer sensor has been calibrated
      and the actual measurements begins.
    */
    void calibrationDone();

protected:

    // Counter for each component to calculate the end of movement
    int m_ZeroAccCounter[3];

    // Keeps the value of calculated gravity
    Coordinate *m_Grav;

    // Shape to store the measured coordinates.
    Shape *m_UserShape;
};

#endif // ACCELEROMETER_H
