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

#define RshuntA 0.00654041823
#define RshuntB 0.00638065488

word data;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

Encoder encoder = Encoder(A, B, 1000, Index);

void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }

LowsideCurrentSense current_sense = LowsideCurrentSense(1, 20, SO1, SO2, _NC);

HardwareSerial Serial3(PA3, PA2);

void setup()
{
    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);
    motor.linkSensor(&encoder);
    // VV================MCU and DRV8303 setup================VV
    Serial3.begin(115200);
    pinMode(EN_GATE, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);
    pinMode(DC_CAL, OUTPUT);

    digitalWrite(EN_GATE, HIGH);
    digitalWrite(DC_CAL, LOW);

    delay(1);
    SPI_SETUP();
    delay(10);
    changeSPI();
    delay(1);
    changeSPIGAIN2_20();
    // ^^================MCU and DRV8303 setup================^^

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();

    current_sense.gain_a = 1.0 / RshuntA / 20;
    current_sense.gain_b = 1.0 / RshuntB / 20;
    current_sense.linkDriver(&driver);

    motor.linkDriver(&driver);

    motor.init();
    current_sense.init();
    motor.linkCurrentSense(&current_sense);

    // motor.initFOC();
    Serial3.println("Motor ready");

    // VV================MCU and DRV8303 setup================VV
    digitalWrite(EN_GATE, HIGH);
    changeSPI();
    _delay(1000);
    // ^^================MCU and DRV8303 setup================^^
}

int Timer = millis();

float i = 1;

void loop()
{
    // if (millis() - Timer >= 300)
    // {
    //     if (i == 1)
    //     {
    //         i = 0;
    //     }
    //     else
    //     {
    //         i = 1;
    //     }
    //     Timer = millis();
    // }

    // driver.setPwm(0, 0, 0);
    // PhaseCurrent_s currents = current_sense.getPhaseCurrents();

    // Serial3.print(currents.a, 6); // milli Amps
    // Serial3.print("\t");
    // Serial3.print(currents.b, 6); // milli Amps
    // Serial3.print("\t");
    // Serial3.println(-currents.a - currents.b, 6); // milli Amps
    Serial3.println(motor.electricalAngle());
}

float Amplified_Volt2Input_Volt(float Amplified_Volt)
{
    const float K = 2.;
    const float G = 10.;
    const float VREF = 3.3;

    return float((VREF - (K * Amplified_Volt)) / (G * K));
}

float Amplified_Volt2Current(float Amplified_Volt)
{
    const float RRSense = 0.00275256; // 0.5 mOhm
    return float(Amplified_Volt2Input_Volt(Amplified_Volt) / RRSense);
}