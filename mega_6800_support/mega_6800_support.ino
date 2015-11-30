// pins config
// PORTF = address low byte
// PORTK = address high byte
// PORTC = Data
#define romCS 53
#define writeEnable 13

//constants



//PROM DATA
const byte PROGMEM prom[] = {0x01, 0x01, 0x7E, 0x00, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7E, 0x00, 0x00};



void setup() {

  Serial.begin(38400);

  //set address lines as inputs
  DDRF = B00000000;
  DDRK = B00000000;
  
  
  
  //set data lines to 0, so CPU resets operation to memory address 0;
  writeData(0);
}

void loop() {
  //write data for instructions
  if (digitalRead(romCS)){

    byte add = readLowAddress();
    
    if (add == 254 || add == 255) {
      writeData(0);
    } else {
     outputPROM(add);
   /*  Serial.print("address: ");
      Serial.print(add);
      //serialOutByte(add);
     Serial.print(" data: ");
      serialOutByte(prom[add]);
      Serial.println(); */
    }
  }

}

void setDataIn() {
  DDRC = B00000000;
}

void setDataOut() {
  DDRC = B11111111;
}

byte readLowAddress() {
  
  return PINF;
}

byte readHighAddress() {
  
  return PINK;
}

byte readData() {
  setDataIn();
  return PORTC;
}

void writeData(byte message) {
  setDataOut();
  PORTC = message;
}

void outputPROM(byte address) {
  byte data = prom[address];
  writeData(data);
}

void serialOutByte(byte message) {
  for (int j = 7; j > -1; j--) {
      Serial.print(bitRead(message, j));
  }
}
