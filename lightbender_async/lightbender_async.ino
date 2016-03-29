#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>

//debugging switch
#define DEBUG 1

#define HANG_TIME 1000
#define DISABLED_TIME 10000
#define FADE_STEPS 25


#define SENSOR_COUNT 4
#define SENSOR_START_PIN 6
#define STRIP_COUNT 4
#define STRIP_LED_COUNT 300
#define STRIP_START_PIN 2


#include "Strip.h"

StripRoutine routine = fade;

Strip *strips[STRIP_COUNT];

void setup() {

  // init strip objects
  for (int i = 0; i < STRIP_COUNT; i++) {
      strips[i]= new Strip(STRIP_LED_COUNT, STRIP_START_PIN + i);
  }
   
  // init input pins
  for(int i=SENSOR_START_PIN; i < SENSOR_COUNT + SENSOR_START_PIN; i++){
      pinMode(i, INPUT);
  }

  // init timer interrupt
  Timer1.initialize(10000);
  Timer1.attachInterrupt(updateStatus);

  #if DEBUG
    Serial.begin(9600);
    Serial.print("Free RAM: ");
    Serial.println(freeRam());
  #endif
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

#if DEBUG
int freeRam() {

  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif
