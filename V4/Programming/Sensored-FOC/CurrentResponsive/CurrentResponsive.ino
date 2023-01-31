#include <SimpleFOC.h>
#include <MovingAverage.h>

#include <SPI.h>

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
word data;

BLDCMotor motor = BLDCMotor(6);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

HardwareSerial Serial3(PA3, PA2);

MovingAverage CurrentAmpliferA(40);
MovingAverage CurrentAmpliferB(40);

// target variable
float target_velocity = -70;

void changeSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001000000001000); // 0 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b1001000000000000); // 1 0010 00000000000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadStatus()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b1000000000000000); // 1 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void SPI_SETUP()
{
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    pinMode(chipSelectPin, OUTPUT);
}

void setup()
{
    Serial3.begin(115200);

    pinMode(INHA, OUTPUT);
    pinMode(INLA, OUTPUT);
    pinMode(SO1, INPUT);

    pinMode(INHB, OUTPUT);
    pinMode(INLB, OUTPUT);
    pinMode(SO2, INPUT);

    pinMode(INHC, OUTPUT);
    pinMode(INLC, OUTPUT);

    pinMode(EN_GATE, OUTPUT);
    pinMode(DC_CAL, OUTPUT);

    pinMode(LED, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    digitalWrite(DC_CAL, LOW);

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();

    motor.linkDriver(&driver);

    motor.voltage_limit = 12; // [V]

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.controller = MotionControlType::velocity_openloop;

    // init motor hardware
    motor.init();

    SPI_SETUP();

    delay(1000);
    changeSPI();
    analogReadResolution(12);
}

int Timer = millis();

float i = 1;

void loop()
{
    if (millis() - Timer >= 300)
    {
        if (i == 1)
        {
            i = 0;
        }
        else
        {
            i = 1;
        }
        Timer = millis();
    }

    driver.setPwm(0, i, 0);
    Serial3.print("A: ");
    Serial3.println(GetShuntResistorVoltage(CurrentAmpliferA.addSample((analogRead(SO1) / 4096. * 3.3))) / 3E-5, 6);
    // Serial3.print(" B: ");
    // Serial3.println(GetShuntResistorVoltage(CurrentAmpliferB.addSample((analogRead(SO2) / 4096. * 3.3))) / 3E-5, 6);
}

double GetShuntResistorVoltage(double RawVolt)
{
    const double G = 10.;
    const double VREF = 3.3;
    return ((VREF / 2.) - RawVolt) / G; // general solve
}