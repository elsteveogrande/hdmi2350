#pragma once

#include "rp2350/Common.h"
#include "runtime/Panic.h"

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

  [[gnu::noinline]] [[noreturn]] static void reset();

  [[gnu::noinline]] static void irqn(u8 intNumber);
  [[gnu::noinline]] static void unknown(u32 intNumber);

  [[gnu::noinline]] __attribute__((interrupt)) static void nmi();
  [[gnu::noinline]] __attribute__((interrupt)) static void hardFault();
  [[gnu::noinline]] __attribute__((interrupt)) static void memManage();
  [[gnu::noinline]] __attribute__((interrupt)) static void busFault();
  [[gnu::noinline]] __attribute__((interrupt)) static void usageFault();
  [[gnu::noinline]] __attribute__((interrupt)) static void svCall();
  [[gnu::noinline]] __attribute__((interrupt)) static void dbgMon();
  [[gnu::noinline]] __attribute__((interrupt)) static void pendSV();
  [[gnu::noinline]] __attribute__((interrupt)) static void sysTick();
  [[gnu::noinline]] __attribute__((interrupt)) static void irq();
  [[gnu::noinline]] __attribute__((interrupt)) static void unknown();
};
// clang-format on
