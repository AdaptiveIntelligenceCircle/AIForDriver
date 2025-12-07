#include "sensor_lidar.h"
#include "../safety/safety_log.h"

using namespace std; 
#include <chrono> 

SensorLiDar :: SensorLiDar()
 : active(false),
   max_range_m(120.0),
   beam_error_sd(0.05),
   occlusion_prob(0.02),
   beam_noise(0.0, 0.05),
   uni01(0.0, 1.0)
{
    std::random_device rd;
    re.seed(rd());
}

bool SensorLiDar :: start()
{
    lock_guard<mutex> lk(mtx); 

    active = true;
    SafetyLog::instance().info("SensorLiDAR started");
    return true; 
}

void SensorLiDar :: stop()
{
    lock_guard<std::mutex> lk(mtx);
    active = false;
    SafetyLog::instance().info("SensorLiDAR stopped");
}

void SensorLiDar::set_max_range(double r) {
    std::lock_guard<std::mutex> lk(mtx);
    max_range_m = r;
}

void SensorLiDar::set_beam_error_sd(double sd) {
    std::lock_guard<std::mutex> lk(mtx);
    beam_error_sd = sd;
    beam_noise = std::normal_distribution<double>(0.0, beam_error_sd);
}

void SensorLiDar::set_occlusion_probability(double p) {
    std::lock_guard<std::mutex> lk(mtx);
    occlusion_prob = p;
}

RawSensorData SensorLiDar::read() {
    std::lock_guard<std::mutex> lk(mtx);
    RawSensorData d;
    d.type = "lidar";
    d.timestamp = std::chrono::steady_clock::now();

    if (!active) {
        d.value1 = max_range_m;
        d.value2 = 0.0;
        d.confidence = 0.0;
        return d;
    }

    // Simulate true range to obstacle: for demo choose between 1..50m
    double true_range = 1.0 + 49.0 * uni01(re);

    // occlusion: if occlusion, return max_range or invalid low intensity
    if (uni01(re) < occlusion_prob) {
        d.value1 = max_range_m;
        d.value2 = 0.0;
        d.confidence = 0.15;
        SafetyLog::instance().debug("LiDAR occlusion simulated");
        return d;
    }

    // beam error noise
    double noise = beam_noise(re);
    double measured = true_range + noise;

    // clamp
    if (measured < 0.0) measured = 0.0;
    if (measured > max_range_m) measured = max_range_m;

    d.value1 = measured;
    // intensity inversely proportional to range (rough)
    d.value2 = std::max(0.0, 1.0 - (measured / max_range_m));
    // confidence reduces with range and noise amplitude
    double conf = 0.95 - std::min(0.7, std::abs(noise) / 0.5) - (measured / (2.0 * max_range_m));
    if (conf < 0.05) conf = 0.05;
    d.confidence = conf;

    return d;
}
