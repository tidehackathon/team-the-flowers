#ifndef MONDET_HFILE
#define MONDET_HFILE montion_detection

#include_next <stdint.h>
#include_next <cstdint>
#include_next <math.h>

#define STATIC_SAMPLES_THRESHOLD 10
#define STATIC_ACCELERATION_THRESHOLD 0.35
#define STATIC_ANGULAR_VELOCITY_THRESHOLD 0.15

class MontionDetection
{
private:
    int samplesCount;
    
public:
    MontionDetection();
    bool zeroVelocityUpdate(double *acceleration, double *angularVelocity);
};

#endif