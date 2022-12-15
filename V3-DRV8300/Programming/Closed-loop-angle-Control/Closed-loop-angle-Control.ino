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
#define A PA11
#define B PA12
#define Index PA15

#define LED PC13

word data;

void changeSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001001000111000); // 0 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b1000000000000000); // 1 0010 00000000000
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

void SPI_SETUP()
{
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    pinMode(chipSelectPin, OUTPUT);
}

BLDCMotor motor = BLDCMotor(3);
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

    SPI_SETUP();

    _delay(100);
    changeSPI();

    encoder.quadrature = Quadrature::OFF;

    // check if you need internal pullups
    encoder.pullup = Pullup::USE_INTERN;

    // initialize encoder hardware

    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);
    motor.linkSensor(&encoder);

    driver.voltage_power_supply = 12;
    driver.init();

    motor.linkDriver(&driver);

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;

    motor.controller = MotionControlType::torque;

    motor.PID_velocity.P = 0.2;
    motor.PID_velocity.I = 20;
    motor.PID_velocity.D = 0.001;

    motor.LPF_velocity.Tf = 0.01;

    motor.voltage_sensor_align = 0.5;

    Serial3.begin(115200);
    // comment out if not needed
    motor.useMonitoring(Serial3);
    // motor.current_limit = 0.5; // Amps

    // initialize motor
    motor.init();
    // align sensor and start FOC
    motor.initFOC();

    changeSPI();

    _delay(1000);
}

void loop()
{
    motor.loopFOC();
    motor.move();
    // encoder.update();
    // display the angle and the angular velocity to the terminal
    
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

    readRegister(0b1000000000000000);
    if (data != 0)
    {
        Serial3.print("0X00 Error Detected");
        Serial3.print("   ");
        Serial3.println(data, BIN);
        Serial3.println("0X00 Error Detected");
    }

    readRegister(0b1000100000000000);
    if (data != 0)
    {
        Serial3.print("0X01 Error Detected");
        Serial3.print("   ");
        Serial3.println(data, BIN);
        Serial3.println("0X01 Error Detected");
    }
}