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

LowsideCurrentSense current_sense = LowsideCurrentSense(0.0005, 10, SO1, SO2);

HardwareSerial Serial3(PA3, PA2);

void setup()
{
    // VV================MCU and DRV8303 setup================VV
    Serial3.begin(115200);
    pinMode(EN_GATE, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    digitalWrite(EN_GATE, HIGH);
    delay(1);
    SPI_SETUP();
    delay(10);
    changeSPI();
    // ^^================MCU and DRV8303 setup================^^

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();

    motor.linkDriver(&driver);

    motor.init();
    motor.initFOC();
    Serial3.println("Motor ready");

    // VV================MCU and DRV8303 setup================VV
    digitalWrite(EN_GATE, HIGH);
    changeSPI();
    _delay(1000);
    // ^^================MCU and DRV8303 setup================^^
}

void loop()
{
    driver.setPwm(0, 0, 0.1);
    PhaseCurrent_s currents = current_sense.getPhaseCurrents();

    Serial3.print(currents.a * 1000); // milli Amps
    Serial3.print("\t");
    Serial3.print(currents.b * 1000); // milli Amps
    Serial3.print("\t");
    Serial3.println(currents.c * 1000); // milli Amps
}