// LCD Library
#include <LiquidCrystal.h>
// PID Library
#include <PID_v1.h>
#include <PID_AutoTune_v0.h>
// temp sensor library
#include "max6675.h"
// So we can save and retrieve settings
#include <EEPROM.h>

// ************************************************
// Pin definitions
// ************************************************

// Output Relay
#define RelayPin 7
#define aPin 11
#define bPin 10
#define cPin 9

int thermoDO = 4;
int thermoCS = 3;
int thermoCLK = 2;

MAX6675 thermocouple (thermoCLK, thermoCS, thermoDO);
int vccPin = 5;
int gndPin = 6;

/*
 LCD circuit:
 * LCD RS pin to digital pin A0
 * LCD Enable pin to digital pin A1
 * LCD D4 pin to digital pin A2
 * LCD D5 pin to digital pin A3
 * LCD D6 pin to digital pin A4
 * LCD D7 pin to digital pin A5
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 */

// ************************************************
// PID Variables and constants
// ************************************************

//Define Variables we'll be connecting to
double Setpoint;
double Input;
double Output;

volatile long onTime = 0;

// pid tuning parameters
double Kp;
double Ki;
double Kd;

// EEPROM addresses for persisted data
const int SpAddress = 0;
const int KpAddress = 8;
const int KiAddress = 16;
const int KdAddress = 24;

//Specify the links and initial tuning parameters
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);

// 10 second Time Proportional Output window
int WindowSize = 10000; 
unsigned long windowStartTime;

// ************************************************
// Auto Tune Variables and constants
// ************************************************
byte ATuneModeRemember=2;

double aTuneStep=500;
double aTuneNoise=3;
unsigned int aTuneLookBack=60;

boolean tuning = false;

PID_ATune aTune(&Input, &Output);

// ************************************************
// DiSplay Variables and constants
// ************************************************

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

unsigned long lastInput = 0; // last button press

byte degree[8] = // define the degree symbol 
{ 
 B00110, 
 B01001, 
 B01001, 
 B00110, 
 B00000,
 B00000, 
 B00000, 
 B00000 
}; 

const int logInterval = 10000; // log every 10 seconds
long lastLogTime = 0;

// ************************************************
// States for state machine
// ************************************************
enum operatingState { OFF = 0, SETP, RUN, TUNE_P, TUNE_I, TUNE_D, AUTO};
operatingState opState = OFF;


// ************************************************
// Setup and diSplay initial screen
// ************************************************
void setup()
{
   Serial.begin(9600);
   

   // Initialize Relay Control:
   pinMode(aPin, INPUT_PULLUP);
   pinMode(bPin, INPUT_PULLUP);
   pinMode(cPin, INPUT_PULLUP);
   pinMode(RelayPin, OUTPUT);    // Output mode to drive relay
   pinMode(13, OUTPUT);    // Output mode to drive relay
   digitalWrite(RelayPin, LOW);  // make sure it is off to start
   digitalWrite(13, LOW);  // make sure it is off to start

   // Set up Ground & Power for the sensor from GPIO pins
    
    pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
    pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);

   // Initialize LCD DiSplay 

   lcd.begin(16, 2);
   lcd.createChar(1, degree); // create degree symbol from the binary
   lcd.setCursor(0, 0);
   lcd.print(F("    Eric's"));
   lcd.setCursor(0, 1);
   lcd.print(F("   Sous Vide"));

   // Start up the DS18B20 One Wire Temperature Sensor

   delay(3000);  // Splash screen

   // Initialize the PID and related variables
   LoadParameters();
   myPID.SetTunings(Kp,Ki,Kd);

   myPID.SetSampleTime(1000);
   myPID.SetOutputLimits(0, WindowSize);

  // Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;
}

// ************************************************
// Timer Interrupt Handler
// ************************************************
SIGNAL(TIMER2_OVF_vect) 
{
  if (opState == OFF)
  {
    turnOff();  // make sure relay is off
  }
  else
  {
    DriveOutput();
  }
}

