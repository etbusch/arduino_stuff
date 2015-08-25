#include <EEPROM.h>

/* LINE PAINTER

By: ERIC BUSCH

VERSION: ALPHA 1

WHO KNOWS WHAT WORKS AND WHAT DOES NOT

FINISHED:

. BASIC SRTRUCCTURE
. EEPROM READING
. EEPROM WRITING
. MESSAGE INPUT
. WRITER FUNCTION STRUCTURE
. Letter coding except for w


NOT FINISHED:

. LETTER CODING W
. VELOCITY
Te occidere possunt sed te edere non possunt nefas est
*/

// EEPROM related variables
int eepromadd = 0;
int eepromdata = 0; // Existence of EEPROM Data, address 0
int eepromlength = 0; // Length of EEPROM data, in bytes, address 1
int eepromrite = 0;


// Misc variables and functional pins.
int dotdelay = 0;
int gapdelay = 0;
int chargapdelay = 0;
int StartVar = LOW;
char MessageStr[100]; // Character array to read from 
char CurrentChar = 0; // Current character from array being painted.
int StartButton = 8; // Input pin to start writing process
int PanicButton = 9; // INput pin to stop wtiting process, might be changed to interrupt if clock cycles allow
int ReadyLED = 13;  // LED message ready indicator.
int serialready = 0;
int mesagestord = 0;

// Paint Output Pins for relays.

int Rel01 = 3;
int Rel02 = 4;
int Rel03 = 5;
int Rel04 = 6;
int Rel05 = 7;



void setup() {
  
    // Read char array from EEPROM memory if it exists
    eepromdata = EEPROM.read(0);
    if (eepromdata == 1) {
      eepromlength = EEPROM.read(1);
      
      for ( int eepromreed = 2; eepromlength >= eepromreed; eepromreed++ ) {
       MessageStr[eepromreed - 2] = EEPROM.read(eepromreed);
       Serial.println("Message Read from EEPROM.");
      }
      
    }
        
  
    // Initialize relay output pins.
    
    pinMode(Rel01, OUTPUT);
    pinMode(Rel02, OUTPUT);
    pinMode(Rel03, OUTPUT);
    pinMode(Rel04, OUTPUT);
    pinMode(Rel05, OUTPUT);
    
    // Init operation inputs/outputs
  
    pinMode(StartButton, INPUT_PULLUP);
    pinMode(PanicButton, INPUT_PULLUP);
    pinMode(ReadyLED, OUTPUT);
    
    //Init Serial connection
    
    Serial.begin(9600);
    Serial.println("Power On");
    Serial.println("Send an '1' to Enter Message:");
}






void loop()
{
  
  // Read incoming serial data into a char array.
 char serialreed = Serial.peek(); 
 if (serialreed == '1') {
  Serial.println("Enter Message to be written:");
  serialready = 1;
  while (Serial.available() > 0) {
      Serial.read() ;
     }
  }

  for (int charcounter=0; serialready == 1 && Serial.available() > 0; charcounter++){
    // read the incoming byte into array:
    Serial.readBytesUntil(-1,MessageStr, 100);
    }
    
    // Confirm that message has been read into memory and turn on ready LED
    if (MessageStr[0] != NULL && mesagestord == 1) {
     Serial.println("Message Received.");
    Serial.println(MessageStr);
    digitalWrite(ReadyLED, HIGH);
    int mesagestord = 1;
    }
  
 
     //clear serial buffer
     while (Serial.available() > 0) {
      Serial.read() ;
      Serial.println("Cleared");
     }
 
 
  // Start writing process if start button is pressed
  
  if (StartVar == HIGH) {
  if (MessageStr[0] != 0) {
    
    Eepromsave();
    
    Serial.println("Starting Graph NOW! Beware of the fuzz\n");
    for (int eepromrite = 0; MessageStr[eepromrite] != 0; eepromrite++) { 
      writer(MessageStr[eepromrite]);
    }

  }
  }
  
}







//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VARIOUS FUNCTIONS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void Eepromsave()  {
    // Write char array to EEPROM for safe keeping.
  
  if (MessageStr[0] != 0) {
    for (int eepromrite = 0; MessageStr[eepromrite] != 0; eepromrite++);
    { EEPROM.write(eepromrite + 2, MessageStr[eepromrite]);
    }

  }
 
}

void writer(char character) {
  
  // check for stop button press
  int STOP = digitalRead(PanicButton);
  if (STOP = HIGH) return;
  
    // begin interpreting 
    
  switch (character)  {
    
    case 'a':                        //a
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;
    
    case 'b':                        //b
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'c':                        //c
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'd':                        //d
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'e':                        //e
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'f':                        //f
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'g':                        //g
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'h':                        //h
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'i':                        //i
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'j':                        //j
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'k':                        //k
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'l':                        //l
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'm':                        //m
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              4
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              5
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'n':                        //n
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'o':                        //o
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'p':                        //p
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'q':                        //q
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'r':                        //r
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 's':                        //s
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 't':                        //t
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'u':                        //u
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'v':                        //v
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'w':                        //w
    
      // open nozzles              1
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'x':                        //x
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'y':                        //y
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;    

case 'z':                        //z
    
      // open nozzles              1
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, HIGH);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles            2
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, HIGH);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(gapdelay);
      
      // open nozzles              3
      digitalWrite(Rel01, HIGH);
      digitalWrite(Rel02, HIGH);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, HIGH);
      
      // paint
      delay(dotdelay);
      
      //close nozzles
      digitalWrite(Rel01, LOW);
      digitalWrite(Rel02, LOW);
      digitalWrite(Rel03, LOW);
      digitalWrite(Rel04, LOW);
      digitalWrite(Rel05, LOW);
      
      //wait for next line
      delay(chargapdelay);
      
      break;
    
                                // END OF CHARACTER LIST
    default:
      delay(chargapdelay);
    
  }
  

  
  return;
}

