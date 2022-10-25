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
HardwareSerial Serial3(PA3, PA2);

void changeSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001000000111000); // 0 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b1000000000000000); // 1 0010 00000000000
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

BLDCMotor motor = BLDCMotor(7, 0.461);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

Encoder encoder = Encoder(A, B, 1000);
void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }

void setup()
{
    Serial3.begin(115200);

    pinMode(INHA, OUTPUT);
    pinMode(INLA, INPUT);
    pinMode(SO1, INPUT);

    pinMode(INHB, OUTPUT);
    pinMode(INLB, INPUT);
    pinMode(SO2, INPUT);

    pinMode(INHC, OUTPUT);
    pinMode(INLC, INPUT);
    // pinMode(SO3, INPUT);

    pinMode(EN_GATE, OUTPUT);
    pinMode(LED, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    delay(1);
    digitalWrite(EN_GATE, HIGH);

    SPI_SETUP();

    _delay(100);

    for (int i = 0; i < 20; i++)
    {
        changeSPI();
        delay(10);
    }

    encoder.quadrature = Quadrature::ON;

    // check if you need internal pullups
    encoder.pullup = Pullup::USE_INTERN;

    // initialize encoder hardware
    encoder.init();
    encoder.enableInterrupts(doA, doB);
    motor.linkSensor(&encoder);

    Serial3.println("Encoder ready");

    driver.voltage_power_supply = 12;
    driver.init();

    motor.linkDriver(&driver);

    // limiting motor current (provided resistance)
    motor.current_limit = 20; // [Amps]
    motor.voltage_limit = 6;

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::torque;

    motor.init();
    // align sensor and start FOC
    motor.initFOC();

    // set the initial motor target
    motor.target = 1; // Amps - if phase resistance defined
    // motor.target = 1; // Volts

    Serial3.println("Motor ready");
    _delay(1000);
}

void loop()
{
    // main FOC algorithm function
    changeSPI();
    motor.loopFOC();

    // Motion control function

    motor.move();

    // driver.setPwm(0, 0, 1);
    // user communication
    // command.run();
}
