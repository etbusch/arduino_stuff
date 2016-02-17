#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

// serial output pins
#define PIN1 2
#define PIN2 3
#define PIN3 4
#define PIN4 5

// fade step count
#define FADE_STEPS 25

//full brightness hang time
#define HANG_TIME 5000
#define DISABLED_TIME 15000

#define SENSOR_COUNT 4
#define SENSOR_START_PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)

#define STRIP_COUNT 4

#include "Strip.h"

Strip strip1 = Strip();

Adafruit_NeoPixel strips[STRIP_COUNT] = {
  Adafruit_NeoPixel(300, PIN1, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(300, PIN2, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(300, PIN3, NEO_GRB + NEO_KHZ800),
  Adafruit_NeoPixel(300, PIN4, NEO_GRB + NEO_KHZ800)
};

int stripTimeout[STRIP_COUNT] = {0};
int stripLockout[STRIP_COUNT] = {0};
int stripActive[STRIP_COUNT] = {0};

void setup() {

  for(int i=SENSOR_START_PIN; i< SENSOR_COUNT; i++){
      pinMode(i, INPUT);
  }
  for (int i = 0; i < STRIP_COUNT; i++) {
    strips[i].begin(); //init neopixel object
    strips[i].show(); // Initialize all pixels to 'off'

  }
}

void loop() {
  int stripNum;
  int startColor = 110;
  int endColor = 140;

  for(int i = SENSOR_START_PIN; i < SENSOR_COUNT + SENSOR_START_PIN; i++){

      stripNum = i - SENSOR_START_PIN;
      if (digitalRead(i) && !stripActive[stripNum] && millis() > stripLockout[stripNum]) {

        fadeInWheel(strips[stripNum], 0, startColor, endColor);

        stripTimeout[stripNum] = millis() + HANG_TIME;
        stripActive[stripNum] = 1;
      }
      if (stripActive[stripNum] && millis() > stripTimeout[stripNum]) {

        fadeOutWheel(strips[stripNum], 0, startColor, endColor);

        stripActive[stripNum] = 0;
        stripLockout[stripNum] = millis() + DISABLED_TIME;
      }
  }


  // for (int i = 0; i < STRIP_COUNT; i++) {
  //   fadeInWheel(strips[i], 0, startColor, endColor);
  // }
  // delay(HANG_TIME);
  // for (int i = 0; i < STRIP_COUNT; i++) {
  //   fadeOutWheel(strips[i], 0, startColor, endColor);
  // }
  // delay(HANG_TIME);
}

// Fill the dots one after the other with a color
void colorWipe(Adafruit_NeoPixel &strip, uint32_t c, uint8_t wait) {
  
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(Adafruit_NeoPixel &strip, uint8_t wait) {
  
  uint16_t i, j;
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(strip, (i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(Adafruit_NeoPixel &strip, uint8_t wait) {
  
  uint16_t i, j;
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    
    for(i=0; i< strip.numPixels(); i++) {
      
      strip.setPixelColor(i, Wheel(strip, ((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(Adafruit_NeoPixel &strip, uint32_t c, uint8_t wait) {
  
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    
    for (int q=0; q < 3; q++) {
      
      for (int i=0; i < strip.numPixels(); i=i+3) {
        
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(Adafruit_NeoPixel &strip, uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    
    for (int q=0; q < 3; q++) {
      
      for (int i=0; i < strip.numPixels(); i=i+3) {
        
        strip.setPixelColor(i+q, Wheel(strip, (i+j) % 255));    //turn every third pixel on
      }
      strip.show();
      delay(wait);
      for (int i=0; i < strip.numPixels(); i=i+3) {
        
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(Adafruit_NeoPixel &strip, byte WheelPos) {
  
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


uint32_t intensityWheel(Adafruit_NeoPixel &strip, byte WheelPos, double intensity) {
  
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    
    return strip.Color((255 - WheelPos * 3) * intensity, 0, (WheelPos * 3) * intensity);
  }
  if(WheelPos < 170) {
    
    WheelPos -= 85;
    return strip.Color(0, (WheelPos * 3) * intensity, (255 - WheelPos * 3) * intensity);
  }
  WheelPos -= 170;
  return strip.Color((WheelPos * 3) * intensity, (255 - WheelPos * 3) * intensity, 0);
}


void fadeInFadeOut(Adafruit_NeoPixel &strip, int speed, int holdTime, int r, int g, int b) {

  float intensity = 0;
  uint32_t color;
  for (int i = 0; i < FADE_STEPS; i++) {

    intensity += 0.01;
    color = strip.Color(r * intensity, g * intensity, b * intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
  intensity = 1;
  delay(holdTime);
  for (int i = FADE_STEPS; i > 0; i--) {

    intensity -= 0.01;
    color = strip.Color(r * intensity, g * intensity, b * intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
}

void fadeInFadeOutWheel(Adafruit_NeoPixel &strip, int speed, int holdTime, int startColor, int endColor) {

  double intensity = 0;
  int colorDifference = endColor - startColor;
  double wheelPos = startColor;
  double increment = colorDifference / (double)FADE_STEPS;
  double intensityIncrement = 1 / (double)FADE_STEPS;
  uint32_t color;
  for (int i = 0; i < FADE_STEPS; i++) {

    wheelPos += increment;
    intensity += intensityIncrement;
    color = intensityWheel(strip, (uint8_t)wheelPos, intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
  intensity = 1;
  wheelPos = endColor;
  delay(holdTime);
  for (int i = FADE_STEPS; i > 0; i--) {

    wheelPos -= increment;
    intensity -= intensityIncrement;
    color = intensityWheel(strip, (uint8_t)wheelPos, intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
}

void fadeInWheel(Adafruit_NeoPixel &strip, int speed, int startColor, int endColor) {

  double intensity = 0;
  int colorDifference = endColor - startColor;
  double wheelPos = startColor;
  double increment = colorDifference / (double)FADE_STEPS;
  double intensityIncrement = 1 / (double)FADE_STEPS;
  uint32_t color;
  for (int i = 0; i < FADE_STEPS; i++) {

    wheelPos += increment;
    intensity += intensityIncrement;
    color = intensityWheel(strip, (uint8_t)wheelPos, intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
}

void fadeOutWheel(Adafruit_NeoPixel &strip, int speed, int startColor, int endColor) {

  double intensity = 1;
  int colorDifference = endColor - startColor;
  double wheelPos = endColor;
  double increment = colorDifference / (double)FADE_STEPS;
  double intensityIncrement = 1 / (double)FADE_STEPS;
  uint32_t color;
  for (int i = FADE_STEPS; i > 0; i--) {

    wheelPos -= increment;
    intensity -= intensityIncrement;
    color = intensityWheel(strip, (uint8_t)wheelPos, intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
}
