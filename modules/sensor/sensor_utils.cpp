#include "sensor_utils.h"

double SensorUtils :: smooth(double prev, double current, double alpha)
{
    return alpha *current + (1 - alpha) *prev; 
}