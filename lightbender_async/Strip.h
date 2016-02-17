#include <Adafruit_NeoPixel.h>
#define HANG_TIME 5000
#define DISABLED_TIME 15000

enum StripRoutine {
  fade,
  intensityFade
};

class Strip {

  private:

    Adafruit_NeoPixel strip;
    StripRoutine routine;
    int direction;
    int waitUntil;
    double fadeSteps;

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


    uint32_t intensityWheel(byte wheelPos, double intensity) {
      
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

    void fadeStep() {

      if (waiting) {

        if (millis() >= waitUntil) {

          waiting = 0;
          direction = -1;
        }
      } else {
        int stepNum;

        if (direction == 1) {

          stepNum = fadeSteps - stepsLeft;
        } else {

          stepNum = stepsLeft;
        }
        double intensity = stepNum / fadeSteps;
        this->setStripColor(this->color(fadeColor[0], fadeColor[1], fadeColor[2], intensity));
        stepsLeft--;
        if (stepsLeft == 0 && direction == 1) {

          waiting = 1;
          waitUntil = millis() + HANG_TIME;
        } else if (stepsLeft == 0 && direction == -1) {

          active = 0;
        }
      }
    }

  public:

    bool active;
    bool waiting;
    int stepsLeft;
    uint32_t fadeColor[3];
  
    Strip(uint16_t ledCount = 300, uint8_t pin = 1) {
      
      strip = Adafruit_NeoPixel(ledCount, pin, NEO_GRB + NEO_KHZ800);
      strip.begin();
      strip.show();
      active = 0;
      waiting = 0;
      direction = 1;
      fadeSteps = 25;
      fadeColor[0] = 255;
      fadeColor[1] = 255;
      fadeColor[2] = 255;
    }

    void startRoutine(StripRoutine newRoutine) {

      routine = newRoutine;
      active = 1;
      direction = 1;
      stepsLeft = fadeSteps;
    }

    void step() {

      if (active) {

        switch (routine) {

          case fade:
            this->fadeStep();
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
