#include <iostream>
#include <stdint.h>
#include <cstdint>
#include <math.h>
#include <vector>

#include "vel_calc.hpp"

double integrate(double x1, double x2, double y1, double y2)
{
    double value;
    value = (y1 + y2) * (x2 - x1) * 0.5;
    return value;
}

double linearFunc(int *x, double *y, double value)
{
    double a = (y[1] - y[0]) / (x[1] - x[0]);
    double b = y[0] - a * x[0];

    return (a * value + b);
}

/*
    INPUT:
        1           2           3
        prev_gx,    prev_gy,    prev_gz
        4           5           6
        prev_ax,    prev_ay,    prev_az
        7           8           9
        ax,         ay,         az
        10
        prev_state
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
    int prev_state = atoi(argv[10]);

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
    if (std::isnan(compensatedGravity[0]) || std::isnan(compensatedGravity[1]) || std::isnan(compensatedGravity[2])){
        return 2;
    }

    std::cout << compensatedGravity[0] << "," << compensatedGravity[1] << "," << compensatedGravity[2] << ",";

    double velocityX, velocityY, velocityZ, velocity;

    

    // Montion Detection
    //  Input:  compensatedGravity, gyro_data
    //  Output: montion detected or not : bool
    MontionDetection *montionDetector = new MontionDetection();

    double *prev_gyro = new double[3];
    prev_gyro[0] = prev_gx;
    prev_gyro[1] = prev_gy;
    prev_gyro[2] = prev_gz;

    bool d = montionDetector->zeroVelocityUpdate(prev_compensatedGravity, prev_gyro);
    if (d)
    {
        std::cerr << "1" << std::endl;

        velocityX = integrate(0, 0.1, prev_compensatedGravity[0], compensatedGravity[0]);
        velocityY = integrate(0, 0.1, prev_compensatedGravity[1], compensatedGravity[1]);
        velocityZ = integrate(0, 0.1, prev_compensatedGravity[2], compensatedGravity[2]);
        
        if(prev_state == 0)
        {
            std::cerr << "-------------------------------------------------" << std::endl
                      << std::endl;

            velocity = sqrt(pow(2, velocityX) + pow(2, velocityY) + pow(2, velocityZ));

            if (std::isnan(velocityX) || std::isnan(velocityY) || std::isnan(velocityZ) || std::isnan(velocity))
                return 2;

            prev_state = 1;

            std::cout << velocityX << "," << velocityY << "," << velocityZ << "," << velocity << "," << prev_state << std::endl;

            return 0;
        }
        prev_state = 1;
    }
    else
    {
        std::cerr << "0" << std::endl;
        
        velocityX = integrate(0, 0.1, prev_compensatedGravity[0], compensatedGravity[0]);
        velocityY = integrate(0, 0.1, prev_compensatedGravity[1], compensatedGravity[1]);
        velocityZ = integrate(0, 0.1, prev_compensatedGravity[2], compensatedGravity[2]);
        
        if(prev_state == 1)
        {
            std::cerr << "-------------------------------------------------" << std::endl
                      << std::endl;

            velocity = sqrt(pow(2, velocityX) + pow(2, velocityY) + pow(2, velocityZ));

            if (std::isnan(velocityX) || std::isnan(velocityY) || std::isnan(velocityZ) || std::isnan(velocity))
                return 2;

            prev_state = 0;
            
            std::cout << velocityX << "," << velocityY << "," << velocityZ << "," << velocity << "," << prev_state << std::endl;

            return 0;
        }
        prev_state = 0;
    }

    std::cerr << "-------------------------------------------------" << std::endl
              << std::endl;

    std::cout << prev_state << "," << d << std::endl;

    return 1;
}
