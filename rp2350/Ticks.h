#pragma once

#include "rp2350/Common.h"

/** Section 8.5 */
struct Ticks {
  struct ControlStructs {
    struct Fields {
      bool  running(this const auto& self) { return self.bit(1); }
      bool  enable(this const auto& self) { return self.bit(0); }
      auto& enable(this const auto& self, bool v) { return self.bit(0, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct CyclesStructs {
    struct Fields {
      u8    count(this const auto& self) { return self.get(8, 0); }
      auto& count(this const auto& self, u8 v) { return self.set(8, 0, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct CountStructs {
    struct Fields {
      u8 countdown(this const auto& self) { return self.get(8, 0); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  using Control = ControlStructs::R;
  using Cycles  = CyclesStructs::R;
  using Count   = CountStructs::R;

  constexpr static u32 kBase = 0x40108000;

  Control proc0Control() { return {kBase + 0x00}; }
  Cycles  proc0Cycles() { return {kBase + 0x04}; }
  Count   proc0Count() { return {kBase + 0x08}; }
  Control proc1Control() { return {kBase + 0x0c}; }
  Cycles  proc1Cycles() { return {kBase + 0x10}; }
  Count   proc1Count() { return {kBase + 0x14}; }
  Control timer0Control() { return {kBase + 0x18}; }
  Cycles  timer0Cycles() { return {kBase + 0x1c}; }
  Count   timer0Count() { return {kBase + 0x20}; }
  Control timer1Control() { return {kBase + 0x24}; }
  Cycles  timer1Cycles() { return {kBase + 0x28}; }
  Count   timer1Count() { return {kBase + 0x2c}; }
  Control watchdogControl() { return {kBase + 0x30}; }
  Cycles  watchdogCycles() { return {kBase + 0x34}; }
  Count   watchdogCount() { return {kBase + 0x38}; }
  Control riscvControl() { return {kBase + 0x3c}; }
  Cycles  riscvCycles() { return {kBase + 0x40}; }
  Count   riscvCount() { return {kBase + 0x44}; }
};
