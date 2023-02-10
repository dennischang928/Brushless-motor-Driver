#include <SimpleFOC.h>

#define A PA11
#define B PA12
#define Index PA15

Encoder encoder = Encoder(A, B, 1000, PA15);
// interrupt routine initialization
void doA() { encoder.handleA(); }
void doB() { encoder.handleB(); }
void doIndex() { encoder.handleIndex(); }

HardwareSerial Serial3(PA3, PA2);

void setup()
{
    Serial3.begin(115200);
    encoder.quadrature = Quadrature::ON;
    // initialize encoder hardware
    encoder.init();
    // hardware interrupt enable
    encoder.enableInterrupts(doA, doB, doIndex);

    Serial3.println("Encoder ready");
    _delay(1000);
}

void loop()
{
    // IMPORTANT - call as frequently as possible
    // update the sensor values
    encoder.update();
    // display the angle and the angular velocity to the terminal
    Serial3.println(encoder.getAngle());
}