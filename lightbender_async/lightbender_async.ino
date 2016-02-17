#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h>

// serial output pins
#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5

#define SENSOR_COUNT 4
#define SENSOR_START_PIN 6
#define STRIP_COUNT 4
#define FADE_STEPS 25

#include "Strip.h"

Strip strips[STRIP_COUNT] = {
  Strip(300, PIN1),
  Strip(300, PIN2),
  Strip(300, PIN3),
  Strip(300, PIN4)
};

void setup() {

  // init input pins
  for(int i=SENSOR_START_PIN; i < SENSOR_COUNT + SENSOR_START_PIN; i++){
      pinMode(i, INPUT);
  }

  // init timer
  MsTimer2::set(20, updateStatus); // 500ms period
  MsTimer2::start();
}

void loop() {

}

void updateStatus() {


}