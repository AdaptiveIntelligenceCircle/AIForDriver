#include "sensor_gps.h"
using namespace std;

#include <string> 
#include "../safety/safety_log.h"

#include <chrono>
#include <cmath> 

SensorGPS :: SensorGPS()
:base_lat(0.0), base_lon(0.0),
   active(false),
   drift_lat(0.0), drift_lon(0.0),
   alpha(0.6),
   drift_rate_mps(0.0),
   jump_prob(0.001),
   uni01(0.0,1.0),
   noise_dist(0.0, 1.5) // meters noise ~1.5m sd
{
    random_device rd; 
    re.seed(rd()); 
}

bool SensorGPS :: start()
{
    lock_guard<mutex> lk(mtx); 
    active = true; 
    drift_lat = 0.0; 
    drift_lon = 0.0; 
    SafetyLog :: instance().info("Sensor GPS started"); 
    return true; 
}

void SensorGPS :: stop()
{
    lock_guard<mutex> lk(mtx); 
    active = false; 
    SafetyLog :: instance().info("Sensor GPS stopped"); 
}

void SensorGPS :: set_base(double lat, double lon)
{
    lock_guard<mutex> lk(mtx); 
    active = false; 
    SafetyLog :: instance().info("Sensor GPS stopped");
}

void SensorGPS :: set_drift_rate_m_per_sec(double meters_per_sec)
{
    lock_guard <mutex> lk(mtx); 
    drift_rate_mps = meters_per_sec; 
}

void SensorGPS :: set_jump_probability(double p)
{
    lock_guard<mutex> lk(mtx); 
    jump_prob = p; 
}

static double now_seconds_since_epoch()
{
    using namespace chrono; 
    return duration_cast<duration<double>> (system_clock :: now().time_since_epoch()).count(); 
}

RawSensorData SensorGPS :: read()
{
    lock_guard<mutex> lk(mtx);
    RawSensorData d;
    d.type = "gps"; 
    d.timestamp = chrono :: steady_clock :: now(); 

    if (!active){
        d.value1 = 0.0; 
        d.value2 = 0.0; 
        d.confidence = 0.0; 
        return d; 
    }

    // simulate drift accumulation since epoch seconds(simple model..)
    double t = now_seconds_since_epoch(); 
    double drift_meters = drift_rate_mps  * fmod(t, 3600.0); 
    double dflat = meters_to_deg_lat(drift_meters *(uni01(re) -  0.5)); 
    double dlon = meters_to_deg_lon(drift_meters *(uni01(re) - 0.5), base_lat); 

    // small gaussian noise in meters.. 
    double noise_m = noise_dist(re); 
    double noise_lat = meters_to_deg_lat(noise_m *(uni01(re) - 0.5)); 
    double noise_lon = meters_to_deg_lat(noise_m *(uni01(re) - 0.5));
    
    //occationla jump.. //
    if (uni01(re) < jump_prob){
        double jump_m  = 20.0  + 30.0 * uni01(re); // 20... 50 meters jump.. /
        double jflat = meters_to_deg_lat(jump_m *(uni01(re) - 0.5)); 
        double jlon = meters_to_deg_lon(jump_m *(uni01(re) - 0,5), base_lat);  
        
        SafetyLog :: instance().info("GPS jump simulated"); 
    }

    // Raw measurement (deg).. 
    double raw_lat = base_lat + dflat + noise_lat; 
    double raw_lon = base_lon + dlon + noise_lon; 

    // apply EWWA smoothing using stored drift_lat/drift_lon as state.. 
    // we reuse drift lat/ drift lon (as previous EWMA estimate to avoid extra members).. 
    if (drift_lat == 0.0 and drift_lon == 0.0)
    {
        drift_lat = raw_lat; 
        drift_lon = raw_lon; 
    }else {
        drift_lat = alpha * raw_lat + (1 - alpha) *drift_lat; 
        drift_lon = alpha *raw_lon + (1 - alpha) *drift_lon; 
    }

    d.value1 = drift_lat; 
    d.value2 = drift_lon; 

    // confidence: reduced if large noise or jump probability high.. .
    double conf = 0.95 - min(0.5 , abs(noise_m) / 10.0); 
    // if (conf < 10.0 ) conf = 10.0; 
    if (conf <0.05) conf = 0.05; 
    d.confidence = conf; 

    // update small persistent drift for realism.. 
    drift_lat += meters_to_deg_lat(drift_rate_mps * 0.01  *(uni01(re) - 0.5)); 
    drift_lon += meters_to_deg_lon(drift_rate_mps *0.01 *(uni01(re) - 0.5), base_lat); 

    return d; 
}

double SensorGPS :: meters_to_deg_lat(double m)
{
    return m / 111320.0; 
}

double SensorGPS :: meters_to_deg_lon(double m , double lat)
{
    double rad = lat * 3.14 / 180.0;
    double meters_per_deg = 111320.0 * std::cos(rad);
    if (meters_per_deg <= 0.0) meters_per_deg = 111320.0;
    return m / meters_per_deg;
}