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

BLDCMotor motor = BLDCMotor(7);
BLDCDriver3PWM driver = BLDCDriver3PWM(INHA, INHB, INHC, EN_GATE);

LowsideCurrentSense current_sense = LowsideCurrentSense(0.007, 20, SO2, SO1, _NC);

Encoder encoder = Encoder(A, B, 1000, Index);

void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }
HardwareSerial Serial3(PA3, PA2);

MovingAverage CurrentAmpliferA(1);
MovingAverage CurrentAmpliferB(1);

void setup()
{
    //================MCU and DRV8303 setup Below================
    Serial3.begin(115200);
    pinMode(EN_GATE, OUTPUT);
    pinMode(NFAULT, INPUT);
    pinMode(NOCTW, INPUT);
    pinMode(DC_CAL, OUTPUT);

    digitalWrite(EN_GATE, HIGH);

    delay(1);
    SPI_SETUP();
    delay(10);
    changeSPIGAIN2_20();
    changeSPI();
    //================MCU and DRV8303 setup Above================

    //================SimpleFOC setup Below================
    // encoder.quadrature = Quadrature::ON;
    encoder.init();
    encoder.enableInterrupts(doA, doB, doIndex);

    motor.linkSensor(&encoder);

    Serial3.println("Encoder ready");

    driver.voltage_power_supply = 12;
    driver.voltage_limit = 2;
    driver.init();

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

    // limiting motor current (provided resistance)
    motor.voltage_sensor_align = 0.1;

    motor.foc_modulation = FOCModulationType::SpaceVectorPWM;
    motor.torque_controller = TorqueControlType::foc_current;
    motor.controller = MotionControlType::torque;

    //======================P axis and D axis PID below======================
    motor.PID_current_q.P = 1.7;
    motor.PID_current_q.I = 80;
    motor.PID_current_q.D = 0.0005;

    motor.PID_current_d.P = 0.0010;
    motor.PID_current_d.I = 0.02;
    motor.PID_current_d.D = 0.003;

    motor.LPF_current_q.Tf = 0.02;
    motor.LPF_current_d.Tf = 0.03;

    motor.useMonitoring(Serial3);
    motor.monitor_variables = _MON_CURR_Q | _MON_CURR_D;

    motor.monitor_downsample = 1; // default 10
    motor.init();
    motor.initFOC();
    Serial3.println("Motor ready");
    //================SimpleFOC setup Above================
}

void loop()
{
    motor.loopFOC();
    motor.move(1);
    motor.monitor();
    // Serial3.print("\t");
    // GetAndPrintCurrentValue();
    // GetAndPrint_FOC_CurrentValue(motor.electricalAngle());
    // Serial3.println("");
}

void GetAndPrintCurrentValue()
{
    PhaseCurrent_s currents = current_sense.getPhaseCurrents();

    Serial3.print(currents.a, 6); // milli Amps
    Serial3.print("\t");
    Serial3.print(currents.b, 6); // milli Amps
    Serial3.print("\t");
}

void GetAndPrint_FOC_CurrentValue(float angle_el)
{
    DQCurrent_s currents = current_sense.getFOCCurrents(angle_el);
    // Serial3.print("ElectricalAngle:");
    // Serial3.print(angle_el, 6); // milli Amps
    // Serial3.pr   int("\t");
    // Serial3.print();          // milli Amps
    Serial3.print(currents.q, 6); // milli Amps
    Serial3.print("\t");
    // Serial3.print("Q:");            // milli Amps
    Serial3.print(currents.d, 6); // milli Amps
    Serial3.print("\t");
}

// void GetAndPrint_AnalogReadCurrentValue()
// {
//     Serial3.print("A:");
//     Serial3.print(GetShuntResistorVoltage(CurrentAmpliferA.addSample((analogRead(SO1) / 1023. * 3.3) + offsetA)) / 0.05E-3, 6);
//     Serial3.print(",B:");
//     Serial3.println(GetShuntResistorVoltage(CurrentAmpliferB.addSample((analogRead(SO2) / 1023. * 3.3) + offsetB)) / 0.05E-3, 6);
// }

double GetShuntResistorVoltage(double RawVolt)
{
    const double G = 80.;
    const double VREF = 3.3;
    return ((VREF / 2.) - RawVolt) / G; // general solve
}