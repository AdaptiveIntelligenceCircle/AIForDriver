#pragma once 

#include <iostream>
using namespace std; 

namespace SensorUtils
{
    double smooth (double prev, double current, double alpha =0.5); 
}