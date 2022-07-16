#include <SimpleFOC.h>

#define INHA PA8
#define INHB PA10
//#define INHC PB7

//#define INHA PA7
//#define INHB PA9
#define INHC PB11

BLDCMotor motor = BLDCMotor(11);

BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC);

float target_velocity = 2;

void setup() {
  pinMode(INHA, OUTPUT);
  pinMode(INHB, OUTPUT);
  pinMode(INHC, OUTPUT);
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
}

void loop() {
  motor.move(target_velocity);
}
