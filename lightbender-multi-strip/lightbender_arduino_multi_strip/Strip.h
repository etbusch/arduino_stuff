#include <Adafruit_NeoPixel.h>

enum StripRoutine {
  fadeInFadeOut,
  intensityFade
}

class Strip {

  private:

    Adafruit_NeoPixel strip;    


  public:
  
  
    Strip(uint16_t ledCount = 300, uint8_t pin = 1) {
      
      strip = Adafruit_NeoPixel(ledCount, pin, NEO_GRB + NEO_KHZ800);
      active = 0;
      changing = 0;

    }

    void step() {

    }

    bool active;
    bool changing;
    int stepsLeft;

    uint32_t color()

    void colorWipe(uint32_t c, uint8_t wait) {
  
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
      }
    }

    void rainbow(uint8_t wait) {
      
      uint16_t i, j;
      for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
          strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
      }
    }

    // Slightly different, this makes the rainbow equally distributed throughout
    void rainbowCycle(uint8_t wait) {
      
      uint16_t i, j;
      for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        
        for(i=0; i< strip.numPixels(); i++) {
          
          strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }
        strip.show();
        delay(wait);
      }
    }

    //Theatre-style crawling lights.
    void theaterChase(uint32_t c, uint8_t wait) {
      
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
    void theaterChaseRainbow(uint8_t wait) {
      for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        
        for (int q=0; q < 3; q++) {
          
          for (int i=0; i < strip.numPixels(); i=i+3) {
            
            strip.setPixelColor(i+q, Wheel((i+j) % 255));    //turn every third pixel on
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
    uint32_t Wheel(byte WheelPos) {
      
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


    uint32_t intensityWheel(byte WheelPos, double intensity) {
      
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


    void fadeInFadeOut(int speed, int holdTime, int r, int g, int b) {

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

    void fadeInFadeOutWheel(int speed, int holdTime, int startColor, int endColor) {

      double intensity = 0;
      int colorDifference = endColor - startColor;
      double wheelPos = startColor;
      double increment = colorDifference / (double)FADE_STEPS;
      double intensityIncrement = 1 / (double)FADE_STEPS;
      uint32_t color;
      for (int i = 0; i < FADE_STEPS; i++) {

        wheelPos += increment;
        intensity += intensityIncrement;
        color = intensityWheel((uint8_t)wheelPos, intensity);
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
        color = intensityWheel((uint8_t)wheelPos, intensity);
        for(int j=0; j<strip.numPixels(); j++) {

          strip.setPixelColor(j, color);
        }
        strip.show();
        delay(speed);
      }
    }

    void fadeInWheel(int speed, int startColor, int endColor) {

      double intensity = 0;
      int colorDifference = endColor - startColor;
      double wheelPos = startColor;
      double increment = colorDifference / (double)FADE_STEPS;
      double intensityIncrement = 1 / (double)FADE_STEPS;
      uint32_t color;
      for (int i = 0; i < FADE_STEPS; i++) {

        wheelPos += increment;
        intensity += intensityIncrement;
        color = intensityWheel((uint8_t)wheelPos, intensity);
        for(int j=0; j<strip.numPixels(); j++) {

          strip.setPixelColor(j, color);
        }
        strip.show();
        delay(speed);
      }
    }

    void fadeOutWheel(int speed, int startColor, int endColor) {

      double intensity = 1;
      int colorDifference = endColor - startColor;
      double wheelPos = endColor;
      double increment = colorDifference / (double)FADE_STEPS;
      double intensityIncrement = 1 / (double)FADE_STEPS;
      uint32_t color;
      for (int i = FADE_STEPS; i > 0; i--) {

        wheelPos -= increment;
        intensity -= intensityIncrement;
        color = intensityWheel((uint8_t)wheelPos, intensity);
        for(int j=0; j<strip.numPixels(); j++) {

          strip.setPixelColor(j, color);
        }
        strip.show();
        delay(speed);
      }
    }

};
