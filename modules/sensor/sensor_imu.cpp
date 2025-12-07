#include "sensor_imu.h"
#include "../safety/safety_log.h"

using namespace std; 
#include <iostream>

SensorIMU :: SensorIMU()
: active(false), 
accel_bias(0.0), gyro_bias(0.0), 
accel_bias_drift_per_sec(1e-4),
gyro_bias_drift_per_sec(1e-3),
accel_noise_sd(0.05),
gyro_noise_sd(0.1),
accel_noise(0.0, 0.05),
gyro_noise(0.0, 0.1),
estimated_velocity(0.0)
{
    random_device rd; 
    re.seed(rd()); 
}

bool SensorIMU :: start()
{
    lock_guard<mutex> lk(mtx); 
    active = true; 
    last_ts = chrono :: steady_clock :: now(); 
    SafetyLog :: instance().info("StateIMU started");
    return true;  
}

void SensorIMU :: stop()
{
    lock_guard<mutex> lk(mtx); 
    active = false; 
    // last_ts = chrono :: steady_clock :: now(); 
    SafetyLog :: instance().info("StateIMU stopped"); 
}

RawSensorData SensorIMU ::read() 
{
    lock_guard<mutex> lk(mtx); 
    RawSensorData d; 
    d.type = "imu"; 
    d.timestamp = chrono :: steady_clock :: now(); 

    if (!active)
    {
        d.value1 = 0.0; 
        d.value2 = 0.0; 
        d.confidence = 0.0; 
        return d; 
    }
    // time delta for intergration and bias drift... 
    auto now = chrono :: steady_clock :: now(); 
    double dt = chrono :: duration<double>(now - last_ts).count(); 
    if (dt <= 0) dt = 0.001; 
    last_ts = now; 

    // simulate true motion : for demo small oscillation.. 
    double true_accel = 0.1 * sin(chrono :: duration<double>(now.time_since_epoch()).count()); 
    double true_gyro = 1.0 * sin(chrono :: duration<double>(now.time_since_epoch()).count()); 

    // bias drifts slowly.. 
    accel_bias += accel_bias_drift_per_sec *dt *((double)rand() / RAND_MAX  - 0.5);
    gyro_bias += gyro_bias_drift_per_sec * dt * ((double)rand()/ RAND_MAX - 0.5 ); 

    double noisy_accel = true_accel  + accel_bias + accel_noise(re); 
    double noisy_gyro = true_gyro  + gyro_bias + gyro_noise(re); 

    // integrate accel -> estimated velocity.. 
    estimated_velocity += noisy_accel *dt; 

    d.value1 = noisy_accel; 
    d.value1 = noisy_gyro; 

    // confidence degrade with bias magnitude and noise.. //
    double conf = 0.9 - min(0.0, abs(accel_bias) + abs(gyro_bias)); 

    if (conf < 0.05) conf = 0.05; 
    d.confidence = conf; 
    return d; 
}

void SensorIMU :: set_accel_noise_sd(double s)
{
    lock_guard<mutex> lk(mtx); 
    accel_noise_sd = s; 
    accel_noise = normal_distribution<double> (0.0, accel_noise_sd); 
}

void SensorIMU :: set_gyro_noise_sd(double s)
{
    lock_guard<mutex> lk(mtx); 
    gyro_noise_sd = s; 
    gyro_noise = normal_distribution<double>(0.0, gyro_noise_sd); 
}

void SensorIMU :: set_bias_drift(double accel_bdps, double gyro_bdps)
{
    lock_guard<mutex> lk(mtx); 
    accel_bias_drift_per_sec = accel_bdps ; 
    gyro_bias_drift_per_sec = gyro_bdps; 
}




