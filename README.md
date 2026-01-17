# 🐍 ESP32 Snake Game (Handheld Console Prototype)

Welcome to the **ESP32 Snake Game** project! This is a fully open-source, Arduino-based implementation of the classic Snake game. 

This project serves as the first prototype for a larger goal: building a complete DIY portable mini-game console from scratch. Currently designed for a breadboard, it uses an **ESP32-S3** and an **ILI9341 TFT Display**.

[![Project Review](https://img.youtube.com/vi/v-BVECaSQdo/0.jpg)](https://youtu.be/v-BVECaSQdo)

## ✨ Features
* **Classic Gameplay:** Retro snake mechanics.
* **High Performance:** Powered by the ESP32-S3.
* **Sound Effects:** Passive buzzer feedback for collecting gems and game over.
* **Progressive Difficulty:** Snake speed increases every 100 points.
* **Easy Build:** No soldering required (Breadboard friendly).
* **Internal Pull-ups:** Buttons use `INPUT_PULLUP` to minimize wiring.

## 🛠️ Hardware Required
* **Microcontroller:** ESP32-S3 Development Board
* **Display:** 2.8" or 2.4" TFT LCD (ILI9341 Driver)
* **Controls:** 4x Tactile Push Buttons
* **Audio:** 1x Passive Buzzer
* **Misc:** Breadboard & Jumper Wires

## 🔌 Wiring & Pinout
This project uses the **HSPI** (or specific hardware SPI) pins for the display.

### 🖥️ Display (ILI9341)
| Display Pin | ESP32 Pin | Note |
| :--- | :--- | :--- |
| **VCC** | 3.3V | Power |
| **GND** | GND | Ground |
| **CS** | GPIO 4 | Chip Select |
| **RESET** | GPIO 9 | Reset |
| **DC** | GPIO 15 | Data/Command |
| **MOSI** | GPIO 11 | SPI Data |
| **SCK** | GPIO 12 | SPI Clock |
| **LED** | 3.3V | Backlight |
| **MISO** | GPIO 13 | *Not strictly used, but connected* |
| **Touch** | N/A | Touch features not used |

### 🎮 Controls (Buttons)
*Buttons connect between the GPIO pin and GND. No external resistors needed.*

| Button | ESP32 Pin |
| :--- | :--- |
| **UP** | GPIO 16 |
| **DOWN** | GPIO 17 |
| **LEFT** | GPIO 18 |
| **RIGHT** | GPIO 8 |

### 🔊 Audio (Buzzer)
| Component | ESP32 Pin |
| :--- | :--- |
| **Positive (+)** | GPIO 14 |
| **Negative (-)** | GND |

---

## 💻 Software Installation

### 1. Arduino IDE Setup
1.  Download and install the **Arduino IDE**.
2.  Go to `File > Preferences`.
3.  Paste the Espressif board manager URL into "Additional Boards Manager URLs".
4.  Go to **Boards Manager**, search for `esp32` by Espressif Systems, and install it.

### 2. Install Libraries
This project relies on the **TFT_eSPI** library for fast graphics.
1.  In Arduino IDE, go to `Sketch > Include Library > Manage Libraries`.
2.  Search for **`TFT_eSPI`** and install it.

### 3. Configure TFT_eSPI (Crucial Step!)
You must configure the library to work with your specific pins.
1.  Navigate to your Arduino libraries folder: `Documents/Arduino/libraries/TFT_eSPI/`.
2.  Open `User_Setup.h` with a text editor (Notepad, VS Code, etc.).
3.  **Uncomment** the line for the driver: `#define ILI9341_DRIVER`.
4.  **Define** your pins in the pin configuration section to match the wiring table above (CS=4, DC=15, RST=9, etc.).

## ⚙️ Customization (Portrait vs. Landscape)
The code is set to **Portrait Mode** by default. To change to Landscape:

1.  **Update Screen Dimensions:**
    Swap the width and height values in the code:
    ```cpp
    // Change 240, 320 to:
    #define screenW 320
    #define screenH 240
    ```
2.  **Update Rotation:**
    In `void setup()`, change the rotation line:
    ```cpp
    tft.setRotation(1); // Use 1 or 3 for Landscape
    ```

## 🤝 Support
If you enjoyed this project or found it useful, please consider supporting the channel!

* 📺 **Subscribe on YouTube:** [DsnIndustries](https://www.youtube.com/@DsnIndustries)
* ⭐ **Star this Repo:** Click the star button at the top right!

---
*Created by DsnIndustries - 2026*

## Games
Maze Escape: https://play.google.com/store/apps/details?id=com.DsnMechanics.MazeEscape

Air Hockey: https://play.google.com/store/apps/details?id=com.DsnMechanics.AirHockey

Click Challenge: https://play.google.com/store/apps/details?id=com.DsNMechanics.ClickChallenge

Flying Triangels: https://play.google.com/store/apps/details?id=com.DsnMechanics.Triangle

SkyScrapper : https://play.google.com/store/apps/details?id=com.DsnMechanics.SkyScraper
