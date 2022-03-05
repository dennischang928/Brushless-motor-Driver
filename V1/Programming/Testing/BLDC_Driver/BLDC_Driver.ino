#include <SPI.h>

#define INHA PA8
#define INLA PA9

#define INHB PA10
#define INLB PB6

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12
#define nFAULT PB13
#define DC_CAL PB4

#define chipSelectPin PA4


void SwitchA (bool Status) {
  digitalWrite(INHA, Status);

}
void SwitchB (bool Status) {
  digitalWrite(INHB, Status);

}
void SwitchC (bool Status) {
  digitalWrite(INHC, Status);

}

void DRV8305_SPI_Setup() {
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, LOW);
  SPI.begin();
  // DRV8301 communxicates with MSB first and has low clock polarity and data is shifted on rising edge
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

}

void readRegister(int Register) {
  uint16_t dataToSend = 0x0000, dataToRead = 0;
  //   bit   1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16
  //   R=1/W=0  |-Raddr--|  |-----------Data---------------------|
  // Fault=1/0  |-Raddr--|  |-----------Data---------------------|
  dataToSend |= 1;

  dataToSend <<= 4;
  dataToSend |= Register % (16);
  dataToSend <<= 11;

  Serial.print("Sending read command: ");
  Serial.println(dataToSend, BIN);
  digitalWrite(chipSelectPin, LOW);
  //  SPI.transfer16(dataToSend);
  //  Serial.print("First try: ");  Serial.println(SPI.transfer16(dataToSend), BIN);
  //  delay(100);
  Serial.print("Second try: "); Serial.println(SPI.transfer16(dataToSend), BIN);
  Serial.println("---------^^-Read Command-^^--------");
  digitalWrite(chipSelectPin, HIGH);
}

void writeRegister(int Register, uint16_t Value) {
  uint16_t dataToSend = 0;
  //   bit   1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16
  //   R=1/W=0  |-Raddr--|  |-----------Data---------------------|
  dataToSend |= 0;
  dataToSend <<= 4;
  dataToSend |= Register % (16);
  dataToSend <<= 11;
  dataToSend |= Value % (2048);
  Serial.print("Senduing write command: ");
  Serial.println(dataToSend, BIN);

  digitalWrite(chipSelectPin, LOW);

  Serial.print("Second try: "); Serial.println(SPI.transfer16(dataToSend), BIN); //Send value to record into register
  digitalWrite(chipSelectPin, HIGH);
  Serial.println("---------^^-Write Command-^^--------");
}

void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(9600);
  DRV8305_SPI_Setup();
  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);

  pinMode(nFAULT, INPUT);
  pinMode(DC_CAL, OUTPUT);
  delay(2000);
  for (int i = 0; i < 3; i++) {
    digitalWrite(PC13, HIGH);
    delay(300);
    digitalWrite(PC13, LOW);
    delay(300);
  }
  digitalWrite(DC_CAL, LOW);
  digitalWrite(EN_GATE, HIGH);
}


void loop() {
  delay(40);
  digitalWrite(chipSelectPin, LOW);

  Serial.print("Second try: "); Serial.println(SPI.transfer(0x02), BIN); //Send value to record into register
  digitalWrite(chipSelectPin, HIGH); 
}

void Nfault_trigger () {
  readRegister(0b0000);
}
