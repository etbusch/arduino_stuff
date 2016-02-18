#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>
#define HANG_TIME 1000
#define DISABLED_TIME 10000

// serial output pins
#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5

#define SENSOR_COUNT 4
#define SENSOR_START_PIN 6
#define STRIP_COUNT 4
#define STRIP_LED_COUNT 300
#define STRIP_START_PIN 2
#define FADE_STEPS 25

#include "Strip.h"

StripRoutine routine = fade;

Strip *strips[STRIP_COUNT];

void setup() {

  for (int i = 0; i < STRIP_COUNT; i++) {
      strips[i]= new Strip(STRIP_LED_COUNT, STRIP_START_PIN + i);
  }
   
  // init input pins
  for(int i=SENSOR_START_PIN; i < SENSOR_COUNT + SENSOR_START_PIN; i++){
      pinMode(i, INPUT);
  }

  // init timer
  Timer1.initialize(10000);
  Timer1.attachInterrupt(updateStatus);  
}

void loop() {
  
    noInterrupts();
    for (int i = 0; i < STRIP_COUNT; i++) {
      strips[i]->step();
    }
    interrupts();
}

void updateStatus() {
  
  for(int i=SENSOR_START_PIN; i < SENSOR_COUNT + SENSOR_START_PIN; i++) {

      if (digitalRead(i)) {
        strips[i - SENSOR_START_PIN]->startRoutine(routine);
      }
  }
}

