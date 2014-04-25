/*
 * Copyright (c) 2011-2014 Microsoft Mobile.
 */

#include <QtCore>
#include <math.h>
#include "accelerometer.h"
#include "accelerometerfilter.h"
#include "datatypes.h"

using namespace AS;

Accelerometer::Accelerometer(QObject *parent)
    : QAccelerometer(parent),
      m_Grav(NULL),
      m_UserShape(NULL)
{
}


Accelerometer::~Accelerometer()
{
    if(m_UserShape) {
        delete m_UserShape;
        m_UserShape = NULL;
    }

    if (m_Grav) {
        delete m_Grav;
        m_Grav = NULL;
    }
}


void Accelerometer::storeCoordinate(const Coordinate &c)
{
    Coordinate m1 = c;

    if (!m_Grav) {

        // Calculate the gravity using average of the values
        const int gravMeasuresMax = 60;

        static Coordinate gravMeasures[gravMeasuresMax];
        static int gravMeasureCount = 0;

        gravMeasures[gravMeasureCount] = m1;
        gravMeasureCount++;

        if(gravMeasureCount == gravMeasuresMax) {
            m_Grav = new Coordinate;

            for (int m=0; m < gravMeasureCount; m++) {
                for(int i=0; i<3; i++) {
                    m_Grav->acc[i] += gravMeasures[m].acc[i];
                }
            }

            for (int i=0; i<3; i++) {
                m_Grav->acc[i] /= gravMeasureCount;
            }

            // Signalize that the calibration is done
            emit calibrationDone();

            gravMeasureCount = 0;
        }
    }
    else {
        // Substract the gravity, notice that the prerequisite is
        // that the phone is held in same orientation through the
        // whole measurement process.
        for (int i=0; i<3; i++) {
            m1.acc[i] -= m_Grav->acc[i];
        }

        // Implement discrimination window to reduce mechanical noise
        for (int i=0; i<3; i++) {
            if(qAbs(m1.acc[i]) < 1.0) {
                m1.acc[i] = 0.0;
            }
        }

        if (m_UserShape->coordinates.isEmpty()) {
            // First real measurement, just put it in the container
            m_UserShape->coordinates.push_back(m1);
        }
        else {
            // The earlier measurement is found on the end of the measurements
            // list
            Coordinate m0(m_UserShape->coordinates.last());

            // Relatime time in seconds (converted from microseconds)
            m1.relTime = (m1.time - m0.time) * 0.000001;


            for (int i=0; i<3; i++) {
                // Calculate speeds
                if(m1.acc[i] == 0) {
                    // Component has zero acceleration, increase the count
                    // how many sequential measurements the component has
                    // been zero
                    m_ZeroAccCounter[i]++;
                }
                else {
                    // Component is no longer still, reset the counter
                    m_ZeroAccCounter[i] = 0;
                }

                if(m_ZeroAccCounter[i] > 10) {
                    // Component has been zero too long, conclusion:
                    // the phone is not moving => zero the speed
                    m1.sp[i] = 0;
                }
                else {
                    // Normal behavior, the phone is moving, calculate
                    // the speed
                    m1.sp[i] = m0.sp[i] + m1.relTime * m0.acc[i] + 0.5
                          * (m1.acc[i] - m0.acc[i]) * m1.relTime;
                }

                // Calculate relative positions
                m1.relPos[i] = pow(m1.relTime, 2)
                        * ((4 * m1.acc[i] - m0.acc[i]) / 6)
                        + m0.sp[i] * m1.relTime;

                // Calculate the absolute positions
                m1.pos[i] = m0.pos[i] + m1.relPos[i];
            }

            // Store the value to the QList.
            m_UserShape->coordinates.push_back(m1);

            emit shapeData(*m_UserShape);
        }
    }
}


bool Accelerometer::start()
{
    if(m_UserShape) {
        delete m_UserShape;
        m_UserShape = NULL;
    }

    if (m_Grav) {
        delete m_Grav;
        m_Grav = NULL;
    }

    memset(m_ZeroAccCounter, 0, sizeof(int) * 3);
    m_UserShape = new Shape();

    return QAccelerometer::start();
}


void Accelerometer::stop()
{
    QAccelerometer::stop();
}
