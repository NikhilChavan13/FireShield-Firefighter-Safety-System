#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// =====================================================
// OLED
// =====================================================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// =====================================================
// PIN CONNECTIONS
// =====================================================

#define SOS_BUTTON        13
#define BUZZER            27
#define WARNING_LED       2

#define ONE_WIRE_BUS      4

#define VISIBILITY_SENSOR 34

#define MQ2_SENSOR        32

#define BATTERY_SENSOR    35

#define SD_CS             5

// =====================================================
// SAFETY LIMITS
// =====================================================

#define TEMP_LIMIT        60.0
#define FALL_LIMIT        15.0
#define GAS_LIMIT         3800
#define VISIBILITY_LIMIT  1500
#define LOW_BATTERY       20

// =====================================================
// OBJECTS
// =====================================================

Adafruit_MPU6050 mpu;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature tempSensor(&oneWire);

// =====================================================
// VARIABLES
// =====================================================

String previousStatus = "SAFE";

bool sdReady = false;

unsigned long eventNumber = 0;

// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(115200);

  delay(500);

  // ---------------------------------------------------
  // PINS
  // ---------------------------------------------------

  pinMode(SOS_BUTTON, INPUT_PULLUP);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

  pinMode(WARNING_LED, OUTPUT);
  digitalWrite(WARNING_LED, LOW);

  // ---------------------------------------------------
  // I2C
  // ---------------------------------------------------

  Wire.begin(21, 22);

  // ---------------------------------------------------
  // TEMPERATURE SENSOR
  // ---------------------------------------------------

  tempSensor.begin();

  // ---------------------------------------------------
  // MPU6050
  // ---------------------------------------------------

  if (!mpu.begin()) {

    Serial.println("MPU6050 NOT FOUND!");

    while (1) {
      digitalWrite(WARNING_LED, HIGH);
      delay(200);
      digitalWrite(WARNING_LED, LOW);
      delay(200);
    }
  }

  Serial.println("MPU6050 READY");

  // ---------------------------------------------------
  // OLED
  // ---------------------------------------------------

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        0x3C
      )) {

    Serial.println("OLED NOT FOUND!");

    while (1);
  }

  Serial.println("OLED READY");

  // ---------------------------------------------------
  // MICRO SD
  // ---------------------------------------------------

  Serial.println("Starting microSD...");

  if (!SD.begin(SD_CS)) {

    Serial.println("SD CARD NOT FOUND!");

    sdReady = false;

  } else {

    Serial.println("SD CARD READY!");

    sdReady = true;

    // Create / open log file
    File file = SD.open(
      "/firelog.txt",
      FILE_WRITE
    );

    if (file) {

      file.println();
      file.println("==============================");
      file.println("FIRESHIELD SYSTEM STARTED");
      file.println("==============================");

      file.close();

      Serial.println("LOG FILE READY!");

    } else {

      Serial.println("ERROR CREATING LOG FILE!");
    }
  }

  // ---------------------------------------------------
  // STARTUP OLED
  // ---------------------------------------------------

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println("FIRESHIELD");
  display.println("----------------");
  display.println("SYSTEM READY");

  if (sdReady)
    display.println("SD: READY");
  else
    display.println("SD: ERROR");

  display.display();

  delay(2000);
}

// =====================================================
// SAVE EVENT TO SD CARD
// =====================================================

void saveEvent(
  String eventName,
  float temperature,
  int gasValue,
  int visibilityValue,
  int batteryPercent
) {

  if (!sdReady) {

    Serial.println("SD NOT READY - EVENT NOT SAVED");

    return;
  }

  File file = SD.open(
    "/firelog.txt",
    FILE_WRITE
  );

  if (file) {

    eventNumber++;

    file.println();
    file.println("------------------------------");

    file.print("EVENT NUMBER: ");
    file.println(eventNumber);

    file.print("EVENT: ");
    file.println(eventName);

    file.print("TEMPERATURE: ");
    file.print(temperature, 1);
    file.println(" C");

    file.print("GAS VALUE: ");
    file.println(gasValue);

    file.print("VISIBILITY: ");
    file.println(visibilityValue);

    file.print("BATTERY: ");
    file.print(batteryPercent);
    file.println("%");

    file.println("------------------------------");

    file.close();

    Serial.println(">>> EVENT SAVED TO SD <<<");

  } else {

    Serial.println(">>> SD WRITE ERROR <<<");
  }
}

