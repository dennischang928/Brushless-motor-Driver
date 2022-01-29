# BLDC Driver

## Electronics

- MCU
  - STM32F103C8T6 (Blue pill)

- Mosfet
  - IRLR7843 (CMOSE)
- Mosfet Driver
  - DRV8305
    - PIN
      -  INHA PA8
      -  INLA PA9
      -  SO1 PB1
      -  INHB PA10
      -  INLB PB6
      -  SO2 PB0
      -  INHC PB7
      -  INLC PB8
      -  SO3 PA3
      -  LED PC13
      -  EN_GATE PB12
      -  nFAULT PB13
      -  nOCTW PB14
      -  DC_CAL PB4
      -  SPI
         -  SPI1_NSS_PIN PA4
         -  SDO PA6
         -  SDI PA7
         -  SCLK PA5

**For Resistors and Capacitors, use 0603 size.**

**Use T-stop Layer for non-綠油層**

## Programming

- MCU

  - Can Bus
    - Library
      [eXoCAN](https://github.com/exothink/eXoCAN)
      - 

  
