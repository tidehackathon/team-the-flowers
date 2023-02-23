#include "vel_calc.hpp"

double calculateVel(double a)
{
    return a * 0.1;
}

/*
    Ostateczna formuła wygląda następująco:

    prędkość UAV względem ziemi = 
    
    sqrt[(prędkość powietrzna UAV * cos(azymut - kierunek wiatru))^2 + (prędkość wiatru * sin(azymut - kierunek wiatru))^2]

    Gdzie:

    "sqrt" oznacza pierwiastek kwadratowy
    "cos" oznacza funkcję cosinus
    "sin" oznacza funkcję sinus
    "azymut" oznacza kierunek, w którym porusza się UAV (w stopniach)
    "kierunek wiatru" oznacza kierunek, z którego wieje wiatr (w stopniach)
    "prędkość powietrzna UAV" oznacza prędkość, z jaką porusza się UAV względem powietrza (w metrach na sekundę)
    "prędkość wiatru" oznacza prędkość, z jaką wieje wiatr (w metrach na sekundę)
*/

double compensateWindForce(double airVelocity, double w_speed, double w_direction, double bearing)
{
    // double alpha = 1 / sin((w_speed / airVelocity) * sin(w_direction - bearing));
    // return sqrt(pow(2, airVelocity) + pow(2, w_speed) - (2 * airVelocity * w_speed * cos(bearing) - bearing));
    // return std::abs(airVelocity * w_speed * cos(alpha)) / 4.5;

    return sqrt(pow(2, airVelocity * cos(w_direction)) + pow(2, w_speed * sin(w_direction)));
}

/*
    INPUT:
        1           2           3
        gx,         gy,         gz

        4           5           6
        ax,         ay,         az

        7           8           9           10
        q0          q1          q2          q3

        11          12                      13
        w_speed     w_direction             bearing

    OUTPUT:
        acc{0..2}  q{0..3}  vel{0..4}
*/
int main(int argc, char **argv)
{
    /* *** Input parse *** */
    //  Gyroscope data
    double *gyro = new double[3];
    gyro[0] = atof(argv[1]); // X
    gyro[1] = atof(argv[2]); // Y
    gyro[2] = atof(argv[3]); // Z

    //  Accelerometer data
    double *acc = new double[3];
    acc[0] = atof(argv[4]); // X
    acc[1] = atof(argv[5]); // Y
    acc[2] = atof(argv[6]); // Z

    //  Madgwick quaterion
    double *q = new double[4];
    q[0] = atof(argv[7]);
    q[1] = atof(argv[8]);
    q[2] = atof(argv[9]);
    q[3] = atof(argv[10]);

    double *wind = new double[2];
    wind[0] = atof(argv[11]);   // wind speed
    wind[1] = atof(argv[12]);   // wind direction

    double bearing = atof(argv[13]);

    /* *** Madgwick Filter *** */
    //  Input: gyro_data, acc_data
    double *res = new double[2];
    res = filterUpdate(gyro, acc, q);

    double pitch = res[0];
    double roll = res[1];

    /* *** Gravity compensation *** */
    //  Input:  acc_data, madgwickFilter.getQuaternions()
    //  Output: compensatedGravity
    double *accCmp = new double[3];
    compensateGravity(acc, pitch, roll, accCmp);
    if (std::isnan(accCmp[0]) || std::isnan(accCmp[1]) || std::isnan(accCmp[2]))
        return 1;

    // Velocity calculation:
    double *vel = new double[4];
    //   component velocities
    vel[0] = calculateVel(accCmp[0]);  // velocity by axis X
    vel[1] = calculateVel(accCmp[1]);  // velocity by axis Y
    vel[2] = calculateVel(accCmp[2]);  // velocity by axis Z
    vel[3] = sqrt(pow(2, vel[0]) + pow(2, vel[1]) + pow(2, vel[2])); // integrated velocity
    vel[3] *= 10; // integrated velocity : normalize

    /* WIND INFLUENCE */
    double velocity = compensateWindForce(vel[3], wind[0], wind[1], bearing);
    // velocity /= 20;

    if (std::isnan(vel[3]) || std::isnan(velocity))
        return 1;

    // Results to STDOUT
    std::cout << accCmp[0] << "," << accCmp[1] << "," << accCmp[2] << ",";
    std::cout << q[0] << "," << q[1] << "," << q[2] << "," << q[3] << ",";
    std::cout << vel[0] << "," << vel[1] << "," << vel[2] << "," << vel[3] << "," << velocity << std::endl;

    return 0;
}
