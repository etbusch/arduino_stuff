// pins config
#define Add0 A0
#define Add1 A1
#define Add2 A2
#define Add3 A3
#define Add4 A4
#define Add5 A5
#define Add6 2
#define Add7 3
#define D0 4
#define D1 5
#define D2 6
#define D3 7
#define D4 8
#define D5 9
#define D6 10
#define D7 11
#define romCS 12
#define writeEnable 13

//constants



//PROM DATA
byte prom[] = {0x01, 0x01, 0x7E, 0x00, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x7E, 0x00, 0x00};



void setup() {

  Serial.begin(38400);

  //set address lines as inputs
  pinMode(Add0, INPUT);
  pinMode(Add1, INPUT);
  pinMode(Add2, INPUT);
  pinMode(Add3, INPUT);
  pinMode(Add4, INPUT);
  pinMode(Add5, INPUT);
  pinMode(Add6, INPUT);
  pinMode(Add7, INPUT);
  pinMode(romCS, INPUT);
  
  //set data lines to 0, so CPU resets operation to memory address 0;
  writeData(0);
}

void loop() {
  //write data for instructions
  if (digitalRead(romCS)){

    byte add = readAddress();
    
    if (add == 254 || add == 255) {
      writeData(0);
    } else {
      outputPROM(add);
//      Serial.print("address: ");
//      Serial.print(add);
//      //serialOutByte(add);
//      Serial.print(" data: ");
//      serialOutByte(prom[add]);
//      Serial.println();
    }
  }

}

void setDataIn() {
  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
}

void setDataOut() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
}

byte readAddress() {
  byte data;
  bitWrite(data, 0, digitalRead(Add0));
  bitWrite(data, 1, digitalRead(Add1));
  bitWrite(data, 2, digitalRead(Add2));
  bitWrite(data, 3, digitalRead(Add3));
  bitWrite(data, 4, digitalRead(Add4));
  bitWrite(data, 5, digitalRead(Add5));
  bitWrite(data, 6, digitalRead(Add6));
  bitWrite(data, 7, digitalRead(Add7));
  return data;
}

byte readData() {
  setDataIn();
  byte data;
  bitWrite(data, 0, digitalRead(D0));
  bitWrite(data, 1, digitalRead(D1));
  bitWrite(data, 2, digitalRead(D2));
  bitWrite(data, 3, digitalRead(D3));
  bitWrite(data, 4, digitalRead(D4));
  bitWrite(data, 5, digitalRead(D5));
  bitWrite(data, 6, digitalRead(D6));
  bitWrite(data, 7, digitalRead(D7));
  return data;
}

void writeData(byte message) {
  setDataOut();
  digitalWrite(D0, bitRead(message, 0));
  digitalWrite(D1, bitRead(message, 1));
  digitalWrite(D2, bitRead(message, 2));
  digitalWrite(D3, bitRead(message, 3));
  digitalWrite(D4, bitRead(message, 4));
  digitalWrite(D5, bitRead(message, 5));
  digitalWrite(D6, bitRead(message, 6));
  digitalWrite(D7, bitRead(message, 7));
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
