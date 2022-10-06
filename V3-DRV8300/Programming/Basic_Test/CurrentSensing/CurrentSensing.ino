#include <SPI.h>

#define INHA PA8
#define INLA PA9
#define SO1 PB1

#define INHB PA10
#define INLB PB6
#define SO2 PB0

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

#define NOCTW PB14
#define NFAULT PB13

#define chipSelectPin PA4
word data;

HardwareSerial Serial3(PA3, PA2);

void changeSPI()
{
  digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b0001000000001000); // 0 0010 00000001000
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
  Serial3.begin(115200);
  delay(1);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(chipSelectPin, OUTPUT);

  pinMode(INHA, OUTPUT);
  pinMode(INLA, INPUT);
  pinMode(SO1, INPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, INPUT);
  pinMode(SO2, INPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, INPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(NOCTW, INPUT);
  pinMode(NFAULT, INPUT);
  pinMode(PB4, INPUT);

  pinMode(LED, OUTPUT);
  digitalWrite(EN_GATE, HIGH);
  digitalWrite(PB4, LOW);

  for (int i = 0; i < 20; i++)
  {
    changeSPI();
    delay(10);
  }
}

void loop()
{
  if (digitalRead(NOCTW) == 0)
  {
    Serial3.println("NOCTW 0");
  }

  if (digitalRead(NFAULT) == 0)
  {
    Serial3.println("NFAULT 0");
  }
  //  changeSPI();
  digitalWrite(INHA, HIGH);

  digitalWrite(INHB, HIGH);
  digitalWrite(INHC, HIGH);

  double I_A = (Current_Calculation(analogRead(SO1) / 4096. * 3.3));
  double I_B = (Current_Calculation(analogRead(SO2) / 4096. * 3.3));
  //  double I_C = -(I_A + I_B);

  Serial3.print(I_A, 6);
  Serial3.print("  ");
  Serial3.println(I_B, 6);
}

double Current_Calculation(double RawVolt)
{
  const double K = 2.;
  const double G = 10.;
  const double VREF = 3.3;
  const double RRSense = 0.5 / 1000.; // 0.5 mOhm

  return ((VREF - (K * RawVolt)) / (G * K));
}
