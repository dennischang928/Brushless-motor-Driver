#include <SPI.h>

#define INHA PA8
#define INLA PA9

#define INHB PA10
#define INLB PB6

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

#define chipSelectPin PA4

void setup() {
  DRV8305_SPI_Setup();

  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);
  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);
}
void loop() {
  writeRegister(0b0010, 0b00000000000);
  readRegister(0b0010);
  delay(1000);
  SwitchA(HIGH);
  SwitchB(HIGH);
  SwitchC(LOW);
}

void SwitchA (bool Status) {
  digitalWrite(INHA, Status);
  digitalWrite(INLA, !Status);
}
void SwitchB (bool Status) {
  digitalWrite(INHB, Status);
  digitalWrite(INLB, !Status);
}
void SwitchC (bool Status) {
  digitalWrite(INHC, Status);
  digitalWrite(INLC, !Status);
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

unsigned int readRegister(word Register) {
  word dataToSend = 0x0000, dataToRead = 0;
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

void writeRegister(int Register, word Value) {
  word dataToSend = 0;
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
  //  SPI.transfer16(dataToSend);
  //  Serial.print("First try: ");  Serial.println(SPI.transfer16(dataToSend), BIN);
  //  delay(100);
  Serial.print("Second try: "); Serial.println(SPI.transfer16(dataToSend), BIN); //Send value to record into register
  digitalWrite(chipSelectPin, HIGH);
  Serial.println("---------^^-Write Command-^^--------");
}
