

4 embedded projects I'm building as part of a bigger system — a distributed embedded pipeline
going from sensor firmware all the way to a live desktop dashboard.

Each project is built standalone first, then they'll be connected together at the end.

> 🚧 Currently working on P1. The rest are planned.

---

| # | Project | Stack | Status |
|---|---|---|---|
| P1 | Smart Control Panel | STM32F411 · libopencm3 · PlatformIO | 🔧 In progress |
| P2 | Vision IoT Node | ESP32-S3 CAM · ESP-IDF · FreeRTOS | 📋 Planned |
| P3 | IoT Backend | Orange Pi Zero 2W · FastAPI · SQLite | 📋 Planned |
| P4 | Viz Dashboard | SDL2 · C++17 · WebSocket | 📋 Planned |

**End goal:** P1 and P2 publish sensor and vision data over MQTT → P3 stores and serves it → P4 visualizes it live.

---

## P1 — Smart Control Panel

Bare-metal STM32F4 firmware with a live display, two sensors, and a state machine
that switches between MANUAL, AUTO, TEST and FAULT modes.

**Hardware:** STM32F411 Black Pill · 1.5" ST7789 TFT · BME280 · MPU-6050 · 2x buttons · ST-Link V2

**Wiring:**
```
SPI1  → ST7789   (PA5 SCK, PA7 MOSI, PA4 CS, PA2 DC, PC14 RST)
I2C1  → BME280   (PB6 SCL, PB7 SDA — address 0x76)
      → MPU-6050 (same bus         — address 0x68)
GPIO  → PB0 btn1, PB1 btn2, PC13 LED
```

**Progress:**
- [x] HAL GPIO
- [x] HAL UART
- [ ] HAL I2C ← here now
- [ ] MPU-6050 driver
- [ ] BME280 driver
- [ ] HAL SPI + ST7789 driver
- [ ] State machine + UI

```bash
pio run -t upload    # flash
pio device monitor   # serial 115200
```

---

## P2 — Vision IoT Node

ESP32-S3 CAM that detects motion using frame-delta and publishes events over MQTT.
FreeRTOS — camera on Core 1, WiFi/MQTT on Core 0, tasks talk through queues only.

---

## P3 — IoT Backend

MQTT broker + Python ingestor + FastAPI REST + WebSocket live feed running on the Orange Pi.
Two separate systemd services sharing only a SQLite database. Survives reboots and crashes.

---

## P4 — Viz Dashboard

SDL2 C++17 desktop app showing live sensor graphs at 60fps.
Network thread feeds a ring buffer. Render thread drains it. They share nothing else.

---

**Tools:** Neovim · PlatformIO · ESP-IDF · CMake · ST-Link · OpenOCD · Debian Linux
