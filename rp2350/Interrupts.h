#pragma once

#include "RP2350/Common.h"

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset();

/*
Section 3.2. Interrupts
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

/**
The vector table.
This `constexpr` table is baked into flash at 0x10000000, and is used to start the system.
At startup this table is then copied into RAM at 0x20000000 so we can update IRQs etc.

Cortex-M33 interrupts are described here:
https://developer.arm.com/documentation/100235/0100/The-Cortex-M33-Processor/Exception-model/Vector-table
*/
struct VectorTable {
  // Used only at startup
  u32 const sp {0x20000400};
  void (*pc)() {&reset};

  // Non-IRQ interrupts (2 through 15); most are never used by us.
  void (*nmi)() {};
  void (*hardFault)() {};
  void (*memManage)() {};
  void (*busFault)() {};
  void (*usageFault)() {};
  void (*_reserved7)() {};
  void (*_reserved8)() {};
  void (*_reserved9)() {};
  void (*_reserved10)() {};
  void (*svCall)() {};
  void (*debugMonitor)() {};
  void (*_reserved13)() {};
  void (*pendSV)() {};
  void (*sysTick)() {};

  // IRQs start at interrupt 16.
  // The RP2350 can only ever use the first 48 IRQs
  // (out of the cortex-m33's 480(!) total IRQs).
  typedef void (*IRQHandler)();
  IRQHandler irqs[48];
};

[[gnu::section(".init_vec_table")]] [[gnu::used]] [[gnu::retain]]
constexpr VectorTable initVecTable {};
