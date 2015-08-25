// Reference our dependent libraries
#include <Wire.h>
#include <SevSeg.h>
#include <HMC5883L.h>

// Store our compass as a variable.
HMC5883L compass;
SevSeg myDisplay;

// Record any errors that may occur in the compass.
int error = 0;
unsigned long timer;
int deciSecond = 0;

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  // Initialize the serial port.
  Serial.begin(9600);
  Wire.begin(); // Start the I2C interface.

  //setup compass
  setupCompass();
  
  //setup display
  setupDisplay();
  
}

void setupCompass() {
  compass = HMC5883L(); // Construct a new HMC5883 compass.
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    Serial.println(compass.GetErrorText(error));
}

void setupDisplay() {
  
   int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode

  
    //This pinout is for a bubble dispaly
       //Declare what pins are connected to the GND pins (cathodes)
       int digit1 = 8; //Pin 1
       int digit2 = 5; //Pin 10
       int digit3 = 11; //Pin 4
       int digit4 = 13; //Pin 6
       
       //Declare what pins are connected to the segments (anodes)
       int segA = 7; //Pin 12
       int segB = 6; //Pin 11
       int segC = 10; //Pin 3
       int segD = 3; //Pin 8
       int segE = 9; //Pin 2
       int segF = 4; //Pin 9
       int segG = 2; //Pin 7
       int segDP= 12; //Pin 5
   
  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level 
}

// Our main program loop.
void loop()
{
 
 myDisplay.DisplayString(convToDirection(getHeading()), 0);
 
}


double getHeading() {
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2� 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = -0.227234502;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  return heading * 180/M_PI; 
}

//using this chart
/*
N  348.75 - 11.25
NNE  11.25 - 33.75
NE  33.75 - 56.25
ENE  56.25 - 78.75
E  78.75 - 101.25
ESE  101.25 - 123.75
SE  123.75 - 146.25
SSE  146.25 - 168.75
S  168.75 - 191.25
SSW  191.25 - 213.75
SW  213.75 - 236.25
WSW  236.25 - 258.75
W  258.75 - 281.25
WNW  281.25 - 303.75
NW  303.75 - 326.25
NNW  326.25 - 348.75
*/
char* convToDirection(float heading) {

  if (heading >= 348.75 || heading < 11.25)
     return "   N";
  else if (heading >= 11.25 && heading < 33.75)
     return " NNE";
  else if (heading >= 33.75 && heading < 56.25)
     return "  NE";
  else if (heading >= 56.25 && heading < 78.75)
     return " ENE";
  else if (heading >= 78.75 && heading < 101.25)
     return "   E";
  else if (heading >= 101.25 && heading < 123.75)
     return " ESE";
  else if (heading >= 123.75 && heading < 146.25)
     return "  SE";
  else if (heading >= 146.25 && heading < 168.75)
     return " SSE";
  else if (heading >= 168.75 && heading < 191.25)
     return "   S";
  else if (heading >= 191.25 && heading < 213.75)
     return " SSU";
  else if (heading >= 213.75 && heading < 236.25)
     return "  SU";
  else if (heading >= 236.25 && heading < 258.75)
     return " USU";
  else if (heading >= 258.75 && heading < 281.25)
     return "   U";
  else if (heading >= 281.25 && heading < 303.75)
     return " UNU";
  else if (heading >= 303.75 && heading < 326.25)
     return "  NU";
  else if (heading >= 326.25 && heading < 348.75)
     return " NNU";
  else
     return "ERRR";
}
