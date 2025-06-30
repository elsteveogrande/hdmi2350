#pragma once

#include "rp2350/Common.h"

namespace cxx {

struct [[gnu::aligned(4)]] PanicData {
  u32 sp;
  u32 r4;
  u32 r5;
  u32 r6;
  u32 r7;
  u32 r8;
  u32 r9;
  u32 r10;
  u32 r11;
  u32 exc;
  u32 type;
  u32 r0;
  u32 r1;
  u32 r2;
  u32 r3;
  u32 r12;
  u32 lr;
  u32 pc;
  u32 psr;
};

[[noreturn]] [[gnu::aligned(16)]] __attribute__((interrupt())) void __hardFault();
[[noreturn]] [[gnu::aligned(16)]] __attribute__((interrupt())) void __memManage();
[[noreturn]] [[gnu::aligned(16)]] __attribute__((interrupt())) void __busFault();
[[noreturn]] [[gnu::aligned(16)]] __attribute__((interrupt())) void __usageFault();

[[gnu::used]] [[gnu::noinline]] [[gnu::aligned(16)]] [[noreturn]]
void __panic(PanicData const& pd);

} // namespace cxx
