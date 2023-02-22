#include <iostream>
#include <stdint.h>
#include <cstdint>
#include <math.h>
#include <vector>

#include "vel_calc.hpp"

double integrate(double x1, double x2, double y1, double y2)
{
    double value;
    value = (y1 + y2) * (x2 - x1) * 0.5 * 0.001;
    return value;
}

/*
    INPUT:
        1           2           3
        prev_gx,    prev_gy,    prev_gz
        4           5           6
        prev_ax,    prev_ay,    prev_az
        7           8           9
        ax,         ay,         az
*/
int main(int argc, char **argv)
{
    /* Input parse */
    double prev_gx = atof(argv[1]);
    double prev_gy = atof(argv[2]);
    double prev_gz = atof(argv[3]);
    double prev_ax = atof(argv[4]);
    double prev_ay = atof(argv[5]);
    double prev_az = atof(argv[6]);
    double ax = atof(argv[7]);
    double ay = atof(argv[8]);
    double az = atof(argv[9]);

    // Madgwick Filter
    //  Input: gyro_data, acc_data
    MagdwickFilter *filter = new MagdwickFilter();

    filter->filterUpdate(prev_ax, prev_ay, prev_az, prev_gx, prev_gy, prev_gz);

    double yawAngle = filter->getYaw();
    double pitchAngle = filter->getPitch();
    double rollAngle = filter->getRoll();

    // Gravity Compensation for previous acceleration
    //  Input: prev_acc_data, madgwickFilter.getQuaternions()
    //  Output: prev_compensatedGravity
    double *prev_acc = new double[3];
    prev_acc[0] = prev_ax;
    prev_acc[1] = prev_ay;
    prev_acc[2] = prev_az;

    double *prev_compensatedGravity = compensateGravity(prev_acc, filter->getQuaternions());

    // Gravity Compensation
    //  Input:  acc_data, madgwickFilter.getQuaternions()
    //  Output: compensatedGravity
    double *acc = new double[3];
    acc[0] = ax;
    acc[1] = ay;
    acc[2] = az;

    double *compensatedGravity = compensateGravity(acc, filter->getQuaternions());

    // Montion Detection
    //  Input:  compensatedGravity, gyro_data
    //  Output: montion detected or not : bool
    MontionDetection *montionDetector = new MontionDetection();

    double velocityX, velocityY, velocityZ;
    velocityX = integrate(0, 0.1, prev_compensatedGravity[0], compensatedGravity[0]);
    velocityY = integrate(0, 0.1, prev_compensatedGravity[1], compensatedGravity[1]);
    velocityZ = integrate(0, 0.1, prev_compensatedGravity[2], compensatedGravity[2]);

    double velocity;
    velocity = sqrt(pow(2, velocityX) + pow(2, velocityY) + pow(2, velocityZ));

    if (std::isnan(velocityX) || std::isnan(velocityY) || std::isnan(velocityZ) || std::isnan(velocity))
        return 1;

    std::cout << velocityX << "," << velocityY << "," << velocityZ << "," << velocity << std::endl;

    return 0;
}
