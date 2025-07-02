#pragma once

#include "rp2350/Common.h"
#include "rp2350/Resets.h"

/*
Section 12.11. HSTX
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct HSTX {

  //
  // 12.11.7, Control Registers
  //

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
      auto& rotates(this auto const& self, u8 v) { return self.set(20, 16, v); }
      auto& rotBits(this auto const& self, u8 v) { return self.set(12, 8, v); }
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
    struct Fields {
      bool clock(this auto const& self) { return self.bit(17); }
      bool invert(this auto const& self) { return self.bit(16); }
      u8   selectN(this auto const& self) { return self.get(12, 8); }
      u8   selectP(this auto const& self) { return self.get(4, 0); }

      auto& clock(this auto const& self, bool v) { return self.bit(17, v); }
      auto& invert(this auto const& self, bool v) { return self.bit(16, v); }
      auto& selectN(this auto const& self, u8 v) { return self.set(12, 8, v); }
      auto& selectP(this auto const& self, u8 v) { return self.set(4, 0, v); }
    };
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

  //
  // 12.11.8: FIFO registers
  //

  constexpr static u32 kHSTXFIFOBase {0x50600000};

  struct FIFOStatusStructs {
    struct Fields {
      bool writtenOnFull(this auto const& self) { return self.bit(10); }
      bool empty(this auto const& self) { return self.bit(9); }
      bool full(this auto const& self) { return self.bit(8); }
      u8   level(this auto const& self) { return self.get(7, 0); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  using FIFOStatus = FIFOStatusStructs::R;

  FIFOStatus fifoStatus {kHSTXFIFOBase + 0x00};
  Reg32      fifo {kHSTXFIFOBase + 0x04};

  void reset() { Resets {}.cycleHSTX(); }
};
