#ifndef GRAV_HFILE
#define GRAV_HFILE gravity_compensation

#include_next <stdint.h>
#include_next <cstdint>
#include_next <math.h>
#include_next <vector>

#define GRAVITY 9.91

double *compensateGravity(double *acc, double *q);

#endif