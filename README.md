# 🚗 AIForDriver

**AIForDriver** is a driver assistance system designed to **reduce stress**, **improve driving performance**, and **enhance safety**.

<p align="center">
  <img src="assets/Aifordriver(new).jpg" alt="AIC Logo" width="1024" length = "1024"/>
</p>

<p align="center">
  <strong>AIForDriver for AIC</strong><br>
  Decentralized nodes • Ethical scaling • auto-driven
</p>

> AIC/HMN is licensed under GPL-3.0. Any fork or derivative work must keep the same license and respect the core principles: zero-donation, third path independence, and ethical-from-kernel. The project is currently maintained by the founder. During the founder’s mandatory military service (expected 2027–2029), the project will enter maintenance mode. All code remains public and transparent.”

---

## 🧠 Objectives

- Analyze the driver's mental and physical state through sensors and behavioral data.

- Provide recommendations or warnings to improve concentration and reduce stress.

- Integrate AI technologies to support real-time decision-making.

## 📦 Key Features

- Recognizes driver emotions and fatigue levels.

- Suggests appropriate music, rest breaks, or route adjustments.

- Warns of distraction or drowsiness.

- Intuitive and easy-to-use interface.

## Structure 
``` plaintext
AIForDriver/
│
├── core/                  # Trái tim hệ thống, xử lý thời gian thực
│   ├── kernel/            # Bộ lõi (reactor loop, memory, adaptive runtime)
│   ├── protocol/          # Giao thức giữa cảm biến, AI và trình điều khiển
│   ├── behavior/          # Logic hành vi điều khiển và phản ứng
│   └── security/          # Bảo vệ, phát hiện bất thường, tự vệ
│
├── modules/               # Các module mở rộng có thể thay thế hoặc plug-in
│   ├── sensor/            # Camera, Lidar, Radar, IMU, GPS,...
│   ├── safety/            # Hệ thống phanh, tránh va chạm, fallback
│   ├── control/           # Adaptive Control, PID/LQR/Fuzzy
│   └── analytics/         # Phân tích dữ liệu, thống kê, logs, học tăng cường
│
├── protocol/              # Giao thức truyền thông & điều phối
│   ├── core/              # C++ lớp điều phối chính
│   │   ├── control_channel.cpp
│   │   ├── driver_state.cpp
│   │   ├── emergency_protocol.cpp
│   │   └── sensor_sync.cpp
│   └── lua/               # Layer trí tuệ thích nghi và an toàn
│       ├── adaptive_driver.lua
│       └── safety_trigger.lua
│
├── runtime/               # Cơ chế chạy và phối hợp runtime (giống Adaptive OS)
│   ├── executor.cpp       # Bộ thực thi nhiệm vụ (các lệnh điều khiển)
│   ├── scheduler.cpp      # Lập lịch phản ứng theo chu kỳ cảm biến
│   ├── bridge_lua.cpp     # Giao tiếp với Lua layer
│   └── event_loop.cpp     # Reactor/Observer để quản lý sự kiện
│
├── utils/                 # Tiện ích nền
│   ├── serializer.cpp
│   ├── checksum.cpp
│   ├── packet_parser.cpp
│   ├── compress.cpp
│   └── logger.cpp
│
├── config/                # Tệp cấu hình hệ thống
│   ├── protocol_spec.json
│   ├── packet_structure.json
│   ├── adaptive_metadata.json
│   ├── version_manifest.yaml
│   └── error_code.json
│
├── docs/                  # Tài liệu nội bộ
│   ├── architecture.md
│   ├── design_notes.md
│   └── safety_standards.md
│
└── tests/                 # Unit test và simulation
    ├── test_protocol.cpp
    ├── test_lua_bridge.cpp
    └── test_emergency.lua

```

## Detailed operating flow :
``` plaintext
Sensors → SensorSync → DriverState → Lua:SafetyTrigger
                     ↓
              Lua:AdaptiveDriver
                     ↓
           ControlChannel → Vehicle Actuators
                     ↓
           EmergencyProtocol (nếu cần)
```
Combining **hardware response (C++)** and **software learning (Lua)**:

- C++: ensures immediate response, hardware control, real-time.

- Lua: adjusts strategies, adapts, self-learns, handles complex exceptions.

## Logic layer.  
```
┌──────────────────────────┐
│     Application Layer    │ → Dashboard, User Feedback, HMI
├──────────────────────────┤
│     Adaptive Protocol    │ → Lua + Core for driver protocol. 
├──────────────────────────┤
│     Behavior Engine      │ → compiler, learning, behavior
├──────────────────────────┤
│     Sensor Integration   │ → sensor, synchronize raw data
├──────────────────────────┤
│     Runtime Layer        │ → Scheduler, Executor, Lua Bridge
├──────────────────────────┤
│     Security Layer       │ → Self-defense, Intrusion detection
├──────────────────────────┤
│     Kernel Layer         │ → Memory, State Control, Introspection
└──────────────────────────┘

```
## Vision :
``` 
| Phase            | Mission                                      | Main Components                   |
| -------------------- | -------------------------------------------- | ------------------------------------------------- |
| **11-12/2025** | Complete `protocol/core` + `lua` + bridge  | `control_channel`, `lua_bridge`, safety unit test |
| **Q1/2026**          | Add `runtime/` and `security/` sandbox       | `executor`, `scheduler`, `intrusion_detector`     |
| **Q2/2026**          | Expand `behavior/` and `analytics/`          | Reinforcement Learning, Behavior Log              |
| **Q3/2026**          | Distributed connection (AIForDriver Cloud Network) | `p2p_protocol`, `trust_manager`, `update_sync`    |
```

## 🛠️ Settings

```bash
git clone https://github.com/AdaptiveIntelligenceCircle/AIForDriver.git
cd AIForDriver
```

## 🚀 Testing

```bash
gcc -o helpdriver.cpp helpdriver
```

The system will begin collecting data and providing responses appropriate to the driver's state.

## Projects 
+ Adaptive OS : https://github.com/AdaptiveIntelligenceCircle/AdaptiveOS
+ DIP : https://github.com/AdaptiveIntelligenceCircle/DriverIntrospectiveProxy

## 📄 LICENSE

Dự án được phát hành theo giấy phép [GNU GPL-3.0](https://github.com/AdaptiveIntelligenceCircle/AIForDriver/blob/main/LICENSE).

## 🤝 Contributing

We welcome all contributions! Please create a pull request or open an issue for discussion.

---

The project is in its launch phase.
