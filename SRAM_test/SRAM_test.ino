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
#define outputEnable 12
#define writeEnable 13
#define writeDelay 0

void setup() {
    
    Serial.begin(19200);
    
    // put your setup code here, to run once:
    pinMode(outputEnable, OUTPUT);
    pinMode(writeEnable, OUTPUT);
    
    pinMode(Add0, OUTPUT);
    pinMode(Add1, OUTPUT);
    pinMode(Add2, OUTPUT);
    pinMode(Add3, OUTPUT);
    pinMode(Add4, OUTPUT);
    pinMode(Add5, OUTPUT);
    pinMode(Add6, OUTPUT);
    pinMode(Add7, OUTPUT);
    
    setDataOut();
    
    
    
    // init state
    digitalWrite(writeEnable, HIGH);
    
}
byte mess = 255;
byte add = 0;
byte response;
void loop() {
    // put your main code here, to run repeatedly:
    

    writeToRam(add, mess);

    response = readFromRam(add);
   
    if (response == mess) {
      
    Serial.print("correct add: ");
    Serial.println(add);
    } else {
      Serial.print("error add: ");
    Serial.println(add);
    }
    mess--;
    add++;
}


void writeToRam(byte address, byte message) {
    setDataOut();
    digitalWrite(outputEnable, HIGH);
    //Serial.println(len);
    
        
            Serial.print("writ byte: ");
            for (int j = 0; j < 8; j++) {
                Serial.print(bitRead(message, j));
            }
        Serial.print(" char: ");
        Serial.println((char)message);
        //write address to port
        digitalWrite(Add0, bitRead(address, 0));
        digitalWrite(Add1, bitRead(address, 1));
        digitalWrite(Add2, bitRead(address, 2));
        digitalWrite(Add3, bitRead(address, 3));
        digitalWrite(Add4, bitRead(address, 4));
        digitalWrite(Add5, bitRead(address, 5));
        digitalWrite(Add6, bitRead(address, 6));
        digitalWrite(Add7, bitRead(address, 7));
        
        //write data to port
        digitalWrite(D0, bitRead(message, 0));
        digitalWrite(D1, bitRead(message, 1));
        digitalWrite(D2, bitRead(message, 2));
        digitalWrite(D3, bitRead(message, 3));
        digitalWrite(D4, bitRead(message, 4));
        digitalWrite(D5, bitRead(message, 5));
        digitalWrite(D6, bitRead(message, 6));
        digitalWrite(D7, bitRead(message, 7));
        
        digitalWrite(writeEnable, LOW);
        delay(writeDelay);
        digitalWrite(writeEnable, HIGH);
    
}

byte readFromRam(byte address) {
    
    
    char data;
    digitalWrite(writeEnable, HIGH);
    setDataIn();
    //set address
    digitalWrite(Add0, bitRead(address, 0));
    digitalWrite(Add1, bitRead(address, 1));
    digitalWrite(Add2, bitRead(address, 2));
    digitalWrite(Add3, bitRead(address, 3));
    digitalWrite(Add4, bitRead(address, 4));
    digitalWrite(Add5, bitRead(address, 5));
    digitalWrite(Add6, bitRead(address, 6));
    digitalWrite(Add7, bitRead(address, 7));
    digitalWrite(outputEnable, LOW);
    
    
    
    delay(writeDelay);
    bitWrite(data, 0, digitalRead(D0));
    bitWrite(data, 1, digitalRead(D1));
    bitWrite(data, 2, digitalRead(D2));
    bitWrite(data, 3, digitalRead(D3));
    bitWrite(data, 4, digitalRead(D4));
    bitWrite(data, 5, digitalRead(D5));
    bitWrite(data, 6, digitalRead(D6));
    bitWrite(data, 7, digitalRead(D7));
    

        Serial.print("read byte: ");
        for (int j = 0; j < 8; j++) {
            Serial.print(bitRead(data, j));
        }
        
        Serial.print(" read: ");
        Serial.println(data);
    
    return data;
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
