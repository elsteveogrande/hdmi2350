#pragma once

#include "rp2350/Common.h"

namespace cxx {

struct [[gnu::packed]] [[gnu::aligned(4)]] PanicData {
  // All registers preserved as of the time the exception occurs,
  // along with additional info

  // Original stack pointer, at time of fault; calculated in fault handler
  u32 sp;

  // Set by the (hard, memory, bus, usage) fault handler
  u32 type;

  // Pushed by the fault handler
  u32 r4, r5, r6, r7, r8, r9, r10, r11;

  // Exception info set by the CPU during faults
  u32 exc;

  // CPU automatically pushed these onto the stack before entering fault handler; see:
  // https://developer.arm.com/documentation/100235/0100/The-Cortex-M33-Processor/Exception-model/Exception-entry-and-return/Exception-entry-
  u32 r0, r1, r2, r3, r12, lr, pc, psr;
};

[[noreturn]] __attribute__((interrupt())) void __hardFault();
[[noreturn]] __attribute__((interrupt())) void __memManage();
[[noreturn]] __attribute__((interrupt())) void __busFault();
[[noreturn]] __attribute__((interrupt())) void __usageFault();

[[gnu::used]] [[gnu::noinline]] [[noreturn]]
void __panic(PanicData const& pd);

} // namespace cxx
