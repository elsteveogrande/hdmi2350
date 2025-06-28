#pragma once

#include "rp2350/Common.h"

struct Timer {
  u32 base_;

  Reg32 timeHR() { return {base_ + 0x08}; }
  Reg32 timeLR() { return {base_ + 0x0c}; }
  Reg32 alarm0() { return {base_ + 0x10}; }
  Reg32 timeRawHR() { return {base_ + 0x24}; }
  Reg32 timeRawLR() { return {base_ + 0x28}; }
  Reg32 intEnable() { return {base_ + 0x40}; }
};

struct Timers {
  constexpr static u32 kTimer0Base {0x400b0000};
  constexpr static u32 kTimer1Base {0x400b8000};

  static Timer timer0() { return {Timers::kTimer0Base}; }
  static Timer timer1() { return {Timers::kTimer1Base}; }
};
