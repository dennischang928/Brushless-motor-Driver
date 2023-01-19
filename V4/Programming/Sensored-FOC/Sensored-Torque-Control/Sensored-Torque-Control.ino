#include <SimpleFOC.h>
#include <SPI.h>
#include <string>

#define INHA PA8
#define INLA PB6
#define SO1 PA1

#define INHB PA9
#define INLB PB7
#define SO2 PA0

#define INHC PA10
#define INLC PB8

#define NFAULT PB13
#define NOCTW PB14

#define EN_GATE PB12

#define LED PC13

#define chipSelectPin PA4
#define A PA11
#define B PA12
#define Index PA15

#define LED PC13

word data;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

Encoder encoder = Encoder(A, B, 1000, Index);

void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }
HardwareSerial Serial3(PA3, PA2);

void setup()
{
    Serial3.begin(115200);
    pinMode(EN_GATE, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    digitalWrite(EN_GATE, HIGH);
    delay(1);
    SPI_SETUP();
    delay(10);
    changeSPI();

    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);

    motor.linkSensor(&encoder);

    Serial3.println("Encoder ready");

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();

    motor.linkDriver(&driver);

    // limiting motor current (provided resistance)
    motor.voltage_sensor_align = 0.5;

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::torque;

    motor.useMonitoring(Serial3);
    motor.monitor_variables = _MON_TARGET | _MON_VEL | _MON_VOLT_Q | _MON_VOLT_D;

    motor.monitor_downsample = 100; // default 10

    motor.init();
    motor.initFOC();
    Serial3.println("Motor ready");
    changeSPI();
    _delay(1000);
}

float i = 0;

void loop()
{
    i += 0.0001;
    motor.monitor();
    motor.loopFOC();
    motor.move(constrain(i, 0, 1));
    // Serial3.println(Angle);
}