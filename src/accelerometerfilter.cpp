/*
 * Copyright (c) 2011 Nokia Corporation.
 */

#include "accelerometerfilter.h"
#include "datatypes.h"
#include "accelerometer.h"

AccelerometerFilter::AccelerometerFilter(Accelerometer *sensor)
    : m_Sensor(sensor)
{
}


bool AccelerometerFilter::filter(QAccelerometerReading *reading)
{
    static bool firstTime = true;
    static Coordinate lowPass;
    const qreal newReadingWeight = 0.9;

    if(firstTime) {
        lowPass.acc[0] = reading->x();
        lowPass.acc[1] = reading->y();
        lowPass.acc[2] = reading->z();
        lowPass.time = reading->timestamp();
        firstTime = false;
    }
    else {
        // Low pass filter to reduce spikes and noise from readings
        lowPass.acc[0] = lowPass.acc[0] * (1 - newReadingWeight) +
                         newReadingWeight * reading->x();
        lowPass.acc[1] = lowPass.acc[1] * (1 - newReadingWeight) +
                         newReadingWeight * reading->y();
        lowPass.acc[2] = lowPass.acc[2] * (1 - newReadingWeight) +
                         newReadingWeight * reading->z();
        lowPass.time = reading->timestamp();
    }

    m_Sensor->storeCoordinate(lowPass);

    return false;
}
