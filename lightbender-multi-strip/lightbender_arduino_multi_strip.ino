#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

#define PIN1 3
#define PIN2 5

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(150, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(60, PIN2, NEO_GRB + NEO_KHZ800);

void setup() {
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  digitalWrite(7, HIGH);
  strip1.begin();
  strip2.begin();
  strip1.show(); // Initialize all pixels to 'off'
  strip2.show(); // Initialize all pixels to 'off'
}

void loop() {

    //fadeInFadeOutWheel(strip1, 10, 5000, 255, 81);
    //fadeInFadeOutWheel(strip2, 10, 5000, 255, 81);
    rainbow(strip1, 0);
    rainbow(strip2, 0);
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
  for (int i = 0; i < 100; i++) {

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
  for (int i = 100; i > 0; i--) {

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
  double increment = colorDifference / 100.0;
  uint32_t color;
  for (int i = 0; i < 100; i++) {

    wheelPos += increment;
    intensity += 0.01;
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
  for (int i = 100; i > 0; i--) {

    wheelPos -= increment;
    intensity -= 0.01;
    color = intensityWheel(strip, (uint8_t)wheelPos, intensity);
    for(int j=0; j<strip.numPixels(); j++) {

      strip.setPixelColor(j, color);
    }
    strip.show();
    delay(speed);
  }
}

