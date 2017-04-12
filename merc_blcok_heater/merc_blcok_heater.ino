
// include the GSM library
#include <GSM.h>

// PIN Number for the SIM and the SS Relay
#define PINNUMBER ""
#define relayPin 13

// initialize the library instances
GSM gsmAccess;
GSM_SMS sms;

// Array to hold the number a SMS is retreived from
char senderNumber[20];
char message[200];
int messageLength = 0;
unsigned long heaterTimer;
int heaterTime = 900000; // 15 minutes in milliseconds
bool heaterStatus = 0;
char ownerNumber[] = "+12482524913";
char tempQuery[] = "What is the temperature?";
char heaterOn[] = "Turn the heater on";
char heaterOff[] = "Turn the heater off";

void setup() {
  
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
  Serial.begin(9600);
  // gsm connection state
  boolean notConnected = true;
  // Start GSM connection
  while (notConnected) {
    
    if (gsmAccess.begin(PINNUMBER) == GSM_READY)
      notConnected = false;
    else {
      
      Serial.println("Not connected");
      delay(1000);
    }
  }
  // send boot message to owner
  sms.beginSMS(ownerNumber);
  sms.print("Rough and Ready!");
  sms.endSMS();
}

void loop() {
  
  char c;
  if (sms.available()) {
    
    sms.remoteNumber(senderNumber, 20);
    if (strcmp(ownerNumber, senderNumber) != 0) {
      Serial.println(senderNumber);
      sms.flush();
      sms.beginSMS(senderNumber);
      sms.print("You're not my daddy.");
      sms.endSMS();
    } else {
      
      int i = 0;
      while (c = sms.read()) {
        
        message[i] = c;
        i++;
      }
      messageLength = i;
      parseMessage();
      if (heaterStatus) {
        Serial.println(millis() - heaterTimer);
      }
    }
    clearMessage();
    sms.flush();
  }
  
  if (heaterStatus && (millis() - heaterTimer) >= heaterTime) {
    sms.beginSMS(ownerNumber);
    sms.print("Turning the block heater off after 15 minutes, you can come start me now.");
    sms.endSMS();
    turnHeaterOff();
  }
}

void parseMessage() {
  
  Serial.println(message);
  if (strcmp(message, tempQuery) == 0) {
    
    sms.beginSMS(ownerNumber);
    sms.print("The temperature is: ");
    sms.print(getTemp());
    sms.print(" degrees F.");
    sms.endSMS();
  } else if (strcmp(message, heaterOn) == 0) {
    
    sms.beginSMS(ownerNumber);
    sms.print("Turning the block heater on.");
    sms.endSMS();
    turnHeaterOn();
  } else if (strcmp(message, heaterOff) == 0) {
    
    sms.beginSMS(ownerNumber);
    sms.print("Turning the block heater off.");
    sms.endSMS();
    turnHeaterOff();
  }
}

void turnHeaterOn() {
  
  digitalWrite(relayPin, HIGH);
  heaterStatus = 1;
  heaterTimer = millis();
}

void turnHeaterOff() {
  
  digitalWrite(relayPin, LOW);
  heaterStatus = 0;
  heaterTimer = 0;
}

void clearMessage() {
  
  for (int i = 0; i < 200; i++) {
    message[i] = NULL;
  }
}

double getTemp() {
  
  unsigned int wADC;
  double t;
  // The internal temperature has to be used
  // with the internal reference of 1.1V.
  // Channel 8 can not be selected with
  // the analogRead function yet.
  // Set the internal reference and mux.
  ADMUX = (_BV(REFS1) | _BV(REFS0) | _BV(MUX3));
  ADCSRA |= _BV(ADEN);  // enable the ADC
  delay(20);            // wait for voltages to become stable.
  ADCSRA |= _BV(ADSC);  // Start the ADC
  // Detect end-of-conversion
  while (bit_is_set(ADCSRA,ADSC));
  // Reading register "ADCW" takes care of how to read ADCL and ADCH.
  wADC = ADCW;
  // The offset of 324.31 could be wrong. It is just an indication.
  t = (wADC - 329.31 ) / 1.22;
  t = t * 1.8 + 32;
  // The returned temperature is in degrees Celcius.
  Serial.println(t);
  return (t);
}
