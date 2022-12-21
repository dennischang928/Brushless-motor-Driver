# BLDC Driver

## Electronics

- MCU
  - STM32F103C8T6 (Blue pill)
- Mosfet
  - IRLR7843 (CMOSE)
- Mosfet Driver
  - DRV8303
    - PIN
      -  INHA PA8
      -  INLA PB6
      -  SO1 **PA11**
         
      -  INHB PA9
      -  INLB PB7
      -  SO2 **PA12**
         
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

**For Resistors and Capacitors, use 0603 package.**

## Programming

- MCU

  - Can Bus
    - Library
      [eXoCAN](https://github.com/exothink/eXoCAN)
      - 
