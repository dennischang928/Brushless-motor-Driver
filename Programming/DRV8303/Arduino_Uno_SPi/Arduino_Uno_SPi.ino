//#include <SPI.h>
//
//word receive;
//
//void setup() {
//  Serial.begin(9600);
//  DRV8305_SPI_Setup();
//  pinMode(8, OUTPUT);
//  digitalWrite(8, HIGH);
//}
//
//int counter  = 0;
//void loop() {
//  sendSPI(0b0001000000001000);
//  //  //
//  Serial.println(receive, BIN);
//  Serial.println("------------^Write^----------");
//  //
//  delay(1000);
//  //  //  sendSPI(0b1001000000000000);
//  //  //  Serial.println(receive, BIN);
//  //  //  Serial.println("------------^READ^----------");
//  //  //  delay(1000);
//  //  counter++;
//  //    while (receive == 0) {
//
//  sendSPI(0b1001000000000000);
//  Serial.println(receive, BIN);
//  Serial.println("------------^READ^----------");
//  delay(1000);
//
//  //    while (true);
//
//  //  }
//
//}
//void DRV8305_SPI_Setup() {
//  SPI.begin(); //Initialize the SPI_1 port.
//  SPI.setBitOrder(MSBFIRST);
//  SPI.setDataMode(SPI_MODE1); //Set the  SPI data mode 1
//  SPI.setClockDivider(SPI_CLOCK_DIV16);      // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
//  pinMode(10, OUTPUT);
//}
//
//void sendSPI(word input)
//{
//  digitalWrite(10, LOW);
//  receive = SPI.transfer16(input);
//  digitalWrite(10, HIGH);
//}













#include <SPI.h>
#define ss digitalWrite(10, LOW);
#define nss digitalWrite(10, HIGH);
const int chipSelectPin = 10;
const int EN_GATE = 8;

void setup() {
  Serial.begin(9600);
  pinMode(chipSelectPin, OUTPUT);
  pinMode(EN_GATE, OUTPUT);
  digitalWrite(EN_GATE, HIGH);
  digitalWrite(chipSelectPin, LOW);
  SPI.begin();
  // DRV8301 communxicates with MSB first and has low clock polarity and data is shifted on rising edge
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1); //  While Idle SPI CLK = 0, falling Egde latching for Rx
}

void loop() {
  //  writeRegister(0b0010, 0b00000001000);
  //  Serial.println("-----------------");
  //  delay(1000);
  writeRegister(0b0010, 0x08);
  delay(100);
  readRegister(0b0010);
  //  Serial.println("-----------------");
  delay(2000);
}

unsigned int readRegister(int Register) {
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
  nss
  ss
  //  SPI.transfer16(dataToSend);
//  Serial.print("First try: ");  Serial.println(SPI.transfer16(dataToSend), BIN);
//  delay(100);
  Serial.print("Second try: "); Serial.println(SPI.transfer16(dataToSend), BIN);
  Serial.println("---------^^-Read Command-^^--------");
  nss
}

void writeRegister(int Register, int Value) {
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
  nss
  ss
  //  SPI.transfer16(dataToSend);
//  Serial.print("First try: ");  Serial.println(SPI.transfer16(dataToSend), BIN);
//  delay(100);
  Serial.print("Second try: "); Serial.println(SPI.transfer16(dataToSend), BIN); //Send value to record into register
  nss
  Serial.println("---------^^-Write Command-^^--------");
}
