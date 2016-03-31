
#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

//debugging switch
#define DEBUG 1

#define HANG_TIME 1000
#define DISABLED_TIME 10000
#define FADE_STEPS 25

#define STRIP_COUNT 2
#define STRIP_LED_COUNT 300
#define STRIP_START_PIN 2


#include "Strip.h"

StripRoutine routine = fade;

Strip *strips[STRIP_COUNT];

SoftwareSerial serial(20, 21, true);

void setup() {

  // init strip objects
  for (int i = 0; i < STRIP_COUNT; i++) {
      strips[i]= new Strip(STRIP_LED_COUNT, STRIP_START_PIN + i);
  }

  // init timer interrupt
 // Timer1.initialize(10000);
 // Timer1.attachInterrupt(updateStatus);

  #if DEBUG
    Serial.begin(9600);
    Serial.println("hello");
  #endif

  // flush bootup garbage from the serial buffer
  serial.begin(9600);
  for (int i = 0; i < 256; i++) {
    serial.read();
  }
}

void loop() {
  

    for (int i = 0; i < STRIP_COUNT; i++) {
      strips[i]->step();
    }
    Serial.print("data: ");
    Serial.println(readSensorSerial());


}


int readSensorSerial()
{
    char buffer[3];

    // flush and wait for a range reading
    serial.flush();

    while (!serial.available() || serial.read() != 'R');

    // read the range
    for (int i = 0; i < 3; i++) {
        while (!serial.available());

        buffer[i] = serial.read();
    }

    return atoi(buffer);
}


