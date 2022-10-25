#include <SimpleFOC.h>
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

#define LED PC13

#define chipSelectPin PA4
#define DebugPin PB5
word data;

// Serial setup
HardwareSerial Serial3(PA3, PA2);

// Simple FOC Motor and Driver Setup
BLDCMotor motor = BLDCMotor(6);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

// Simple FOC Current Sense Setup
LowsideCurrentSense current_sense = LowsideCurrentSense(0.00275256, 10, SO1, SO2, _NC);

// Target variable


void changeSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001000000111000); // 0 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

unsigned int readRegister(int Register)
{
    digitalWrite(chipSelectPin, LOW);
    uint16_t dataToSend = 0x0000, dataToRead = 0;
    //   bit   1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16
    //   R=1/W=0  |-Raddr--|  |-----------Data---------------------|
    // Fault=1/0  |-Raddr--|  |-----------Data---------------------|
    dataToSend |= 1;

    dataToSend <<= 4;
    dataToSend |= Register % (16);
    dataToSend <<= 11;

    data = SPI.transfer16(dataToSend);

    digitalWrite(chipSelectPin, HIGH); // manually take CSN high between spi transmissions
}

void ReadStatus(int Register)
{
    digitalWrite(chipSelectPin, LOW);  // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(Register);   // 1 0010 00000001000
    digitalWrite(chipSelectPin, HIGH); // manually take CSN high between spi transmissions
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
    pinMode(PB10, OUTPUT);
    pinMode(INHA, OUTPUT);
    pinMode(INLA, INPUT);
    pinMode(SO1, INPUT);

    pinMode(INHB, OUTPUT);
    pinMode(INLB, INPUT);
    pinMode(SO2, INPUT);

    pinMode(INHC, OUTPUT);
    pinMode(INLC, INPUT);

    pinMode(EN_GATE, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    pinMode(DebugPin, OUTPUT);
    digitalWrite(EN_GATE, HIGH);

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();

    // init C.S.A.

    current_sense.linkDriver(&driver);
    if (current_sense.init())
        Serial3.println("Current sense init success!");
    else
    {
        Serial3.println("Current sense init failed!");
        return;
    }

    current_sense.gain_a *= -1;
    current_sense.gain_b *= -1;
    current_sense.gain_c *= -1;

    current_sense.skip_align = true;

    // init C.S.A.
    // init motor hardware

    motor.linkCurrentSense(&current_sense);

    motor.linkDriver(&driver);

    motor.voltage_limit = 6; // [V]

    motor.foc_modulation = FOCModulationType::SinePWM;

    motor.controller = MotionControlType::velocity_openloop;

    motor.init();
    current_sense.skip_align = true;

    motor.initFOC();
    delay(1);
    SPI_SETUP();
    delay(1);
    changeSPI(); // Setup the registers in the driver

    delay(1000);
}

float target_velocity = 200;

void loop()
{
    changeSPI();
    motor.move(target_velocity);
    // digitalWrite(PB10, LOW);
}

void Debbuger()
{
    if (digitalRead(NFAULT) == LOW)
    {
        Serial3.println("NFAULT Detected");
    }

    if (digitalRead(NOCTW) == LOW)
    {
        Serial3.println("NOCTW Detected");
    }

    ReadStatus(0b1000000000000000);
    if (data != 0)
    {
        Serial3.print("0X01 Error Detected");
        Serial3.print("   ");
        Serial3.println(data, BIN);
    }

    ReadStatus(0b1000100000000000);
    if (data != 0)
    {
        Serial3.print("0X00 Error Detected");
        Serial3.print("   ");
        Serial3.println(data, BIN);
    }
}

double Current_Calculation(double RawVolt)
{
    const double K = 2.;
    const double G = 10.;
    const double VREF = 3.3;
    const double RRSense = 0.5 / 1000.; // 0.05 mOhm

    return ((VREF - (K * RawVolt)) / (G * K));
}