# Smart-Embedded-Systems-Panel

# STM32F4 Smart Control Panel

A bare-metal embedded system built on the STM32F411 Black Pill using libopencm3 and PlatformIO.
No HAL library, no Arduino — just direct register access and clean C.

> 🚧 **Work in progress** — currently working on I2C + MPU-6050 integration.

---

## What this is

A control panel system with a live display, two sensors, and a state machine that switches between modes.
The kind of thing you'd find as the brain of a small industrial controller or robotics node.

It reads temperature and pressure from a BME280, raw accelerometer and gyroscope data from an MPU-6050,
and shows everything on a 1.5" ST7789 TFT display. Two buttons let you switch between MANUAL, AUTO, and TEST modes.

---

## Hardware

| Component | Role |
|---|---|
| STM32F411 Black Pill | Main MCU |
| 1.5" ST7789 TFT (240×240) | Display |
| BME280 | Temperature, pressure, humidity |
| MPU-6050 | 3-axis accelerometer + 3-axis gyroscope |
| 2× tactile buttons | Mode switch + confirm |
| ST-Link V2 | Flashing + SWD debug |

### Wiring summary

**SPI1 — ST7789 display**
```
PA5 → SCL    PA7 → SDA    PA4 → CS
PA2 → DC     PC14 → RES   3.3V → BLK
```

**I2C1 — BME280 + MPU-6050 (shared bus)**
```
PB6 → SCL (both sensors)
PB7 → SDA (both sensors)
4.7k pull-ups on both lines to 3.3V

BME280  SDO → GND  → address 0x76
MPU6050 AD0 → GND  → address 0x68
```

**GPIO**
```
PB0 → Button 1 (mode)     PB1 → Button 2 (confirm)
PC13 → onboard LED (status)
```

---

## Firmware structure

```
src/
├── main.c                  # clock init, super-loop
├── app/
│   ├── state_machine.c/h   # FSM: INIT → MANUAL ↔ AUTO → TEST → FAULT
│   ├── ui_controller.c/h   # what to draw on the display per state
│   └── sensor_manager.c/h  # read + filter sensor data
├── drivers/
│   ├── st7789.c/h          # SPI display driver
│   ├── bme280.c/h          # BME280 I2C driver
│   └── mpu6050.c/h         # MPU-6050 I2C driver
└── hal/
    ├── hal_gpio.c/h        # ✅ done
    ├── hal_uart.c/h        # ✅ done
    ├── hal_i2c.c/h         # 🔧 in progress
    ├── hal_spi.c/h         # pending
    └── hal_timer.c/h       # pending
```

---

## State machine

```
INIT → self-test all peripherals
     ↓ pass              ↓ fail
  MANUAL               FAULT
  (live sensor display)
     ↕ btn1
   AUTO
  (filtered view)
     ↕ btn2 hold (2s) from any state
   TEST
  (PASS/FAIL per peripheral)
```

- **MANUAL** — shows live BME280 temp/pressure + MPU-6050 accel/gyro values on screen
- **AUTO** — same data, different display layout, larger font
- **TEST** — runs WHO_AM_I checks on both sensors + SPI ping on LCD, shows PASS/FAIL
- **FAULT** — something failed, all outputs off, shows fault code, needs hardware reset

---

## Build & flash

```bash
# build
pio run

# flash via ST-Link
pio run -t upload

# serial monitor (115200 baud)
pio device monitor
```

---

## Progress

- [x] PlatformIO project setup + platformio.ini
- [x] Clock config (84 MHz HSI)
- [x] HAL GPIO
- [x] HAL UART + printf over serial
- [ ] HAL I2C  ← working on this now
- [ ] MPU-6050 driver (WHO_AM_I, wake from sleep, burst read)
- [ ] BME280 driver (calibration + compensation formula)
- [ ] HAL SPI
- [ ] ST7789 display driver
- [ ] Button debounce
- [ ] State machine
- [ ] UI controller
- [ ] Self-test routine

---

## Part of a 4-project embedded portfolio

This is P1 of a standalone 4-project embedded systems portfolio:

| # | Project | Stack | Status |
|---|---|---|---|
| P1 | Smart Control Panel | STM32F4 + libopencm3 | 🔧 In progress |
| P2 | Vision IoT Node | ESP32-S3 + ESP-IDF + FreeRTOS | Pending |
| P3 | IoT Backend | Orange Pi Zero 2W + FastAPI + MQTT | Pending |
| P4 | Viz Dashboard | SDL2 + C++17 | Pending |

Each project is fully standalone — no cross-project dependencies.
