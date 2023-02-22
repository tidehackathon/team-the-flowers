#include "montion_detection.hpp"


/* *** PUBLIC *** */
// Constructor
MontionDetection::MontionDetection()
{
    this->samplesCount = 0;
}

/* *** Velocity Update Detection *** */
bool MontionDetection::zeroVelocityUpdate(double *acceleration, double *angularVelocity)
{
    if (abs(acceleration[0]) <= STATIC_ACCELERATION_THRESHOLD &&
        abs(acceleration[1]) <= STATIC_ACCELERATION_THRESHOLD &&
        abs(acceleration[2]) <= STATIC_ACCELERATION_THRESHOLD &&
        abs(angularVelocity[0]) <= STATIC_ANGULAR_VELOCITY_THRESHOLD &&
        abs(angularVelocity[1]) <= STATIC_ANGULAR_VELOCITY_THRESHOLD &&
        abs(angularVelocity[2]) <= STATIC_ANGULAR_VELOCITY_THRESHOLD)
    {
        this->samplesCount++;
    }
    else
    {
        this->samplesCount = 0;
    }

    return (this->samplesCount >= STATIC_SAMPLES_THRESHOLD);
}