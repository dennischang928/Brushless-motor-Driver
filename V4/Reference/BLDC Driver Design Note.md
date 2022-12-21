# BLDC Driver Design

## Rgate (Driving Current)

- The resistance of Rgate need to be optimized

  - Rgate is effecting the Rise time and Fall time of the driver output
  - <img src="/Users/changdennis/Library/Application Support/typora-user-images/Screen Shot 2022-12-20 at 09.14.35.png" alt="Screen Shot 2022-12-20 at 09.14.35" style="zoom:25%;" />
  - <img src="/Users/changdennis/Library/Application Support/typora-user-images/Screen Shot 2022-12-20 at 09.16.21.png" alt="Screen Shot 2022-12-20 at 09.16.21" style="zoom:25%;" />

  - If "tRiseFall" is too small, IDrive increased, which would lead to "**Inductive Spiking and Coupling From Switching on MOSFET**"

  - Our goal is to control IDrive at minimum, while tRiseFall is reasonable. 

- Steps:

  - Both the gate resistor and the Smart Gate Drive technology adjust the gate sink and source current

  - Choose a gate resistor that is equal to the reactance of the source inductance for critically damped

    performance and choose a gate resistor that is up to twice the reactance of the source inductance for underdamped performance as described in the *External Gate Resistor Design Guide for Gate Drivers* tech note

  - Alternatively, the resistors can also be chosen by using general calculations, observing the VGS waveform, and adjusting the value higher or lower for slower or faster slew rates, respectively

  - The Smart Gate Drive source or sink current can be calculated for a given FET, as described in Section 3.1.3

  - The process for choosing the gate resistor or IDRIVE is iterative and experimental

  - If the source and sink current can be controlled within the device, for example, Smart Gate Drive, it is still a

    good practice to add a 0-Ω resistor in series with the gate of the FET and replace with a non-zero resistor if further adjustment is required

## Buck Capacitor

