#include <Adafruit_CircuitPlayground.h>
#include "Adafruit_VL53L0X.h"
#include "pitches.h"

#define ROTATION_RATE   75    // lower is faster

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int colorIndex;
int startIndex;

unsigned long previousMillis = 0;        // will store last time tone was updated

unsigned long interval = 400;           // interval at which to play tone (milliseconds)
unsigned long currentMillis;
///////////////////////////////////////////////////////////////////////////////
void setup() {
  CircuitPlayground.begin();

  // Start at the beginning
  startIndex = 0;
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }

  Serial.println("Adafruit VL53L0X test");
  Serial.println("oh hai");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}


void loop() {
    VL53L0X_RangingMeasurementData_t measure;
    Serial.println(measure.RangeMilliMeter);

  // Turn off all the NeoPixels
  CircuitPlayground.clearPixels();

  measure.RangeMilliMeter;
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 600) {  // phase failures have incorrect data
    // GETTING CLOSE
    if (measure.RangeMilliMeter > 350 && measure.RangeMilliMeter < 500) {
      // flash yellow!
      CircuitPlayground.setBrightness(50);
      for (int pixel=0; pixel<10; pixel++) {
        CircuitPlayground.setPixelColor(pixel, 0xFFFF00);
        colorIndex++;
        if (colorIndex > 9) colorIndex = 0;
      }
    }

    // TOO CLOSE
    if (measure.RangeMilliMeter <= 350 ) {
      // flash red and beep!
      CircuitPlayground.setBrightness(50);
      for (int pixel=0; pixel<10; pixel++) {
        CircuitPlayground.setPixelColor(pixel, 0xFF0000);
        colorIndex++;
        if (colorIndex > 9) colorIndex = 0;
      }

       currentMillis = millis();

      if (currentMillis - previousMillis >= interval) {
        // save the last time you play tone
        previousMillis = currentMillis;

          CircuitPlayground.playTone(NOTE_F4,200);
          CircuitPlayground.playTone(NOTE_DS4,200);
      }
    }
  } if (measure.RangeMilliMeter > 500 && measure.RangeMilliMeter < 600)  {
      // flash green!
      CircuitPlayground.setBrightness(50);
      for (int pixel=0; pixel<10; pixel++) {
        CircuitPlayground.setPixelColor(pixel, 0x008000);
        colorIndex++;
        if (colorIndex > 9) colorIndex = 0;
      }
  }
    // Increment start index into color array
    startIndex++;

    // Check value and reset if necessary
    if (startIndex > 9) startIndex = 0;

    // Wait a little bit so we don't spin too fast
    delay(ROTATION_RATE);
 }
