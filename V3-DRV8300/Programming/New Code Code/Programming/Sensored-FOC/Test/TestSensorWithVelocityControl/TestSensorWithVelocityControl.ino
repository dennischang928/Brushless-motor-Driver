#include <SimpleFOC.h>
#include <SPI.h>
#include <MovingAverage.h>

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

MovingAverage CurrentAmpliferA(2);
MovingAverage CurrentAmpliferB(2);

Encoder encoder = Encoder(A, B, 1000, Index);

void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }
HardwareSerial Serial3(PA3, PA2);
// CD0.01
//  CD0.03
PIDController PIDv{0.05, 0.3, 0.000, 400, 12};
// motor.PID_velocity.P = 0.05;
// motor.PID_velocity.I = 0.3;
// motor.PID_velocity.D = 0.0000;
// CP0.01

LowPassFilter LPFv{0.04};

// motor.PID_velocity.P = 0.05;
// motor.PID_velocity.I = 0.3;
// motor.PID_velocity.D = 0.0000;

// add communication
Commander commands = Commander(Serial3);
void doController(char *cmd) { commands.pid(&PIDv, cmd); }
void doFilter(char *cmd) { commands.lpf(&LPFv, cmd); }
void doTarget(char *cmd) { commands.scalar(&Velocity, cmd); }

void setup()
{
    Serial3.begin(115200);
    pinMode(EN_GATE, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);

    //=========New code Below========
    pinMode(DC_CAL, OUTPUT);

    digitalWrite(EN_GATE, HIGH);
    digitalWrite(DC_CAL, LOW);

    pinMode(SO1, INPUT);
    pinMode(SO2, INPUT);
    //=========New code Above========

    digitalWrite(EN_GATE, HIGH);
    delay(1);
    SPI_SETUP();
    delay(10);
    changeSPI3PWM();
    changeSPIGAIN2_80();

    encoder.quadrature = Quadrature::OFF;
    encoder.pullup = Pullup::USE_INTERN;

    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);

    motor.linkSensor(&encoder);

    Serial3.println("Encoder ready");

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 12;
    driver.init();
    changeSPI3PWM();
    changeSPIGAIN2_80();

    motor.linkDriver(&driver);

    // limiting motor current (provided resistance)
    motor.voltage_sensor_align = 0.1;

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::voltage;
    motor.controller = MotionControlType::torque;

    motor.voltage_limit = 12; // Volts - default driver.voltage_limit

    // motor.PID_velocity.P = 0.05;
    // motor.PID_velocity.I = 0.3;
    // motor.PID_velocity.D = 0.0000;
    // // jerk control using voltage voltage ramp
    // // default value is 300 volts per sec  ~ 0.3V per millisecond
    // motor.PID_velocity.output_ramp = 400;

    // // velocity low pass filtering
    // // default 5ms - try different values to see what is the best.
    // // the lower the less filtered
    // motor.LPF_velocity.Tf = 0.03;

    motor.useMonitoring(Serial3);
    motor.monitor_variables = _MON_TARGET | _MON_VOLT_Q | _MON_VEL;
    // default _MON_TARGET | _MON_VOLT_Q | _MON_VEL | _MON_ANGLE
    motor.monitor_downsample = 100; // default 10

    motor.init();
    motor.initFOC();

    commands.add('C', doController, "tune velocity pid");
    commands.add('L', doFilter, "tune velocity LPF");
    commands.add('T', doTarget, "motor target");

    Serial3.println("Motor ready");
    Serial3.println("----------------------------");
    Serial3.print(motor.zero_electric_angle);
    Serial3.print(", ");
    Serial3.println(motor.sensor_direction == 1 ? "Direction::CW" : "Direction::CCW");
    Serial3.println("----------------------------");

    _delay(1000);
}

long counter = millis();
int i = 0;

void loop()
{
    // motor.monitor();
    float target_voltage = PIDv(Velocity - LPFv(motor.shaft_velocity));
    motor.move(target_voltage);
    motor.loopFOC();

    Serial3.print("A: ");
    Serial3.print(GetShuntResistorVoltage(CurrentAmpliferA.addSample((analogRead(SO1) / 4096. * 3.3))) * 1000000., 6); // microvolt
    Serial3.print(" B: ");
    Serial3.println(GetShuntResistorVoltage(CurrentAmpliferB.addSample((analogRead(SO2) / 4096. * 3.3))) * 1000000., 6); // microvolt

    commands.run();
    //This program is so stupid, moving the motor, measuring the current while not aligning the measuring time and pwm time
    //The only purpose of this program is to check if the current value responds to the motor moving velocity
}

double GetShuntResistorVoltage(double RawVolt)
{
    const double G = 80.;
    const double VREF = 3.3;
    return ((VREF / 2.) - RawVolt) / G; // general solve
}