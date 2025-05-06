The structure of this thing which I'm calling the "Boot Block" is:

  - An initial ARM vector table at the very beginning of flash
  - A minimal image definition: see Section 5.9.5,
    "Minimum Viable Image Metadata" in the datasheet

References:
  Datasheet: 
    https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
  ARM Vector Table:
    https://developer.arm.com/documentation/100235/0100/The-Cortex-M33-Processor/Exception-model/Vector-table
