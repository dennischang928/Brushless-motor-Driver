# BLDC Driver

## Electronics

- MCU
  - STM32F103C8T6 (Blue pill)
    - Serial PIN
      - RX PA3 
      - TX PA2
- Mosfet
  - IRLR7843 (CMOSE)
- Mosfet Driver
  - DRV8303
    - PIN
      -  INHA PA8
      -  INLA PB6
      -  SO1 **PA1**
         
      -  INHB PA9
      -  INLB PB7
      -  SO2 **PA0**
         
      -  INHC PA10
      -  INLC PB8
         
      -  LED PC13
      -  EN_GATE PB12
      -  nFAULT PB13
      -  nOCTW PB14
      -  DC_CAL PB4
      -  SPI
         -  SPI1_NSS_PIN PA4
         -  SDO / MISO PA6
         -  We SDI / MOSI PA7
         -  SCLK PA5

- Encoder
  - AS5047P
    - PIN
      - A PA11
      - B PA12
      - Index PA15

**For Resistors and Ceramic Capacitors, use 0603 package.**
