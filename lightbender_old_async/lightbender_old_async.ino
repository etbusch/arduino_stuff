#include <SoftwareSerial.h>
#include <TimerOne.h>
#include <Adafruit_NeoPixel.h>

//debugging switch
#define DEBUG 1

#define HANG_TIME 500
#define DISABLED_TIME 5000
#define FADE_STEPS 30

#define SENSOR_TRIGGER_DISTANCE 50
#define SENSOR_EXCLUDE_DISTANCE 300
#define STRIP_CASCADE_WAIT 100


#define SENSOR_COUNT 4
#define SENSOR_START_PIN 6
#define STRIP_COUNT 4
#define STRIP_LED_COUNT 300
#define STRIP_START_PIN 2

#include "Strip.h"

SoftwareSerial sensorSerial(20, 21, true);

int lastSensorReading = 100;
bool triggered = 0;
int triggeredTimestamp = 0;


StripRoutine routine = fade;

Strip *strips[STRIP_COUNT];

void setup() {

  // init strip objects
  for (int i = 0; i < STRIP_COUNT; i++) {
      strips[i]= new Strip(STRIP_LED_COUNT, STRIP_START_PIN + i);
  }
   
  // init timer interrupt
  Timer1.initialize(100000);
  Timer1.attachInterrupt(updateStatus);

  //begin serial connection for sensor
  sensorSerial.begin(9600);

  #if DEBUG
    Serial.begin(9600);
    Serial.print("Free RAM: ");
    Serial.println(freeRam());
  #endif


}

void loop() {
  
    noInterrupts();
    step();
    interrupts();
}

void step() {
      
  for (int i = 0; i < STRIP_COUNT; i++) {
    
    strips[i]->step();

    if(triggered) {

      int timeSinceTrigger = millis() - triggeredTimestamp;
      int stripToTrigger = timeSinceTrigger / STRIP_CASCADE_WAIT;      
      if (stripToTrigger == i) {

        strips[i]->startRoutine(routine);
      }

      if (stripToTrigger > STRIP_COUNT) {
        
        triggered = 0;
      }
    }
  }
}

void updateStatus() {
  
  int reading = readSensorSerial();
  
  #if DEBUG
    Serial.print("reading: ");
    Serial.print(reading);
    Serial.print(" change: ");
    Serial.println(lastSensorReading - reading);
  #endif
  
  if (lastSensorReading - reading > SENSOR_TRIGGER_DISTANCE && lastSensorReading - reading < SENSOR_EXCLUDE_DISTANCE) {
    
    for (int i=SENSOR_START_PIN; i < SENSOR_COUNT + SENSOR_START_PIN; i++) {

      triggered = 1;
      triggeredTimestamp = millis();
    }
  }
  lastSensorReading = reading;
}



int readSensorSerial() {
  
    char buffer[3];
    if (sensorSerial.available()) {
      
      for (int j = 0; j < 20; j++) {

        if (sensorSerial.read() == 'R') {
          for (int i = 0; i < 3; i++) {
        
            buffer[i] = sensorSerial.read();
          }
          break;
        }
      }
    } else {
      return lastSensorReading;
    }
    return atoi(buffer);
}



#if DEBUG
int freeRam() {

  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}
#endif
