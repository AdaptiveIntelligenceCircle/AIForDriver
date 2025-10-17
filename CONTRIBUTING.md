# Contributing to AIForDriver

**AIForDriver** is an open-source Adaptive Intelligence system designed for real-time perception, decision, and control in driver and autonomous system environments.

---

## 🧭 Project Vision

AIForDriver aims to bridge **human-like adaptability** and **machine-level precision**, forming a behavioral AI layer that can:
- Learn from sensory data in real or simulated driving environments.
- Introspect on its decision process.
- Adaptively recover from anomalies or unsafe behaviors.

---

## 🧑‍💻 How to Contribute

### 1. Fork & Branch
```bash
git clone https://github.com/<yourname>/AIForDriver.git
git checkout -b feature/<your_feature>
```

### 2. Follow the Project Architecture

/core/ — behavior kernel, adaptive control, decision engine

/modules/ — sensors, simulation, protection, learning

/runtime/ — memory, reaction, and safety controller

/language/ — Lua/C++ adaptive scripts for model behavior

/data/ — test inputs, simulated packets, and scenario datasets

### 3. Development Rules

Use C++23, Rust, or Lua 5.4.x.

All modules must remain deterministic under safe mode.

Avoid any unsafe thread-level or driver-level calls outside the sandbox.

### Testing Requirements
Before submitting a Pull Request:

- Run all unit and integration tests.

- Validate using simulation datasets in /tests/scenario/.

- Ensure regression tests pass:
``` bash
./scripts/run_all_tests.sh

```

### Require Test
``` plaintext
| Type                 | Purpose                                            |
| -------------------- | -------------------------------------------------- |
| Unit Test            | Validate module functionality                      |
| Simulation Test      | Check decision output vs expected driving behavior |
| Adaptive Stress Test | Measure model stability under uncertain input      |
| Security Test        | Detect unauthorized data access or unsafe I/O      |

```
### Security Policy 
All contributions must comply with:

+ Transparency — AI decisions must be traceable.

+ Privacy — No data collection without user consent.

+ Control Safety — AI must defer to human override when applicable.

Contributors must not:

+ Embed remote control APIs without disclosure.

+ Include backdoors or driver-level unsafe code.

+ Use AIForDriver for surveillance or harm.
  
### Communication

GitHub Discussions: https://github.com/CodelikeC/AIForDriver/discussions

Email: ndtribk@gmail.com

Issue tracker tags: [bug], [feature], [simulation], [ethics]
