//--------Current Sensing with bldc rotation--------
#include <SimpleFOC.h>

#define INHA PA8
#define INLA PA9
#define SO1 PB1

#define INHB PA10
#define INLB PB6
#define SO2 PB0

#define INHC PB7
#define INLC PB8
#define SO3 PA3

#define nFAULT PB13

#define EN_GATE PB12

#define LED PC13

BLDCMotor motor = BLDCMotor(6);
BLDCDriver6PWM driver = BLDCDriver6PWM(INHA, INLA, INHB, INLB, INHC, INLC, EN_GATE);
float target_velocity = 50;

Commander command = Commander(Serial);
void doTarget(char* cmd) {
  command.scalar(&target_velocity, cmd);
}

//SPI
#define SPI1_NSS_PIN PA4
word receive;
//SPI
void setup() {
  DRV8305_SPI_Setup();
  Serial.begin(115200);
  driver.voltage_power_supply = 12;
  driver.init();

  // link the motor and the driver
  motor.linkDriver(&driver);

  // limiting motor current (provided resistance)
  motor.current_limit = 20;   // [Amps]
  motor.voltage_limit = 12;
  // open loop control config
  motor.foc_modulation = FOCModulationType::SinePWM;
  motor.controller = MotionControlType::velocity_openloop;

  // init motor hardware
  motor.init();

  // add target command T
  command.add('T', doTarget, "target velocity");

  Serial.println("Motor ready!");
  Serial.println("Set target velocity [rad/s]");

  digitalWrite(EN_GATE, HIGH);
  //  sendSPI(0b0100111000100001); //prevent Voltage Drop shut down
  sendSPI(0b0011101010010110);
  sendSPI(0b0101000000000000); // write the current amplifier to be the default value
  sendSPI(0b0101100100001010); // write the current amplifier to be the default value
  Serial.println(receive);

  //  sendSPI(0b01011);
  //new Code
  pinMode(SO1, INPUT);
  pinMode(SO2, INPUT);
  pinMode(SO3, INPUT);
  //  delay(1000);
}

int counter  = 0;

double HAHA[1000];

void loop() {
  //  Serial.print((analogRead(SO1) / 4096. * 3.3));
  //  Serial.print(',');
  //  Serial.print((analogRead(SO2) / 4096. * 3.3));
  //  Serial.print(',');
  //  Serial.println((analogRead(SO3) / 4096. * 3.3));
  //  digitalWrite(INHA, HIGH);
  //  digitalWrite(INHB, LOW);
  //  0.0025756836
  //  delay()
  //  digitalWrite(INHC, LOW);
  //  Serial.println(Current_Calculation(analogRead(SO2) / 4096. * 3.3));
  //  Serial.print(',');
  //  Serial.println(Current_Calculation(1.5));
  //  Serial.print(' ');
  //  Serial.print((analogRead(SO1) / 4096. * 3.3), 10);
  //  Serial.print(',');
  //  Serial.print(Current_Calculation(analogRead(SO1) / 4096. * 3.3));
  //  Serial.print(',');


  HAHA[counter] = (Current_Calculation(analogRead(SO3) / 4096. * 3.3) / (5 / 1000.));
  if (counter == 999) {
    counter = 0;
    double Average_Accumulated = 0;
    for (int i = 0; i < 1000; i++) {
      Average_Accumulated = Average_Accumulated + HAHA[i];
    }
    Serial.println(Average_Accumulated / 100);
    Serial.println("--------------------------------------------------------");
  }
  counter ++;
}


void DRV8305_SPI_Setup() {
  SPI.begin(); //Initialize the SPI_1 port.
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1); //Set the  SPI data mode 1
  SPI.setClockDivider(SPI_CLOCK_DIV16); // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
  pinMode(SPI1_NSS_PIN, OUTPUT);
}

void sendSPI(word input)
{
  digitalWrite(SPI1_NSS_PIN, LOW);
  receive = SPI.transfer16(input);
  digitalWrite(SPI1_NSS_PIN, HIGH);
}

double Current_Calculation(double RawVolt) {
  const double K = 2.;
  const double G = 10.;
  const double VREF = 5.;

  //  return (VREF - (K * RawVolt)) / (G * K); //general solve
  // y = 1.0181RawVolt - 0.0369
  return (1.0181 * RawVolt - 0.0369); //stupid hard solve (mou's Way)
}
