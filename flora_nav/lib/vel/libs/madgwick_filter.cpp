#include "madgwick_filter.hpp"

/* *** PUBLIC *** */
// Constructor
MagdwickFilter::MagdwickFilter()
{
    this->sampleFrequency = 10.0;
    this->betaDef = 0.5;
    this->beta = betaDef;
    this->q0 = 1.0;
    this->q1 = 0.0;
    this->q2 = 0.0;
    this->q3 = 0.0;
    this->pi = 2 * acos(0.0);
}


/* *** Single iteration of filter *** */
void MagdwickFilter::filterUpdate(double ax, double ay, double az, double gx, double gy, double gz)
{
    double recipNorm, s0, s1, s2, s3, qDot1, qDot2, qDot3, qDot4, _2q0, _2q1, _2q2, _2q3,
        _4q0, _4q1, _4q2, _8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

    qDot1 = 0.5 * (-q1 * gx - this->q2 * gy - this->q3 * gz);
    qDot2 = 0.5 * (this->q0 * gx + this->q2 * gz - this->q3 * gy);
    qDot3 = 0.5 * (this->q0 * gy - this->q1 * gz + this->q3 * gx);
    qDot4 = 0.5 * (this->q0 * gz + this->q1 * gy - this->q2 * gx);

    // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
    if (!((ax == 0.0) && (ay == 0.0) && (az == 0.0)))
    {

        // Normalise accelerometer measurement
        recipNorm = this->invSqrt(ax * ax + ay * ay + az * az);
        ax *= recipNorm;
        ay *= recipNorm;
        az *= recipNorm;

        // Auxiliary variables to avoid repeated arithmetic
        _2q0 = 2.0 * this->q0;
        _2q1 = 2.0 * this->q1;
        _2q2 = 2.0 * this->q2;
        _2q3 = 2.0 * this->q3;
        _4q0 = 4.0 * this->q0;
        _4q1 = 4.0 * this->q1;
        _4q2 = 4.0 * this->q2;
        _8q1 = 8.0 * this->q1;
        _8q2 = 8.0 * this->q2;
        q0q0 = this->q0 * this->q0;
        q1q1 = this->q1 * this->q1;
        q2q2 = this->q2 * this->q2;
        q3q3 = this->q3 * this->q3;

        // Gradient decent algorithm corrective step
        s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
        s1 = _4q1 * q3q3 - _2q3 * ax + 4.0 * q0q0 * this->q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
        s2 = 4.0 * q0q0 * this->q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
        s3 = 4.0 * q1q1 * this->q3 - _2q1 * ax + 4.0 * q2q2 * this->q3 - _2q2 * ay;
        recipNorm = this->invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
        s0 *= recipNorm;
        s1 *= recipNorm;
        s2 *= recipNorm;
        s3 *= recipNorm;

        // Apply feedback step
        qDot1 -= beta * s0;
        qDot2 -= beta * s1;
        qDot3 -= beta * s2;
        qDot4 -= beta * s3;
    }

    // Integrate rate of change of quaternion to yield quaternion
    this->q0 += qDot1 * (1.0 / sampleFrequency);
    this->q1 += qDot2 * (1.0 / sampleFrequency);
    this->q2 += qDot3 * (1.0 / sampleFrequency);
    this->q3 += qDot4 * (1.0 / sampleFrequency);

    // Normalise quaternion
    recipNorm = this->invSqrt(this->q0 * this->q0 + this->q1 * this->q1 + this->q2 * this->q2 + this->q3 * this->q3);
    this->q0 *= recipNorm;
    this->q1 *= recipNorm;
    this->q2 *= recipNorm;
    this->q3 *= recipNorm;

    this->R11 = 2. * this->q0 * this->q0 - 1 + 2. * this->q1 * this->q1;
    this->R21 = 2. * (this->q1 * this->q2 - this->q0 * this->q3);
    this->R31 = 2. * (this->q1 * this->q3 + this->q0 * this->q2);
    this->R32 = 2. * (this->q2 * this->q3 - this->q0 * this->q1);
    this->R33 = 2. * this->q0 * this->q0 - 1 + 2. * this->q3 * this->q3;
}

double MagdwickFilter::invSqrt(double number) 
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;           // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1); // what the fuck?
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
                                         //      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}


/* *** Getters *** */
double MagdwickFilter::getRoll()
{
    double phi = atan2(R32, R33);
    return (phi * 180) / this->pi;
}

double MagdwickFilter::getPitch()
{
    double theta = -atan(R31 / sqrt(1 - R31 * R31));
    return (theta * 180) / this->pi;
}

double MagdwickFilter::getYaw()
{
    double psi = atan2(R21, R11);
    return (psi * 180) / this->pi;
}

std::vector<double> MagdwickFilter::getQuaternions()
{
    return {this->q0, this->q1, this->q2, this->q3};
}
