#include <SPI.h>

#define INHA PA8
#define INLA PA9

#define INHB PA10
#define INLB PB6

#define INHC PB7
#define INLC PB8
#define EN_PIN PB12

#define BITORDER MSBFIRST
#define chipSelectPin PA4

void setup()
{
  DRV8305_SPI_Setup();

  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);
  pinMode(EN_PIN, OUTPUT);

  digitalWrite(EN_PIN, HIGH);
  Serial.begin(115200);
}
int delayValue = 10;
int PWMValue = 100;

void loop()
{
  analogWrite(INHB, 100);
  analogWrite(INHA, 100);
  // writeRegister(0x02, 0b00000001000);
  delay(1000);
  readRegister(0x00);
}

void DRV8305_SPI_Setup()
{
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, LOW);
  SPI.begin();
  // DRV8301 communxicates with MSB first and has low clock polarity and data is shifted on rising edge
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
}

unsigned int readRegister(word Register)
{
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
  Serial.println(SPI.transfer16(dataToSend), BIN);
  digitalWrite(chipSelectPin, HIGH);

  Serial.println("---------^^-Read Command-^^--------");
  return (dataToRead);
}

void writeRegister(int Register, word Value)
{
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
  Serial.println(SPI.transfer16(dataToSend), BIN);
  digitalWrite(chipSelectPin, HIGH);

  Serial.println("---------^^-Write Command-^^--------");
}