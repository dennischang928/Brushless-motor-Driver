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

    encoder.quadrature = Quadrature::OFF;
    encoder.pullup = Pullup::USE_INTERN;

    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);

    motor.linkSensor(&encoder);

    Serial3.println("Encoder ready");

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 2;
    driver.init();
    changeSPI();

    motor.linkDriver(&driver);

    // limiting motor current (provided resistance)
    motor.voltage_sensor_align = 0.5;

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.controller = MotionControlType::angle;

    motor.voltage_limit = 1; // Volts - default driver.voltage_limit

    motor.PID_velocity.P = 0.2;
    motor.PID_velocity.I = 0.3;
    motor.PID_velocity.D = 0.0006;
    // jerk control using voltage voltage ramp
    // default value is 300 volts per sec  ~ 0.3V per millisecond
    motor.PID_velocity.output_ramp = 1000;

    // velocity low pass filtering
    // default 5ms - try different values to see what is the best.
    // the lower the less filtered
    motor.LPF_velocity.Tf = 0.3;
    motor.P_angle.P = 16;

    motor.useMonitoring(Serial3);
    motor.monitor_variables = _MON_TARGET | _MON_VOLT_Q | _MON_ANGLE;
    // default _MON_TARGET | _MON_VOLT_Q | _MON_VEL | _MON_ANGLE
    motor.monitor_downsample = 100; // default 10

    motor.init();
    motor.initFOC();

    Serial3.println("Motor ready");
    Serial3.println("----------------------------");
    Serial3.print(motor.zero_electric_angle);
    Serial3.print(", ");
    Serial3.println(motor.sensor_direction == 1 ? "Direction::CW" : "Direction::CCW");
    Serial3.println("----------------------------");

    _delay(1000);
}

long Timer = millis();

int Position_Value = 1;
void loop()
{
    if (millis() - Timer >= 300)
    {
        Position_Value = 0 - Position_Value;
        Timer = millis();
    }

    motor.monitor();
    motor.move(Position_Value);
    motor.loopFOC();
}
