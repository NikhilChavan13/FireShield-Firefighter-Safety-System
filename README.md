🔥 FireShield – Firefighter Safety & Monitoring System
An ESP32-based firefighter safety monitoring prototype developed and tested using Wokwi.

FireShield monitors multiple hazardous conditions and provides immediate alerts when a dangerous situation is detected.

🚨 Features
🌡️ Temperature monitoring using DS18B20
💨 Gas/smoke detection using MQ-2
👁️ Visibility monitoring
🧍 Fall detection using MPU6050
🆘 Manual SOS button
🔊 Emergency buzzer
🔴 Warning LED
📺 OLED live status display
💾 SD-card event logging
🔋 Battery-level monitoring
🛠️ Components
ESP32 DevKit
DS18B20
MQ-2 Gas Sensor
MPU6050
SSD1306 OLED
MicroSD Card
Push Button
Buzzer
LED
Potentiometers for visibility and battery simulation
💻 Technologies
Embedded C/C++, ESP32 Arduino, I²C, SPI, Wokwi.

🔌 Main Pin Configuration
| Function | ESP32 | SOS Button - GPIO 13 Buzzer - GPIO 27 LED - GPIO 2 DS18B20 - GPIO 4 SD CS - GPIO 5 MQ-2 - GPIO 32 Visibility - GPIO 34 Battery - GPIO 35 I²C SDA - GPIO 21 I²C SCL - GPIO 22 SD SCK - GPIO 18 SD MISO - GPIO 19 SD MOSI - GPIO 23

🧠 How It Works
Sensors → ESP32 → Analyze conditions → Emergency detected → Buzzer + LED + OLED + SD logging

💾 Example SD Log
EVENT NUMBER: 1 EVENT: HIGH TEMP TEMPERATURE: 72.4 C GAS VALUE: 3628 VISIBILITY: 820 BATTERY: 76%

📡 Communication Protocols
I²C Used for: MPU6050 SSD1306 OLED

OneWire Used for: DS18B20

SPI Used for: MicroSD card

Analog Input Used for: MQ-2 Visibility simulation Battery simulation

🚦 Safety Thresholds
Parameter Threshold Temperature ≥ 60°C Gas > 3800 Visibility < 1500 Fall acceleration > 15 m/s² Low Battery ≤ 20%

Note: These values are prototype thresholds for simulation and demonstration. They are not certified safety limits for real firefighter operations.

🧪 Simulation
Built and tested using Wokwi.

Wokwi: https://wokwi.com/projects/471623661827837953

🔮 Future Work
GPS location tracking GSM emergency SMS Real hardware implementation Wearable enclosure Remote monitoring

👨‍💻 Author
Nikhil Chavan Electronics & Telecommunication Engineering
