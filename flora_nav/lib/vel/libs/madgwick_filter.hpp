#ifndef MADGWICK_HFILE
#define MADGWICK_HFILE madgwick_filter

#include_next <stdint.h>
#include_next <cstdint>
#include_next <math.h>
#include_next <vector>

static inline uint64_t doubleToRawBits(double x);
static inline double rawBitsToDouble(uint64_t bits);

class MagdwickFilter
{
private:
    double sampleFrequency;
    double betaDef;
    double beta;
    double q0, q1, q2, q3;
    double R11, R21, R31, R32, R33;
    int count;
    double pi;

public:
    MagdwickFilter(double q0, double q1, double q2, double q3);

    /*
     * This method is performing single iteration of filter:
     *   ax - accelerometer measurement by X
     *   ay - accelerometer measurement by Y
     *   az - accelerometer measurement by Z
     *   gx - gyroscope measurement by X
     *   gy - gyroscope measurement by Y
     *   gz - gyroscope measurement by Z
     */
    void filterUpdate(double ax, double ay, double az, double gx, double gy, double gz);

    double invSqrt(double x);

    /* *** Getters *** */
    double getRoll();
    double getPitch();
    double getYaw();
    std::vector<double> getQuaternions();
};

#endif