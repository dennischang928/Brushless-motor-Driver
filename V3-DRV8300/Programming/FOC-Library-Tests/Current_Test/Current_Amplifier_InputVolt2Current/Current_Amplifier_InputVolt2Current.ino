#include <SPI.h>

#define INHA PA8
#define INLA PA9
#define SO1 PB1

#define INHB PA10
#define INLB PB6
#define SO2 PB0

#define INHC PB7
#define INLC PB8
#define LED PC13

#define EN_GATE PB12

#define NOCTW PB14
#define NFAULT PB13

#define chipSelectPin PA4
word data;

HardwareSerial Serial3(PA3, PA2);

void Three_PWM_Mode_Setup()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001000000001000); // 0 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadStatus()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b1000000000000000); // 1 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void ReadSPI()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b1001000000000000); // 1 0010 00000000000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void setup()
{
    Serial3.begin(115200);
    delay(1);
    SPI.begin();
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE1);
    SPI.setClockDivider(SPI_CLOCK_DIV16);
    pinMode(chipSelectPin, OUTPUT);

    pinMode(INHA, OUTPUT);
    pinMode(INLA, INPUT);

    pinMode(INHB, OUTPUT);
    pinMode(INLB, INPUT);

    pinMode(INHC, OUTPUT);
    pinMode(INLC, INPUT);

    pinMode(EN_GATE, OUTPUT);
    pinMode(NOCTW, INPUT);
    pinMode(NFAULT, INPUT);

    pinMode(LED, OUTPUT);
    digitalWrite(EN_GATE, HIGH);
    delay(1);

    //====================================DRV830X_Mode_Setup====================================
    for (int i = 0; i < 20; i++)
    {
        Three_PWM_Mode_Setup();
        delay(10);
    }
    //====================================^DRV830X_Mode_Setup^====================================
}

void loop()
{

    while (true)
    {
        digitalWrite(INHA, LOW);
        analogWrite(INHB, 255);
        digitalWrite(INHC, LOW);

        Serial3.println(Amplified_Volt2Current(analogRead(SO1) / 1023. * 3.3), 6); // be careful of the resolution. In the case, the analogRead() Function resolution is 1023
        // Serial3.print("   ");
        // Serial3.println(Amplified_Volt2Input_Volt(analogRead(SO2) / 1023. * 3.3), 6);
    }
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