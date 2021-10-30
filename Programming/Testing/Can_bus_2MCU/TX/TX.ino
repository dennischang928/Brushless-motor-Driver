/*
  bpCanBlink.ino                                                                     4/24/20
  C:\Users\jhe\Documents\PlatformIO\mySTM32LIBS\eXoCAN\bpCanBlink.cpp

  This simple example requires two STM32f103 boards.  One of the boards is loaded
  with the 'frame' using txMsg = 0x069.  The second board gets loaded with the second
  frame where tsMsgID = 0x005 by uncommenting that section.

  On each board connect CAN default RX and TX pins(11,12) together.  Then connect
  the boards together: +5V, Gnd., and Can TX/RX.

  When a board receives a CAN message it toggles its LED.  One board sends a message
  every second and the other every five seconds.

  RAM:   904 bytes
  Flash: 12988 bytes

  working                                                                            4/25

*/
#include <arduino.h>
#include <eXoCAN.h>

#define bluePillLED PC13


//  ****** uncomment the following for the second stm32f103 board ******

int txMsgID = 0x005;
uint8_t txData[8] = {0, 0xfe, 0xdc, 0xba, 0x11, 0x12, 0x34, 0x56};
uint8_t txDataLen = 8;

eXoCAN can;

void setup()
{
  can.begin(STD_ID_LEN, BR250K, PORTB_8_9_WIRE_PULLUP); // 11b IDs, 250k bit rate, no transceiver chip, portA pins 11,12
  pinMode(PB11, OUTPUT);
  for (int i = 0; i < 3; i++) {
    digitalWrite(PB11, HIGH);
    delay(200);
    digitalWrite(PB11, LOW);
    delay(200);
  }
}

uint32_t last = 0;
void loop()
{
  digitalWrite(PB11, HIGH);
  txData[0] = 0;
  can.transmit(txMsgID, txData, txDataLen);
  delay(1);
  txData[0] = 3;
  can.transmit(txMsgID, txData, txDataLen);

  delay(2000);
  digitalWrite(PB11, LOW);
  delay(1700);
}
