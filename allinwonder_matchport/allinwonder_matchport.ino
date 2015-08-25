/*

*** Channel 1
Baudrate 9600, I/F Mode 4C, Flow 02
Port 10001
Connect Mode : D4
Send '+++' in Modem Mode enabled
Show IP addr after 'RING' enabled
Auto increment source port disabled
Remote IP Adr: --- none ---, Port 00000
Disconn Mode : 00
Flush Mode : 77 

**** Be sure to connect the CTS pin (#6) and the RESET pin (#4)! ****
*/

#include <NewSoftSerial.h>
#include <AF_XPort.h>
#include <avr/io.h>
#include <string.h>

#define PIEZOPIN  12

#define IPADDR "123.34.56.78"  // pop server
#define PORT 110                  // POP3 port most likely used
#define USERNAME "username"  
#define PASSWORD "password"
#define XPORTBAUDRATE 9600
#define SERIALBAUDRATE 57600

char linebuffer[256]; // large buffer for storing data

#define XPORT_RXPIN 2
#define XPORT_TXPIN 3
#define XPORT_RESETPIN 4
#define XPORT_DTRPIN 5
#define XPORT_CTSPIN 6
#define XPORT_RTSPIN 7

AF_XPort xport = AF_XPort(XPORT_RXPIN, XPORT_TXPIN, XPORT_RESETPIN, XPORT_DTRPIN, XPORT_RTSPIN, XPORT_CTSPIN);

uint8_t errno;
uint32_t lastmessagenum = 0, lastmessagesize = 0;

uint32_t parsenumber(char *str) {
  uint32_t num = 0;
  char c;
  
  // grabs a number out of a string
  while (c = str[0]) {
   if ((c < '0') || (c > '9'))
     return num;
   num *= 10;
   num += c - '0';
   str++;
 }
 return num;
}

uint8_t fetchemails(void) {
  uint8_t ret;
  char *found=0;
  int messagenum, messagesize;
  
  ret = xport.reset();
  //Serial.print("Ret: "); Serial.print(ret, HEX);
  switch (ret) {
   case  ERROR_TIMEDOUT: { 
      Serial.println("Timed out on reset!"); 
      return 0;
   }
   case ERROR_BADRESP:  { 
      Serial.println("Bad respons on reset!");
      return 0;
   }
   case ERROR_NONE: { 
    Serial.println("Reset OK!");
    break;
   }
   default:
     Serial.println("Unknown error"); 
     return 0;
  }
  
  // time to connect...
 
  ret = xport.connect(IPADDR, PORT);
  switch (ret) {
   case  ERROR_TIMEDOUT: { 
      Serial.println("Timed out on connect"); 
      return 0;
   }
   case ERROR_BADRESP:  { 
      Serial.println("Failed to connect");
      return 0;
   }
   case ERROR_NONE: { 
     Serial.println("Connected..."); break;
   }
   default:
     Serial.println("Unknown error"); 
     return 0;
  }

  ret = xport.readline_timeout(linebuffer, 255, 3000); // get first line
  Serial.print(linebuffer);
  if (! strstr(linebuffer, "+OK")) {
    xport.println("QUIT");
    return 0;
  }
  ret = xport.readline_timeout(linebuffer, 255, 3000); // dunno why but theres an error, some terminal thing
  Serial.print(linebuffer);

  xport.print("USER "); xport.println(USERNAME);
  Serial.print("USER "); Serial.println(USERNAME);

  ret = xport.readline_timeout(linebuffer, 255, 3000); // get first line
  Serial.print(linebuffer);
  
  if (! strstr(linebuffer, "+OK")) {
    xport.println("QUIT");
    return 0;
  }
  xport.print("PASS "); xport.println(PASSWORD);
  Serial.print("PASS "); Serial.println(PASSWORD);

  ret = xport.readline_timeout(linebuffer, 255, 3000);
  Serial.print(linebuffer);
  if (! strstr(linebuffer, "+OK logged in")) {
    xport.println("QUIT");
    return 0;
  }
 
  xport.println("LIST");
  Serial.println("LIST");

  while (xport.readline_timeout(linebuffer, 255, 3000) && (linebuffer[0] != '.')) {
    Serial.print(linebuffer);
    messagenum = parsenumber(linebuffer);
    
    if (! (found = strchr(linebuffer, ' '))) {
      xport.println("QUIT");
      return 0;
    }
    messagesize = parsenumber(found+1);
    
    Serial.print("Message #"); Serial.print(messagenum); 
    Serial.print(" size: "); Serial.println(messagesize);
  } 
  if (messagenum < lastmessagenum) {
     // some emails were deleted, skip it
     lastmessagenum = messagenum;
     xport.println("QUIT");
     return 0;
  } 
  if (messagenum > lastmessagenum) {
    //new messages!
    ret = messagenum-lastmessagenum;
    lastmessagenum = messagenum;
    
    // print the latest message
    xport.print("RETR "); xport.println(lastmessagenum);
    Serial.print("RETR "); Serial.println(lastmessagenum);
    while (xport.readline_timeout(linebuffer, 255, 3000) && (linebuffer[0] != '.')) {
      Serial.print(linebuffer);
    }
    return ret;
  }
  xport.println("QUIT");
  return 0; 
}


void beep(uint8_t pin, uint16_t freq, int32_t us) {
  int usdelay;
  usdelay = 500000 / freq;
  
  us *= 1000; //in uS
  while (us > 0) {
    digitalWrite(pin, HIGH);
    delayMicroseconds(usdelay);
    digitalWrite(pin, LOW);
    delayMicroseconds(usdelay);
    us -= usdelay*2;
  }  
}


void setup()  {
  
  pinMode(13, OUTPUT);
  pinMode(PIEZOPIN, OUTPUT);
  
  Serial.begin(SERIALBAUDRATE);
  // set the data rate for the SoftwareSerial port
  xport.begin(XPORTBAUDRATE);
  Serial.println("Hello, world...");
}



void loop()                     // run over and over again
{
  if (fetchemails() != 0) {
     // new email!
     Serial.println("\nTWEEEEEEEEET\n");
     beep(PIEZOPIN, 2000, 500); // 2khz, 500ms
    
  }
  delay(30000); // wait ten seconds