// =====================================================
// LOOP
// =====================================================

void loop() {

  bool emergency = false;

  String currentStatus = "SAFE";

  // ===================================================
  // MPU6050
  // ===================================================

  sensors_event_t acceleration;
  sensors_event_t gyro;
  sensors_event_t sensorTemp;

  mpu.getEvent(
    &acceleration,
    &gyro,
    &sensorTemp
  );

  // ===================================================
  // TEMPERATURE
  // ===================================================

  tempSensor.requestTemperatures();

  float temperature =
    tempSensor.getTempCByIndex(0);

  // ===================================================
  // VISIBILITY
  // ===================================================

  int visibilityValue =
    analogRead(VISIBILITY_SENSOR);

  bool poorVisibility =
    visibilityValue < VISIBILITY_LIMIT;

  // ===================================================
  // MQ-2 GAS
  // ===================================================

  int gasValue =
    analogRead(MQ2_SENSOR);

  bool dangerousGas =
    gasValue > GAS_LIMIT;

  // ===================================================
  // BATTERY
  // ===================================================

  int batteryRaw =
    analogRead(BATTERY_SENSOR);

  int batteryPercent =
    map(
      batteryRaw,
      0,
      4095,
      0,
      100
    );

  batteryPercent =
    constrain(
      batteryPercent,
      0,
      100
    );

  bool lowBattery =
    batteryPercent <= LOW_BATTERY;

  // ===================================================
  // SOS BUTTON
  // ===================================================

  if (digitalRead(SOS_BUTTON) == LOW) {

    emergency = true;

    currentStatus = "SOS";
  }

  // ===================================================
  // FALL DETECTION
  // ===================================================

  if (!emergency &&
      (abs(acceleration.acceleration.x) > FALL_LIMIT ||
       abs(acceleration.acceleration.y) > FALL_LIMIT ||
       abs(acceleration.acceleration.z) > FALL_LIMIT)) {

    emergency = true;

    currentStatus = "FALL";
  }

  // ===================================================
  // HIGH TEMPERATURE
  // ===================================================

  if (!emergency &&
      temperature >= TEMP_LIMIT) {

    emergency = true;

    currentStatus = "HIGH TEMP";
  }

  // ===================================================
  // GAS ALERT
  // ===================================================

  if (!emergency &&
      dangerousGas) {

    emergency = true;

    currentStatus = "GAS ALERT";
  }

  // ===================================================
  // LOW VISIBILITY
  // ===================================================

  if (!emergency &&
      poorVisibility) {

    emergency = true;

    currentStatus = "LOW VISIBILITY";
  }

  // ===================================================
  // LOW BATTERY
  // ===================================================

  if (!emergency &&
      lowBattery) {

    currentStatus = "LOW BATTERY";
  }

  // ===================================================
  // WARNING OUTPUTS
  // ===================================================

  if (emergency) {

    digitalWrite(BUZZER, HIGH);
    digitalWrite(WARNING_LED, HIGH);

  } else {

    digitalWrite(BUZZER, LOW);
    digitalWrite(WARNING_LED, LOW);
  }

  // ===================================================
  // SAVE EMERGENCY EVENT
  // ===================================================

  if (currentStatus != "SAFE" &&
      currentStatus != "LOW BATTERY" &&
      currentStatus != previousStatus) {

    saveEvent(
      currentStatus,
      temperature,
      gasValue,
      visibilityValue,
      batteryPercent
    );
  }

  previousStatus = currentStatus;

  // ===================================================
  // OLED
  // ===================================================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println("FireShield");
  display.println("----------------");

  display.print("T:");
  display.print(temperature, 1);
  display.print("C ");

  display.print("B:");
  display.print(batteryPercent);
  display.println("%");

  display.print("Gas:");
  display.println(gasValue);

  display.print("Vis:");
  display.println(visibilityValue);

  display.print("Status:");
  display.println(currentStatus);

  display.display();

  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  Serial.println();
  Serial.println("============================");

  Serial.print("Temperature : ");
  Serial.print(temperature, 1);
  Serial.println(" C");

  Serial.print("Gas Value   : ");
  Serial.println(gasValue);

  Serial.print("Visibility  : ");
  Serial.println(visibilityValue);

  Serial.print("Battery     : ");
  Serial.print(batteryPercent);
  Serial.println("%");

  Serial.print("Status      : ");
  Serial.println(currentStatus);

  Serial.println("============================");

  delay(500);
}
