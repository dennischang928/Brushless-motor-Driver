#include <arduino.h>
#include <eXoCAN.h>

#define bluePillLED PC13


int txMsgID = 0x005;      //tx MSg
uint8_t txData[1] = {1};
uint8_t txDataLen = 1;


int id, fltIdx;           //rx MSG
uint8_t rxbytes[1];

eXoCAN can;

void setup()
{
  can.begin(STD_ID_LEN, BR250K, PORTB_8_9_WIRE_PULLUP); // 11b IDs, 250k bit rate, no transceiver chip, portA pins 11,12
  Serial1.begin(115200);
  Serial1.println("Start_up");
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
  //  txData = 0;
  can.transmit(txMsgID, txData, txDataLen);
  delay(2000);
  digitalWrite(PB11, LOW);
  delay(1000);
  for (int i = 0; i < 3; i++) {
    if (can.receive(id, fltIdx, rxbytes) > -1) {
      Serial1.println(rxbytes[0]);
      digitalWrite(PB11, HIGH);
//      break;
    } else {
      Serial1.println("Nothing");
    }
    delay(100);
  }
  while (true);

}
