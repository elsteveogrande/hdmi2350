# rp2350 library

This aims to implement all the hardware and other registers
in the datasheet.  This implementation is not complete of course.  Covers much of the stuff you want for a new project, hopefully.

https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf

| Section     | Corresponding impl (`.h`, maybe `.cc`'s)    |
| ----------- | ---------------------- |
| 3.1         | `rp2350/SIO.h`         |
| 3.2         | `rp2350/VecTable.h`    |
| 3.3, 3.4    | `rp2350/Events.h`      |
| 3.5         | `rp2350/Debug.h`       |
| 6           | `rp2350/Power.h`       |
| 7           | `rp2350/Resets.h`      |
| 8           | `rp2350/Clocks.h`      |
| 9           | `rp2350/GPIO.h`        |
| 11          | `rp2350/PIO.h`         |
| 12.1        | `rp2350/UART.h`        |
| 12.2        | `rp2350/I2C.h`         |
| 12.3        | `rp2350/SPI.h`         |
| 12.4        | `rp2350/ADC.h`         |
| 12.6        | `rp2350/DMA.h`         |
| 12.8        | `rp2350/Timers.h`      |
| 12.11       | `rp2350/HSTX.h`        |
| 12.15       | `rp2350/SysControl.h`  |
