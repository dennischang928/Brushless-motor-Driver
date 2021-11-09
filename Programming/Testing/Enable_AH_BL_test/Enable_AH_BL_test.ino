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

#define LED PC13

#define EN_GATE PB12

//SPI
#define SPI1_NSS_PIN PA4
word receive;

void setup() {
  DRV8305_SPI_Setup();
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

  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);

  digitalWrite(EN_GATE, HIGH);
  sendSPI(0b0100101001000000);

}


void loop() {
  digitalWrite(INHC, LOW);
  digitalWrite(INLA, LOW);

  digitalWrite(INHA, HIGH);
  digitalWrite(INLB, HIGH);
  delay(10);
  digitalWrite(INLB, LOW);
  digitalWrite(INHA, HIGH);
  digitalWrite(INLC, HIGH);
  delay(10);
  digitalWrite(INHA, LOW);
  digitalWrite(INHB, HIGH);
  digitalWrite(INLC, HIGH);
  delay(10);
  digitalWrite(INHB, LOW);
  digitalWrite(INLC, LOW);
  digitalWrite(INLA, HIGH);
  digitalWrite(INHB, HIGH);
  delay(10);
  digitalWrite(INHB, LOW);
  digitalWrite(INHC, HIGH);
  digitalWrite(INLA, HIGH);
  delay(10);
  digitalWrite(INLA, LOW);
  digitalWrite(INHC, HIGH);
  digitalWrite(INLA, HIGH);
  delay(10);  
}

void DRV8305_SPI_Setup() {
  SPI.begin(); //Initialize the SPI_1 port.
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1); //Set the  SPI data mode 1
  SPI.setClockDivider(SPI_CLOCK_DIV16);      // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
  pinMode(SPI1_NSS_PIN, OUTPUT);
}

void sendSPI(word input)
{
  digitalWrite(SPI1_NSS_PIN, LOW);
  receive = SPI.transfer16(input);
  digitalWrite(SPI1_NSS_PIN, HIGH);
}
