#include <SPI.h>

#define INHA PA8
#define INLA PB6

#define INHB PA9
#define INLB PB7

#define INHC PA10
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

#define NOCTW PB14
#define NFAULT PB13

#define chipSelectPin PA4
word data;

void changeSPI()
{
  digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b0001000000111000); // 0 0010 00000001000
  digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadStatus()
{
  digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b1000000000000000); // 1 0010 00000001000
  digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadSPI()
{
  digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b1001000000000000); // 1 0010 00000000000
  digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void setup()
{
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(chipSelectPin, OUTPUT);
  Serial.begin(115200);
  pinMode(INHA, OUTPUT);
  pinMode(INLA, INPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, INPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, INPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(NOCTW, INPUT);
  pinMode(NFAULT, INPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);

  for (int i = 0; i < 20; i++)
  {
    changeSPI();
    delay(10);
  }
}

void loop()
{
  delay(5000);

  ReadStatus();
  if (digitalRead(NFAULT) == 0)
  {
    Serial.println(data, BIN);
    Serial.println("NFAULT!!!!!!");
  }
  if (digitalRead(NOCTW) == 0)
  {
    Serial.println(data, BIN);
    Serial.println("NOCTW!!!!!!");
  }

  analogWrite(INHC, 0.5);
  analogWrite(INHB, 100);
  analogWrite(INHA, 0);
}
