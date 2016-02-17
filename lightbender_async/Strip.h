#include <Adafruit_NeoPixel.h>

enum StripRoutine {
  fade,
  intensityFade
};

class Strip {

  private:

    Adafruit_NeoPixel strip;
    StripRoutine routine;
    int direction;

    void setStripColor(uint32_t color) {
      
      for(uint16_t i=0; i<strip.numPixels(); i++) {
        
        strip.setPixelColor(i, color);
        strip.show();
      }
    }

    // Input a value 0 to 255 to get a color value.
    // The colours are a transition r - g - b - back to r.
    uint32_t wheel(byte wheelPos) {
      
      wheelPos = 255 - wheelPos;
      if(wheelPos < 85) {
        
        return strip.Color(255 - wheelPos * 3, 0, wheelPos * 3);
      }
      if(wheelPos < 170) {
        
        wheelPos -= 85;
        return strip.Color(0, wheelPos * 3, 255 - wheelPos * 3);
      }
      wheelPos -= 170;
      return strip.Color(wheelPos * 3, 255 - wheelPos * 3, 0);
    }


    uint32_t intensitywheel(byte wheelPos, double intensity) {
      
      wheelPos = 255 - wheelPos;
      if(wheelPos < 85) {
        
        return strip.Color((255 - wheelPos * 3) * intensity, 0, (wheelPos * 3) * intensity);
      }
      if(wheelPos < 170) {
        
        wheelPos -= 85;
        return strip.Color(0, (wheelPos * 3) * intensity, (255 - wheelPos * 3) * intensity);
      }
      wheelPos -= 170;
      return strip.Color((wheelPos * 3) * intensity, (255 - wheelPos * 3) * intensity, 0);
    }


  public:

    bool active;
    bool changing;
    int stepsLeft;  
  
    Strip(uint16_t ledCount = 300, uint8_t pin = 1) {
      
      strip = Adafruit_NeoPixel(ledCount, pin, NEO_GRB + NEO_KHZ800);
      active = 0;
      changing = 0;
      direction = 1;
    }

    void startRoutine(StripRoutine newRoutine) {

      routine = newRoutine;
      active = 1;
      direction = 1;
      stepsLeft = FADE_STEPS;
    }

    void step() {

      if (active) {

        switch (routine) {

          case fade:
            
            break;

          case intensityFade:
            break;
        }
      }
    }

    uint32_t color(uint8_t r, uint8_t g, uint8_t b, double intensity = 1) {
      
      return strip.Color(r * intensity, g * intensity, b * intensity);
    }
};