// ************************************************
// Main Control Loop
//
// All state changes pass through here
// ************************************************
void loop()
{
   // wait for button release before changing state
   while(ReadButtons() != 0) {}

   lcd.clear();

   switch (opState)
   {
   case OFF:
      Off();
      break;
   case SETP:
      Tune_Sp();
      break;
    case RUN:
      Run();
      break;
   }
}

// ************************************************
// Initial State - press RIGHT to enter setpoint
// ************************************************
void Off()
{
   myPID.SetMode(MANUAL);
   turnOff();
   lcd.print(F("   Erics"));
   lcd.setCursor(0, 1);
   lcd.print(F("   Sous Vide!"));
   uint8_t buttons = 0;
   
   while(buttons != 3)
   {
      buttons = ReadButtons();
   }
   // Prepare to transition to the RUN state
    // Start an asynchronous temperature reading

   //turn the PID on
   myPID.SetMode(AUTOMATIC);
   windowStartTime = millis();
   opState = RUN; // start control
}

// ************************************************
// Setpoint Entry State
// UP/DOWN to change setpoint
// RIGHT for tuning parameters
// LEFT for OFF
// SHIFT for 10x tuning
// ************************************************
void Tune_Sp()
{
   lcd.print(F("Set Temperature:"));
   uint8_t buttons = 0;
   while(true)
   {
      buttons = ReadButtons();

      float increment = 1;
      if (buttons == 1)
      {
         Setpoint += increment;
         delay(200);
      }
      if (buttons == 2)
      {
         Setpoint -= increment;
         delay(200);
      }
    
      if ((millis() - lastInput) > 10000)  // return to RUN after 10 seconds idle
      {
         opState = RUN;
         return;
      }
      lcd.setCursor(0,1);
      lcd.print(Setpoint);
      lcd.print(" ");
      DoControl();
   }
}


// ************************************************
// PID COntrol State
// SHIFT and RIGHT for autotune
// RIGHT - Setpoint
// LEFT - OFF
// ************************************************
void Run()
{
   // set up the LCD's number of rows and columns: 
   lcd.print(F("Sp: "));
   lcd.print(Setpoint);
   lcd.write(1);
   lcd.print(F("C : "));

   SaveParameters();
   myPID.SetTunings(Kp,Ki,Kd);

   uint8_t buttons = 0;
   while(true)
   {

      buttons = ReadButtons();
      if ((buttons == 4) && (abs(Input - Setpoint) < 0.5))  // Should be at steady-state
      {
         StartAutoTune();
      }
      else if (buttons == 2)
      {
        opState = SETP;
        return;
      }
      else if (buttons == 3)
      {
        opState = OFF;
        return;
      }
      
      DoControl();
      
      lcd.setCursor(0,1);
      lcd.print(Input);
      lcd.write(1);
      lcd.print(F("C : "));
      
      float pct = map(Output, 0, WindowSize, 0, 1000);
      lcd.setCursor(10,1);
      lcd.print(F("      "));
      lcd.setCursor(10,1);
      lcd.print(pct/10);
      //lcd.print(Output);
      lcd.print("%");

      lcd.setCursor(15,0);
      if (tuning)
      {
        lcd.print("T");
      }
      else
      {
        lcd.print(" ");
      }
      
      // periodically log to serial port in csv format
      if (millis() - lastLogTime > logInterval)  
      {
        Serial.print(Input);
        Serial.print(",");
        Serial.println(Output);
      }

      delay(250);
   }
}

// ************************************************
// Execute the control loop
// ************************************************
void DoControl()
{
  // Read the input:
    Input = thermocouple.readCelsius();
  
  if (tuning) // run the auto-tuner
  {
     if (aTune.Runtime()) // returns 'true' when done
     {
        FinishAutoTune();
     }
  }
  else // Execute control algorithm
  {
     myPID.Compute();
  }
  
  // Time Proportional relay state is updated regularly via timer interrupt.
  onTime = Output; 
}

