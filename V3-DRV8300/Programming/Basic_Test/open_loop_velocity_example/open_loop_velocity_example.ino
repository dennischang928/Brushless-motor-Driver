#include <SimpleFOC.h>
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

#define EN_GATE PB12

#define LED PC13

#define chipSelectPin PA4
word data;

void changeSPI()
{
  digitalWrite(chipSelectPin, LOW); // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b0001000000111000); //0 0010 00000001000
  digitalWrite(chipSelectPin, HIGH); // manually take CSN high between spi transmissions
}

void ReadSPI () {
  digitalWrite(chipSelectPin, LOW); // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b1001000000000000); //1 0010 00000000000
  digitalWrite(chipSelectPin, HIGH); // manually take CSN high between spi transmissions
}

void ReadStatus()
{
  digitalWrite(chipSelectPin, LOW); // manually take CSN low for SPI_1 transmission
  data = SPI.transfer16(0b1000000000000000); //1 0010 00000001000
  digitalWrite(chipSelectPin, HIGH); // manually take CSN high between spi transmissions
}

void SPI_SETUP() {
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);
  SPI.setClockDivider(SPI_CLOCK_DIV16);
  pinMode(chipSelectPin, OUTPUT);
}

BLDCMotor motor = BLDCMotor(6);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);
// target variable
float target_velocity = -50;

void setup()
{
  Serial.begin(115200);
  SPI_SETUP();

  pinMode(INHA, OUTPUT);
  pinMode(INLA, INPUT);
  pinMode(SO1, INPUT);

  pinMode(INHB, OUTPUT);
  pinMode(INLB, INPUT);
  pinMode(SO2, INPUT);

  pinMode(INHC, OUTPUT);
  pinMode(INLC, INPUT);
  pinMode(SO3, INPUT);

  pinMode(EN_GATE, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(nFAULT, INPUT);


  driver.voltage_power_supply = 12;
  driver.init();

  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor current (provided resistance)
  motor.current_limit = 20;   // [Amps]
  motor.voltage_limit = 6;

  // open loop control config
  motor.foc_modulation = FOCModulationType::SinePWM;
  motor.controller = MotionControlType::velocity_openloop;
  //
  // init motor hardware
  motor.init();

  digitalWrite(EN_GATE, HIGH);

  Serial.println("Motor ready!");
  delay(1000);
  changeSPI();
}

void loop()
{
  motor.move(target_velocity);
}
double Current_Calculation(double RawVolt) {
  const double K = 2.;
  const double G = 10.;
  const double VREF = 3.;

  return (VREF - (K * RawVolt)) / (G * K); //general solve
  // y = 1.0181RawVolt - 0.0369
  //  return (1.0181 * RawVolt - 0.0369); //stupid hard solve (mou's Way)
}
