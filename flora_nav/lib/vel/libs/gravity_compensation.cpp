#include "gravity_compensation.hpp"

double *compensateGravity(double *acc, std::vector<double> q)
{
    double *g = new double[3];
    g[0] = 2 * (q[1] * q[3] - q[0] * q[2]) * GRAVITY;
    g[1] = 2 * (q[0] * q[1] + q[2] * q[3]) * GRAVITY;
    g[2] = (q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]) * GRAVITY;

    double *compensatedGravity = new double[3];
    compensatedGravity[0] = acc[0] - g[0];
    compensatedGravity[1] = acc[1] - g[1];
    compensatedGravity[2] = acc[2] - g[2];

    return compensatedGravity;
}