// ************************************************
// Called by ISR every 15ms to drive the output
// ************************************************
void DriveOutput()
{  
  long now = millis();
  // Set the output
  // "on time" is proportional to the PID output
  if(now - windowStartTime>WindowSize)
  { //time to shift the Relay Window
     windowStartTime += WindowSize;
  }
  if((onTime > 100) && (onTime > (now - windowStartTime)))
  {
     turnOn();
  }
  else
  {
     turnOff();
  }
}


// ************************************************
// Start the Auto-Tuning cycle
// ************************************************

void StartAutoTune()
{
   // REmember the mode we were in
   ATuneModeRemember = myPID.GetMode();

   // set up the auto-tune parameters
   aTune.SetNoiseBand(aTuneNoise);
   aTune.SetOutputStep(aTuneStep);
   aTune.SetLookbackSec((int)aTuneLookBack);
   tuning = true;
}

// ************************************************
// Return to normal control
// ************************************************
void FinishAutoTune()
{
   tuning = false;

   // Extract the auto-tune calculated parameters
   Kp = aTune.GetKp();
   Ki = aTune.GetKi();
   Kd = aTune.GetKd();

   // Re-tune the PID and revert to normal control mode
   myPID.SetTunings(Kp,Ki,Kd);
   myPID.SetMode(ATuneModeRemember);
   
   // Persist any changed parameters to EEPROM
   SaveParameters();
}

// ************************************************
// Check buttons and time-stamp the last press
// ************************************************
uint8_t ReadButtons()
{
  uint8_t buttons = 0;
  if (digitalRead(aPin) == LOW && digitalRead(bPin)== LOW && digitalRead(cPin) == HIGH) {
    buttons = 4;
  } else if (digitalRead(aPin)== LOW) {
    buttons = 1;
  } else if (digitalRead(bPin)== LOW) {
    buttons = 2;
  } else if (digitalRead(cPin)== LOW) {
    buttons = 3;
  }

  if (buttons != 0)
  {
    lastInput = millis();
  }
  return buttons;
}

// ************************************************
// Save any parameter changes to EEPROM
// ************************************************
void SaveParameters()
{
   if (Setpoint != EEPROM_readDouble(SpAddress))
   {
      EEPROM_writeDouble(SpAddress, Setpoint);
   }
   if (Kp != EEPROM_readDouble(KpAddress))
   {
      EEPROM_writeDouble(KpAddress, Kp);
   }
   if (Ki != EEPROM_readDouble(KiAddress))
   {
      EEPROM_writeDouble(KiAddress, Ki);
   }
   if (Kd != EEPROM_readDouble(KdAddress))
   {
      EEPROM_writeDouble(KdAddress, Kd);
   }
}

// ************************************************
// Load parameters from EEPROM
// ************************************************
void LoadParameters()
{
  // Load from EEPROM
   Setpoint = EEPROM_readDouble(SpAddress);
   Kp = EEPROM_readDouble(KpAddress);
   Ki = EEPROM_readDouble(KiAddress);
   Kd = EEPROM_readDouble(KdAddress);
   
   // Use defaults if EEPROM values are invalid
   if (isnan(Setpoint) || Setpoint == 0)
   {
     Setpoint = 60;
   }
   if (isnan(Kp) || Kp == 0)
   {
     Kp = 850;
   }
   if (isnan(Ki) || Ki == 0)
   {
     Ki = 0.5;
   }
   if (isnan(Kd) || Kd == 0)
   {
     Kd = 0.1;
   }  
}


// ************************************************
// Write floating point values to EEPROM
// ************************************************
void EEPROM_writeDouble(int address, double value)
{
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
      EEPROM.write(address++, *p++);
   }
}

// ************************************************
// Read floating point values from EEPROM
// ************************************************
double EEPROM_readDouble(int address)
{
   double value = 0.0;
   byte* p = (byte*)(void*)&value;
   for (int i = 0; i < sizeof(value); i++)
   {
      *p++ = EEPROM.read(address++);
   }
   return value;
}


// turn on heater
void turnOn() {
  digitalWrite(RelayPin, HIGH); 
  digitalWrite(13, HIGH); 
}

// turn off heater
void turnOff() {
  digitalWrite(RelayPin, LOW); 
  digitalWrite(13, LOW); 
}
