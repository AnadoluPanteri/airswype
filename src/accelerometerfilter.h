/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#ifndef ACCELEROMETERFILTER_H
#define ACCELEROMETERFILTER_H

#include <QAccelerometerFilter>

class QAcceleromterReading;
class Accelerometer;

QTM_USE_NAMESPACE

/*!
  \class AccelerometerFilter
  \brief Handles the values received from accelerometer sensor and applies
         low pass filtering to them.
*/
class AccelerometerFilter : public QAccelerometerFilter
{
public:
    /*!
      Constructor
    */
    explicit AccelerometerFilter(Accelerometer *sensor);

    /*!
      Derived method to apply low pass filtering to the measured values.
      The reading will be stored to Coordinate variable and passed to
      m_Sensor.
    */
    bool filter(QAccelerometerReading *reading);

protected:
    // Used to store the values to the sensor
    Accelerometer *m_Sensor;
};

#endif // ACCELEROMETERFILTER_H
