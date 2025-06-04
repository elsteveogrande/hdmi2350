#pragma once

#include "RP2350/Common.h"

/*
Section 12.11. HSTX
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct HSTX {
  constexpr static u32 kHSTXControlBase {0x400c0000};

  struct CSRStructs {
    struct Fields {
      // p1207
      u8   clkDiv(this auto const& self) { return self.get(31, 28); }
      u8   clkPhase(this auto const& self) { return self.get(27, 24); }
      u8   nShifts(this auto const& self) { return self.get(20, 16); }
      u8   shift(this auto const& self) { return self.get(12, 8); }
      u8   coupledSel(this auto const& self) { return self.get(6, 5); }
      bool coupledMode(this auto const& self) { return self.bit(4); }
      bool expandEnabled(this auto const& self) { return self.bit(1); }
      bool enabled(this auto const& self) { return self.bit(0); }

      auto& clkDiv(this auto const& self, u8 v) { return self.set(31, 28, v); }
      auto& clkPhase(this auto const& self, u8 v) { return self.set(27, 24, v); }
      auto& nShifts(this auto const& self, u8 v) { return self.set(20, 16, v); }
      auto& shift(this auto const& self, u8 v) { return self.set(12, 8, v); }
      auto& coupledSel(this auto const& self, u8 v) { return self.set(6, 5, v); }
      auto& coupledMode(this auto const& self, bool v) { return self.bit(4, v); }
      auto& expandEnabled(this auto const& self, bool v) { return self.bit(1, v); }
      auto& enabled(this auto const& self, bool v) { return self.bit(0, v); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct BitStructs {
    struct Fields {};
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct ExpandShiftStructs {
    struct Fields {};
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct ExpandTMDSStructs {
    struct Fields {};
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  using CSR         = CSRStructs::R;
  using Bit         = BitStructs::R;
  using ExpandShift = ExpandShiftStructs::R;
  using ExpandTMDS  = ExpandTMDSStructs::R;

  CSR         csr {kHSTXControlBase + 0x00};
  Bit         bit(u8 i) { return {kHSTXControlBase + ((i + 1u) << 2)}; }
  ExpandShift expandShift {kHSTXControlBase + 0x24};
  ExpandTMDS  expandTMDS {kHSTXControlBase + 0x28};
};
