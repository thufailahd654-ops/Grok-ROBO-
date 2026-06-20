# Grok-ROBO-
Sound-Reactive Robot Eyes with DFPlayer Mini (ESP32)
# Sound-Reactive Robot Eyes with DFPlayer Mini (ESP32)

A sound-triggered project: a sound sensor detects noise → an SSD1306 OLED wakes up with animated robot eyes → a DFPlayer Mini plays an MP3.

## Features

- **Sound-triggered wake-up** — eyes are off until sound is detected
- **Animated eyes** — white, rounded-square, symmetrical eyes centered on screen
- **Wake-up animation** — eyes grow from a thin line to full size
- **Look-sideways animation** — eyes glance left, center, right, center
- **Blink animation** — eyes shrink to a line and snap back to full size
- **Idle blinking** — once awake, eyes blink periodically to look alive
- **Audio playback** — DFPlayer Mini plays `0001.mp3` from the SD card on trigger

## Hardware Required

| Component | Notes |
|---|---|
| ESP32 Dev Board | Any standard ESP32 |
| Sound Sensor Module | KY-038 style, VCC / GND / DO / AO |
| DFPlayer Mini | MP3 module with SD card slot |
| 0.96" OLED Display | SSD1306, I2C |
| Speaker | 3W 8Ω (for DFPlayer) |
| MicroSD Card | FAT32 formatted |
| 1kΩ Resistor | Protects DFPlayer RX line |

## Wiring

### Sound Sensor → ESP32

| Sensor Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| DO | GPIO 4 |
| AO | Not used |

### OLED (SSD1306, I2C) → ESP32

| OLED Pin | ESP32 Pin |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

### DFPlayer Mini → ESP32

| DFPlayer Pin | ESP32 Pin |
|---|---|
| VCC | 5V (VIN) |
| GND | GND |
| RX | GPIO 17 (via 1kΩ resistor) |
| TX | GPIO 16 |
| SPK_1 / SPK_2 | Speaker |

> All GND pins (sensor, OLED, DFPlayer, ESP32) must share a common ground.

## SD Card Setup

- Format the microSD card as **FAT32**
- Place your audio file in the **root directory** as `0001.mp3`
- DFPlayer requires the 4-digit naming convention; `myDFPlayer.play(1)` plays `0001.mp3`

## Libraries Required

Install these via the Arduino Library Manager:

- `Adafruit SSD1306`
- `Adafruit GFX Library`
- `DFRobotDFPlayerMini`

## How It Works

1. On boot, the OLED screen stays **off/blank**.
2. The sound sensor's digital output (DO) is monitored. The module used here triggers **LOW** on sound detection (some modules trigger HIGH — flip the condition in code if needed).
3. On first sound trigger:
   - DFPlayer plays `0001.mp3`
   - Eyes **wake up** (grow from a line to full size, centered)
   - Eyes **glance left → center → right → center**
   - Eyes **blink** once
4. After waking, the eyes **stay on indefinitely** and blink automatically every few seconds to look alive.
