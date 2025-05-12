#pragma once

#include "rp2350/Reg.h"

/*
Section 7.5, Subsystem Resets
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct Resets {

  enum class Periph : u8 {
    kADC       = 0,
    kBUSCTRL   = 1,
    kDMA       = 2,
    kHSTX      = 3,
    kI2C0      = 4,
    kI2C1      = 5,
    kIOBANK0   = 6,
    kIOQSPI    = 7,
    kJTAG      = 8,
    kPADSBANK0 = 9,
    kPADSQSPI  = 10,
    kPIO0      = 11,
    kPIO1      = 12,
    kPIO2      = 13,
    kPLLSYS    = 14,
    kPLLUSB    = 15,
    kPWM       = 16,
    kSHA256    = 17,
    kSPI0      = 18,
    kSPI1      = 19,
    kSYSCFG    = 20,
    kSYSINFO   = 21,
    kTBMAN     = 22,
    kTIMER0    = 23,
    kTIMER1    = 24,
    kTRNG      = 25,
    kUART0     = 26,
    kUART1     = 27,
    kUSBCTRL   = 28,
  };

  constexpr static RegBlock reset {0x40020000};
  constexpr static RegBlock resetDone {0x40020008};
};
