#include <SPI.h>

#define INHA PA8
#define INLA PA9

#define INHB PA10
#define INLB PB6

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

#define SPI1_NSS_PIN PA4
word receive;

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
  sendSPI(0b0011101010010110); //3 pwm mode
  Serial.println(receive);

}
void loop() {

  digitalWrite(INHA, HIGH);
  digitalWrite(INHB, HIGH);
  digitalWrite(INHC, HIGH);


  digitalWrite(LED, HIGH);
  delay(1000);
  digitalWrite(INHA, LOW);
  digitalWrite(INHB, LOW);
  digitalWrite(INHC, LOW);
  digitalWrite(LED, LOW);
  delay(1000);
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
