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
#define DC_CAL PB4

#define LED PC13

#define chipSelectPin PA4
#define A PA11
#define B PA12
#define Index PA15

#define LED PC13

word data;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

LowsideCurrentSense current_sense = LowsideCurrentSense(0.001, 80, SO1, SO2, _NC);

HardwareSerial Serial3(PA3, PA2);

Encoder encoder = Encoder(A, B, 1000, PA15);
// interrupt routine initialization
void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }

// add communication
void setup()
{
    Serial3.begin(115200);
    pinMode(EN_GATE, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    encoder.quadrature = Quadrature::ON;
    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);

    motor.linkSensor(&encoder);

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();
    motor.linkDriver(&driver);

    //=========New code Below========
    pinMode(DC_CAL, OUTPUT);

    digitalWrite(EN_GATE, HIGH);
    digitalWrite(DC_CAL, LOW);

    pinMode(SO1, INPUT);
    pinMode(SO2, INPUT);

    current_sense.linkDriver(&driver);

    current_sense.init();

    current_sense.gain_a = 1. / 80. / 0.01;
    current_sense.gain_b = 1. / 80. / 0.015;
    //=========New code Above========

    //===================Encoder Code Above===================

    digitalWrite(EN_GATE, HIGH);
    delay(1);
    SPI_SETUP();
    delay(10);
    changeSPI3PWM();
    changeSPIGAIN2_80();

    motor.linkDriver(&driver);

    motor.voltage_limit = 1;

    motor.foc_modulation = FOCModulationType::SinePWM;
    motor.controller = MotionControlType::velocity_openloop;

    motor.init();
    motor.initFOC();

    Serial3.println("Encoder ready");
    _delay(1000);
}
int Timer = millis();

float i = 1;

void loop()
{
    motor.move(10);
    GetAndPrint_FOC_CurrentValue(motor.electricalAngle());
    // GetAndPrintCurrentValue(); // For serial plotter, Blue curve is A, Red curve is B
}
