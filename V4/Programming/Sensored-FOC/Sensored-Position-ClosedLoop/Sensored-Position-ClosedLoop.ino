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

float Velocity = 0;

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);
z
LowsideCurrentSense current_sense = LowsideCurrentSense(0.00658395332, 20, SO2, SO1, _NC);

Encoder encoder = Encoder(A, B, 1000, Index);

void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }
HardwareSerial Serial3(PA3, PA2);

// PIDController PIDv{0.01, 0, 0.000, 400, 12};
// LowPassFilter LPFv{0.04};

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
    changeSPIGAIN2_20();
    changeSPI();

    encoder.quadrature = Quadrature::OFF;
    encoder.pullup = Pullup::USE_INTERN;

    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);

    motor.linkSensor(&encoder);

    Serial3.println("Encoder ready");

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 3;
    driver.init();
    changeSPI();

    motor.linkDriver(&driver);
    //-----------------Current Sensor Setup Below-----------------
    pinMode(SO1, INPUT);
    pinMode(SO2, INPUT);
    digitalWrite(DC_CAL, LOW);

    current_sense.skip_align = true;

    current_sense.linkDriver(&driver);

    current_sense.init();

    motor.linkCurrentSense(&current_sense);
    //-----------------Current Sensor Setup Above-----------------

    motor.voltage_sensor_align = 0.1;

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.controller = MotionControlType::velocity;

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~Velocity PID Below~~~~~~~~~~~~~~~~~~~~~~~~~~~
    motor.PID_velocity.P = 0.2;
    motor.PID_velocity.I = 0;
    motor.PID_velocity.D = 0;

    motor.PID_velocity.output_ramp = 200;
    motor.LPF_velocity.Tf = 0.04;
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~Velocity PID Above~~~~~~~~~~~~~~~~~~~~~~~~~~~

    //======================P axis and D axis PID Below======================
    motor.PID_current_q.P = 1.7;
    motor.PID_current_q.I = 80;
    motor.PID_current_q.D = 0.0005;

    motor.PID_current_d.P = 0.0010;
    motor.PID_current_d.I = 0.02;
    motor.PID_current_d.D = 0.003;

    motor.LPF_current_q.Tf = 0.02;
    motor.LPF_current_d.Tf = 0.03;
    //======================P axis and D axis PID Above======================

    motor.useMonitoring(Serial3);
    motor.monitor_variables = _MON_TARGET | _MON_VOLT_Q | _MON_VEL;

    motor.monitor_downsample = 100; // default 10

    motor.init();
    motor.initFOC();

    Serial3.println("Motor ready");
    Serial3.println("----------------------------");
    Serial3.print(motor.zero_electric_angle);
    Serial3.print(", ");
    Serial3.println(motor.sensor_direction == 1 ? "Direction::CW" : "Direction::CCW");
    Serial3.println("----------------------------");

    delay(1000);
}

long counter = millis();
int i = 0;

float target_angle = 20;
// timestamp for changing direction
long timestamp_us = millis();

void loop()
{

    // each one second
    if (millis() - timestamp_us > 3000)
    {
        timestamp_us = millis();
        // inverse angle
        target_angle = -target_angle;
    }

    // main FOC algorithm function
    motor.loopFOC();
    motor.monitor();

    // Motion control function
    motor.move(target_angle);
}