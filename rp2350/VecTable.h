#pragma once

#include "rp2350/Common.h"

extern "C" {
// Addresses for these are defined in the linker script
extern void* __vec_table;
extern void* __stack_top;
}

/*
Section 3.2. Interrupts
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf

Cortex-M33 interrupts are described here:
https://developer.arm.com/documentation/100235/0100/The-Cortex-M33-Processor/Exception-model/Vector-table
*/

// clang-format off
struct [[gnu::aligned(256)]] Handlers {
  typedef void (*Handler)();
  static Handler handlers[64];

  /** Defined in `ResetHandler.cc` */
  [[gnu::noinline]] [[gnu::aligned(16)]] [[noreturn]] static void reset();

  [[gnu::noinline]] [[gnu::aligned(16)]] static void irqn(u8 intNumber);
  [[gnu::noinline]] [[gnu::aligned(16)]] static void unknown(u32 intNumber);

  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void nmi();
  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void svCall();
  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void dbgMon();
  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void pendSV();
  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void sysTick();
  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void irq();
  [[gnu::noinline]] [[gnu::aligned(16)]] __attribute__((interrupt)) static void unknown();
};
// clang-format on
