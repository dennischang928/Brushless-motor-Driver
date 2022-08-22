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

void changeSPI3PWM()
{
  digitalWrite(chipSelectPin, LOW); // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b0001000000001000); //0 0010 00000111000
  digitalWrite(chipSelectPin, HIGH);
}

void changeCurrentSense() {
  digitalWrite(chipSelectPin, LOW);
  data = SPI.transfer16(0b000110000000000); //0 0011 0000000000
  digitalWrite(chipSelectPin, HIGH);
}

void setup() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(chipSelectPin, OUTPUT);
  Serial.begin(115200);
  pinMode(INHA, OUTPUT);
  pinMode(INLA, INPUT);
  pinMode(SO1, INPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, INPUT);
  pinMode(SO2, INPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, INPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(PB4, OUTPUT);

  pinMode(NOCTW, INPUT);
  pinMode(NFAULT, INPUT);

  pinMode(LED, OUTPUT);

  digitalWrite(PB4, LOW); //PB4 is the DC_CAL PIN
  digitalWrite(EN_GATE, HIGH);


  changeSPI3PWM();
  changeCurrentSense();
}



void loop() {
  digitalWrite(INHB, HIGH);
  digitalWrite(INHA, HIGH);
  digitalWrite(INHC, HIGH);

  if (digitalRead(NFAULT) == LOW) {
    Serial.println("NFault low");
  }

  if (digitalRead(NOCTW) == LOW) {
    Serial.println("NOCTW low");
  }

  Serial.print(Current_Calculation(analogRead(SO1) / 4096. *3.3), 10);
  Serial.print("    ");
  Serial.println(Current_Calculation(analogRead(SO2) / 4096. *3.3), 10);
}

double Current_Calculation(double RawVolt) {
  const double K = 2. ;
  const double G = 80.;
  const double VREF = 3.3;
  return (VREF - (K * RawVolt)) / (G * K); //general solve
}
