// The purpose of this sketch is to find how to create a BLE service that a Garmin FR935 can connect to.
#include "BLEHelper.hh"

BLEHelper bleHelper;
unsigned int heartRate = 50;
int change = 1;
uint8_t batteryLevel = 50;

// for pico-d4
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

void setup() {
  Serial.begin(115200);
  for (unsigned long const serialBeginTime = millis(); !Serial && (millis() - serialBeginTime <= 5000);) {}
#if CONFIG_IDF_TARGET_ESP32S3
  rgbLedWrite(RGB_BUILTIN, 0, 0, /*RGB_BRIGHTNESS*/ 8);  // Blue
#else
  pinMode(LED_BUILTIN, OUTPUT);
#endif
  bleHelper.setup();
}

void loop() {
  static unsigned long tick;
  static bool toggle;


  // refresh every 1sec
  if (millis() - tick > 1000) {
    tick = millis();

    heartRate += change;
    batteryLevel = (batteryLevel + change >= 100) ? 100 : (batteryLevel + change);
    if (heartRate >= 100)
      change = -1;
    else if (heartRate <= 60)
      change = 1;

    //Serial.print("HR: ");
    //Serial.print(heartRate);
    //Serial.println();
    // Serial.print("batt: ");
    // Serial.print(batteryLevel);
    // Serial.println();

    if (bleHelper.connected) {

      bleHelper.notifyHeartRate(heartRate);
      //bleHelper.notifyBatteryLevel(batteryLevel);

#if CONFIG_IDF_TARGET_ESP32S3
      rgbLedWrite(RGB_BUILTIN, 0, 0, /*RGB_BRIGHTNESS*/ 8);  // Blue
#else
      digitalWrite(LED_BUILTIN, HIGH);
#endif
    } else {
#if CONFIG_IDF_TARGET_ESP32S3
      rgbLedWrite(RGB_BUILTIN, 0, 0, toggle ? /*RGB_BRIGHTNESS*/ 8 : 0);
#else
      digitalWrite(LED_BUILTIN, toggle ? HIGH : LOW);
#endif
      toggle = !toggle;
    }
  }
}
