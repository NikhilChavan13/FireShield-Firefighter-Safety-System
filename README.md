🔥 FireShield – Firefighter Safety & Monitoring System
An ESP32-based firefighter safety monitoring prototype developed and tested using Wokwi.

FireShield monitors multiple hazardous conditions and provides immediate alerts when a dangerous situation is detected.

🚨 Features:
🌡️ 1. Temperature monitoring using DS18B20
💨 2. Gas/smoke detection using MQ-2
👁️ 3. Visibility monitoring
🧍 4. Fall detection using MPU6050
🆘 5. Manual SOS button
🔊 6. Emergency buzzer
🔴 7. Warning LED
📺 8. OLED live status display
💾 9. SD-card event logging
🔋 10. Battery-level monitoring

🛠️ Components:
1. ESP32 DevKit
2. DS18B20
3. MQ-2 Gas Sensor
4. MPU6050
5. SSD1306 OLED
6. MicroSD Card
7. Push Button
8. Buzzer
9. LED
10. Potentiometers for visibility and battery simulation

💻 Technologies:
Embedded C/C++, ESP32 Arduino, I²C, SPI, Wokwi.

🔌 Main Pin Configuration:
| Function | ESP32 | 
1. SOS Button - GPIO 13
2. Buzzer - GPIO 27
3. LED - GPIO 2
4. DS18B20 - GPIO 4
5. SD CS - GPIO 5
6. MQ-2 - GPIO 32
7. Visibility - GPIO 34
8. Battery - GPIO 35
9. I²C SDA - GPIO 21
10. I²C SCL - GPIO 22
11. SD SCK - GPIO 18
12. SD MISO - GPIO 19
13. SD MOSI - GPIO 23

🧠 How It Works:
Sensors → ESP32 → Analyze conditions → Emergency detected → Buzzer + LED + OLED + SD logging

💾 Example SD Log:

EVENT NUMBER:
1 EVENT:HIGH TEMP TEMPERATURE: 72.4 C 
2 GAS VALUE: 3628 
3 VISIBILITY: 820
4 BATTERY: 76%

📡 Communication Protocols:
1. I²C Used for: MPU6050 SSD1306 OLED

2. OneWire Used for: DS18B20

3. SPI Used for: MicroSD card

4. Analog Input Used for: MQ-2 Visibility simulation Battery simulation

🚦 Safety Thresholds:
Parameter:
1. Threshold Temperature ≥ 60°C
2. Gas > 3800
3. Visibility < 1500
4. Fall acceleration > 15 m/s²
5. Low Battery ≤ 20%

Note: These values are prototype thresholds for simulation and demonstration. They are not certified safety limits for real firefighter operations.

🧪 Simulation:

Built and tested using Wokwi.

Wokwi: https://wokwi.com/projects/471623661827837953

🔮 Future Work:

GPS location tracking GSM emergency SMS Real hardware implementation Wearable enclosure Remote monitoring

👨‍💻 Author:

Nikhil Chavan:
Electronics & Telecommunication Engineering
