#pragma once

#include "RP2350/Common.h"
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
  typedef void __attribute__((interrupt)) (*Handler)();
  Handler handlers[64];

  [[noreturn]] static void               reset();
  static void __attribute__((interrupt)) nmi();
  static void __attribute__((interrupt)) hardFault();
  static void __attribute__((interrupt)) memManage();
  static void __attribute__((interrupt)) busFault();
  static void __attribute__((interrupt)) usageFault();
  static void __attribute__((interrupt)) svCall();
  static void __attribute__((interrupt)) dbgMon();
  static void __attribute__((interrupt)) pendSV();
  static void __attribute__((interrupt)) sysTick();

  [[gnu::noinline]] static void unknown(u8 intNumber);
  template <u8 I> [[gnu::nodebug]]
  static void __attribute__((interrupt)) unknown() {
    [[clang::noinline]] unknown(I);
  }

  [[gnu::noinline]] static void irq(u8 intNumber);
  template <u8 I> [[gnu::nodebug]]
  static void __attribute__((interrupt)) irq() {
    [[clang::noinline]] irq(I);
  }
};
// clang-format on
