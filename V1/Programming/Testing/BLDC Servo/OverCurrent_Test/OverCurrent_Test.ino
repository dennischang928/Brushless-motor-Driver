#include <SPI.h>

#define INHA PA8
#define INLA PA9
#define SO1 PB1

#define INHB PA10
#define INLB PB6
#define SO2 PB0

#define INHC PB7
#define INLC PB8
#define SO3 PA3

#define nFAULT PB13

#define LED PC13

#define EN_GATE PB12


//SPI
#define SPI1_NSS_PIN PA4
word data;
//SPI
void setup() {
  //SPI
  SPI.begin(); //Initialize the SPI_1 port.
  SPI.setBitOrder(MSBFIRST); // Set the SPI_1 bit order
  SPI.setDataMode(SPI_MODE1); //Set the  SPI_2 data mode 0
  SPI.setClockDivider(SPI_CLOCK_DIV16);      // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
  pinMode(SPI1_NSS_PIN, OUTPUT);
  //SPI
  Serial.begin(115200);
  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);
  pinMode(SO1, INPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);
  pinMode(SO2, INPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);
  pinMode(SO3, INPUT);

  pinMode(nFAULT, INPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);
  sendSPI(0b0100111000100000); //prevent Voltage Drop shut down
  delay(100);
  sendSPI(0b0011101010010110); //3 pwm mode
  delay(100);
}


void loop() {
  //  sendSPI(0b0100111000100000); //prevent Voltage Drop shut down
  //  delay(100);
  //  sendSPI(0b0011101010010110); //3 pwm mode

  //  sendSPI(0b0110000011001001); // 0b|0|1100|000|11001|001
  //  sendSPI(0b1110000000000000);
  //  sendSPI(0b1000100000000000);// looking for 0x3 fault
  //  sendSPI(0b1010000000000000);

  digitalWrite(INHA, HIGH);
  digitalWrite(INHB, LOW);
  //  delay(1000)
  Serial.println(digitalRead(nFAULT));
  //  Serial.println(data, BIN);
}
void sendSPI(word input)
{
  digitalWrite(SPI1_NSS_PIN, LOW); // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(input); //Send the HEX data 0x55 over SPI-1 port and store the received byte to the <data> variable.
  digitalWrite(SPI1_NSS_PIN, HIGH); // manually take CSN high between spi transmissions
}

//VDS Sense Control |= 0x01

//void loop() {
//  digitalWrite(INHA, HIGH);
//  digitalWrite(INLB, HIGH);
//  Serial.println(digitalRead(nFAULT));
//
//}
