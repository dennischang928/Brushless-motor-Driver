
void GetAndPrintCurrentValue()
{
    PhaseCurrent_s currents = current_sense.getPhaseCurrents();

    Serial3.print(currents.a, 6); // milli Amps
    Serial3.print("\t");
    Serial3.println(currents.b, 6); // milli Amps
}

void GetAndPrint_FOC_CurrentValue(float angle_el)
{
    DQCurrent_s currents = current_sense.getFOCCurrents(angle_el);
    // Serial3.print("ElectricalAngle:");
    // Serial3.print(angle_el, 6); // milli Amps
    // Serial3.print("\t");
    Serial3.print("D:");          // milli Amps
    Serial3.print(currents.d, 6); // milli Amps
    Serial3.print("\t");
    Serial3.print("Q:");            // milli Amps
    Serial3.println(currents.q, 6); // milli Amps
}

double GetShuntResistorVoltage(double RawVolt)
{
    const double G = 80.;
    const double VREF = 3.3;
    return ((VREF / 2.) - RawVolt) / G; // general solve
}

void changeSPI3PWM()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001001000111000); // 0 0010 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}
void changeSPIGAIN2_20()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001100000000100); // 0 0011 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void changeSPIGAIN2_40()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001100000001000); // 0 0011 00000001000
    digitalWrite(chipSelectPin, HIGH);         // manually take CSN high between spi transmissions
}

void changeSPIGAIN2_80()
{
    digitalWrite(chipSelectPin, LOW);          // manually take CSN low for SPI_1 transmission
    data = SPI.transfer16(0b0001100000001100); // 0 0011 00000001000
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