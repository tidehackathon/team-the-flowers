#include "vel_calc.hpp"

double calculateVel(double prev_vel, double a)
{
    return a * 0.1;
}

/*
    ARGS:
        1           2           3
        prev_gx,    prev_gy,    prev_gz

        4           5           6
        prev_ax,    prev_ay,    prev_az

        7           8           9
        ax,         ay,         az

        10
        prev_state

        11          12          13          14
        q0          q1          q2          q3

        15          16          17
        prev_vx     prev_vy     prev_vz
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
    double q0 = atof(argv[11]);
    double q1 = atof(argv[12]);
    double q2 = atof(argv[13]);
    double q3 = atof(argv[14]);
    double prev_vx = atof(argv[15]);
    double prev_vy = atof(argv[16]);
    double prev_vz = atof(argv[17]);

    // Madgwick Filter
    //  Input: gyro_data, acc_data
    double *res = new double[6];
    res = filterUpdate(prev_gx, prev_gy, prev_gz, prev_ax, prev_ay, prev_az, q0, q1, q2, q3);

    float *q = new float[4];
    q[0] = res[0];
    q[1] = res[1];
    q[2] = res[2];
    q[3] = res[3];

    // Gravity Compensation for previous acceleration
    //  Input: prev_acc_data, madgwickFilter.getQuaternions()
    //  Output: prev_compensatedGravity
    double *prev_acc = new double[3];
    prev_acc[0] = prev_ax;
    prev_acc[1] = prev_ay;
    prev_acc[2] = prev_az;

    double *prev_compensatedGravity = new double[3];
    compensateGravity(prev_acc, res[5], res[6], prev_compensatedGravity);

    // Gravity Compensation
    //  Input:  acc_data, madgwickFilter.getQuaternions()
    //  Output: compensatedGravity
    double *acc = new double[3];
    acc[0] = ax;
    acc[1] = ay;
    acc[2] = az;

    double *compensatedGravity = new double[3];
    compensateGravity(prev_acc, res[5], res[6], compensatedGravity);
    if (std::isnan(compensatedGravity[0]) || std::isnan(compensatedGravity[1]) || std::isnan(compensatedGravity[2])){
        std::cerr << "cmpGrav NaN" << std::endl;
        std::cerr
          << "-------------------------------------------------" << std::endl
          << std::endl;
        
        return 2;
    }

    std::cout << compensatedGravity[0] << "," << compensatedGravity[1] << "," << compensatedGravity[2] << ",";

    // Velocity calculation of axis X, Y and Z
    //  Calculation integrated velocity
    double velocityX, velocityY, velocityZ, velocity;
    velocityX = calculateVel(prev_vx, compensatedGravity[0]);
    velocityY = calculateVel(prev_vy, compensatedGravity[1]);
    velocityZ = calculateVel(prev_vz, compensatedGravity[2]);
    velocity = sqrt(pow(2, velocityX) + pow(2, velocityY) + pow(2, velocityZ));
    velocity *= 10;

    // Montion Detection
    //  Input:  compensatedGravity, gyro_data
    //  Output: montion detected or not : bool
    MontionDetection *montionDetector = new MontionDetection();

    double *prev_gyro = new double[3];
    prev_gyro[0] = prev_gx;
    prev_gyro[1] = prev_gy;
    prev_gyro[2] = prev_gz;

    std::cout << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << ",";
    std::cout << velocityX << "," << velocityY << "," << velocityZ << "," << velocity << "," << prev_state << std::endl;
    
    return 0;
}
