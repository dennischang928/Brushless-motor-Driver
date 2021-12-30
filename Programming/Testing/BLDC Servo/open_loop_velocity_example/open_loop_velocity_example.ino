#include <SimpleFOC.h>

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

BLDCMotor motor = BLDCMotor(6);
BLDCDriver6PWM driver = BLDCDriver6PWM(INHA, INLA, INHB, INLB, INHC, INLC, EN_GATE);
//BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

float target_velocity = 10;

//Commander command = Commander(Serial);
//void doTarget(char* cmd) {
//  command.scalar(&target_velocity, cmd);
//}

//SPI
#define SPI1_NSS_PIN PA4

word receive;
//SPI
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
  pinMode(nFAULT, INPUT);

  driver.voltage_power_supply = 12;
  driver.init();

  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor current (provided resistance)
  motor.current_limit = 20;   // [Amps]
  motor.voltage_limit = 12;

  // open loop control config
  motor.foc_modulation = FOCModulationType::SinePWM;
  motor.controller = MotionControlType::velocity_openloop;
  //
  // init motor hardware
  motor.init();

  digitalWrite(EN_GATE, HIGH);


  sendSPI(0b0100111000100000); //prevent Voltage Drop shut down
  sendSPI(0b0011101010010110); //3 pwm mode
  //  sendSPI(0b0101000000000000);
  //  SPITest();
  //  Serial.print("SPI Test:");
  //  Serial.println(receive, BIN);
  //  delay(3000);
}

void loop() {
  sendSPI(0b0100111000100000); //prevent Voltage Drop shut down
  sendSPI(0b0011101010010110);
//  Serial.println(receive, BIN);
  //  Serial.println("HEllo");
  //  digitalWrite(INHB, HIGH);
  motor.move(target_velocity);
  //  Serial.println(receive, BIN);
  //  Serial.println(digitalRead(nFAULT));
}


void DRV8305_SPI_Setup() {
  SPI.begin(); //Initialize the SPI_1 port.
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1); //Set the  SPI data mode 1
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
  pinMode(SPI1_NSS_PIN, OUTPUT);
}

void sendSPI(word input)
{
  digitalWrite(SPI1_NSS_PIN, LOW);
  receive = SPI.transfer16(input);
  digitalWrite(SPI1_NSS_PIN, HIGH);
}
void SPITest() {
  sendSPI(0b1100100000000000);
}
