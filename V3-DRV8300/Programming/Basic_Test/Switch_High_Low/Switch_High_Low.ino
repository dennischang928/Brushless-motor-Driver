#include <SPI.h>

#define INHA PA8
#define INLA PA9

#define INHB PA10
#define INLB PB6

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

#define NOCTW PB14
#define NFAULT PB13

#define chipSelectPin PA4

void setup()
{
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(chipSelectPin, OUTPUT);
  Serial.begin(115200);
  pinMode(INHA, OUTPUT);
  pinMode(INLA, OUTPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, OUTPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, OUTPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(NOCTW, INPUT);
  pinMode(NFAULT, INPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);
}

void loop()
{
  if (digitalRead(NFAULT) == 0) {
    Serial.println("NFAULT!!!!");
  }
  analogWrite(INHA, 126);
  analogWrite(INHB, 126);
  analogWrite(INHC, 126);
}
