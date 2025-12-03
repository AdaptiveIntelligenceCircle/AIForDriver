flowchart TD

    Camera[CameraDriver]
    LiDAR[LiDARDriver]
    GPS[GPSDriver]

    subgraph SensorManager
        direction TB
        Poller[Polling Loop]
        HealthCheck[SensorHealth]
        FusionEngine[SensorFusion]
        Calib[CalibrationManager]
    end

    Camera --> SensorManager
    LiDAR --> SensorManager
    GPS --> SensorManager

    SensorManager --> SafetyModule
    SensorManager --> Analytics
    SensorManager --> Protocol(core)
    SensorManager --> Runtime(bridge/Lua)
