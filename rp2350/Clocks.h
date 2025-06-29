#pragma once

#include "Common.h"

// Covers several components from Chapter 8, "Clocks":
// clocks, XOSC, PLL, Tick generators

struct Clocks {
  constexpr static u32 kBase = 0x40010000;

  /*
  BEWARE that these different clock control registers,
  and their auxiliary / clock-source bit fields, all
  differ subtly.

  TODO: refactor this insanity
  */

  struct GPControlStruct {
    enum class AuxSource : u8 {
      PLL_SYS              = 0,
      GPIN0                = 1,
      GPIN1                = 2,
      PLL_USB              = 3,
      PLL_USB_PRI_REF_OPCG = 4, // ???
      ROSC                 = 5,
      XOSC                 = 6,
      LPOSC                = 7,
      CLK_SYS              = 8,
      CLK_USB              = 9,
      CLK_ADC              = 10,
      CLK_REF              = 11,
      CLK_PERI             = 12,
      CLK_HSTX             = 13,
      OTP_CLK2FC           = 14,
    };

    struct Fields {
      bool enabled(this auto const& self) { return self.bit(28); }

      bool      nudge(this auto const& self) { return self.bit(20); }
      u8        phase(this auto const& self) { return self.get(17, 16); }
      bool      dc50(this auto const& self) { return self.bit(12); }
      bool      enable(this auto const& self) { return self.bit(11); }
      bool      kill(this auto const& self) { return self.bit(10); }
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(8, 5)); }

      auto& nudge(this auto const& self, bool v) { return self.bit(20, v); }
      auto& phase(this auto const& self, u8 v) { return self.get(17, 16, v); }
      auto& dc50(this auto const& self, bool v) { return self.bit(12, v); }
      auto& enable(this auto const& self, bool v) { return self.bit(11, v); }
      auto& kill(this auto const& self, bool v) { return self.bit(10, v); }
      auto& auxSource(this auto const& self, AuxSource v) { return self.set(8, 5, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct RefControlStruct {
    enum class AuxSource : u8 {
      PLL_USB              = 0,
      PLL_GPIN0            = 1,
      PLL_GPIN1            = 2,
      PLL_USB_PRI_REF_OPCG = 3,
    };

    enum class ClkSource : u8 {
      ROSC_PH     = 0,
      CLK_REF_AUX = 1,
      XOSC        = 2,
      LPOSC       = 3,
    };

    struct Fields {
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(6, 5)); }
      ClkSource clkSource(this auto const& self) { return self.get(1, 0); }

      auto& auxSource(this auto const& self, AuxSource v) { return self.set(6, 5, u8(v)); }
      auto& clkSource(this auto const& self, ClkSource v) { return self.set(1, 0, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct RefSelectedStruct {
    enum class SelectedSrc {
      ROSC_PH     = 1 << 0,
      CLK_REF_AUX = 1 << 1,
      XOSC        = 1 << 2,
      LPOSC       = 1 << 3,
    };

    struct Fields {
      SelectedSrc source(this const auto& self) { return SelectedSrc(self.get(3, 0)); }
    };

    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct SysControlStruct {
    enum class AuxSource : u8 {
      PLL_SYS   = 0,
      PLL_USB   = 1,
      ROSC      = 2,
      XOSC      = 3,
      PLL_GPIN0 = 4,
      PLL_GPIN1 = 5,
    };

    enum class ClkSource : u8 {
      CLK_REF     = 0,
      CLK_SYS_AUX = 1,
    };

    struct Fields {
      bool enabled(this auto const& self) { return self.bit(28); }

      bool      nudge(this auto const& self) { return self.bit(20); }
      u8        phase(this auto const& self) { return self.get(17, 16); }
      bool      dc50(this auto const& self) { return self.bit(12); }
      bool      enable(this auto const& self) { return self.bit(11); }
      bool      kill(this auto const& self) { return self.bit(10); }
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(7, 5)); }
      ClkSource clkSource(this auto const& self) { return ClkSource(self.bit(0)); }

      auto& nudge(this auto const& self, bool v) { return self.bit(20, v); }
      auto& phase(this auto const& self, u8 v) { return self.get(17, 16, v); }
      auto& dc50(this auto const& self, bool v) { return self.bit(12, v); }
      auto& enable(this auto const& self, bool v) { return self.bit(11, v); }
      auto& kill(this auto const& self, bool v) { return self.bit(10, v); }
      auto& auxSource(this auto const& self, AuxSource v) { return self.set(7, 5, u8(v)); }
      auto& clkSource(this auto const& self, ClkSource v) { return self.bit(0, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct SysSelectedStruct {
    enum class SelectedSrc {
      CLK_REF     = 1 << 0,
      CLK_SYS_AUX = 1 << 1,
    };

    struct Fields {
      SelectedSrc source(this const auto& self) { return SelectedSrc(self.get(1, 0)); }
    };

    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct PeriControlStruct {
    enum class AuxSource : u8 {
      CLK_SYS = 0,
      PLL_SYS = 1,
      PLL_USB = 2,
      ROSC_PH = 3,
      XOSC    = 4,
      GPIN0   = 5,
      GPIN1   = 6,
    };

    struct Fields {
      bool enabled(this auto const& self) { return self.bit(28); }

      bool      enable(this auto const& self) { return self.bit(11); }
      bool      kill(this auto const& self) { return self.bit(10); }
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(7, 5)); }

      auto& enable(this auto const& self, bool v) { return self.bit(11, v); }
      auto& kill(this auto const& self, bool v) { return self.bit(10, v); }
      auto& auxSource(this auto const& self, AuxSource v) { return self.set(7, 5, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct HSTXControlStruct {
    enum class AuxSource : u8 {
      CLK_SYS = 0,
      PLL_SYS = 1,
      PLL_USB = 2,
      GPIN0   = 3,
      GPIN1   = 4,
    };

    struct Fields {
      bool enabled(this auto const& self) { return self.bit(28); }

      bool      nudge(this auto const& self) { return self.bit(20); }
      u8        phase(this auto const& self) { return self.get(17, 16); }
      bool      enable(this auto const& self) { return self.bit(11); }
      bool      kill(this auto const& self) { return self.bit(10); }
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(7, 5)); }

      auto& nudge(this auto const& self, bool v) { return self.bit(20, v); }
      auto& phase(this auto const& self, u8 v) { return self.get(17, 16, v); }
      auto& enable(this auto const& self, bool v) { return self.bit(11, v); }
      auto& kill(this auto const& self, bool v) { return self.bit(10, v); }
      auto& auxSource(this auto const& self, AuxSource v) { return self.set(7, 5, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct USBControlStruct {
    enum class AuxSource : u8 {
      PLL_USB = 0,
      PLL_SYS = 1,
      ROSC_PH = 2,
      XOSC    = 3,
      GPIN0   = 4,
      GPIN1   = 5,
    };

    struct Fields {
      bool enabled(this auto const& self) { return self.bit(28); }

      bool      nudge(this auto const& self) { return self.bit(20); }
      u8        phase(this auto const& self) { return self.get(17, 16); }
      bool      enable(this auto const& self) { return self.bit(11); }
      bool      kill(this auto const& self) { return self.bit(10); }
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(7, 5)); }

      auto& nudge(this auto const& self, bool v) { return self.bit(20, v); }
      auto& phase(this auto const& self, u8 v) { return self.get(17, 16, v); }
      auto& enable(this auto const& self, bool v) { return self.bit(11, v); }
      auto& kill(this auto const& self, bool v) { return self.bit(10, v); }
      auto& auxSource(this auto const& self, AuxSource v) { return self.set(7, 5, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct ADCControlStruct {
    enum class AuxSource : u8 {
      PLL_USB = 0,
      PLL_SYS = 1,
      ROSC_PH = 2,
      XOSC    = 3,
      GPIN0   = 4,
      GPIN1   = 5,
    };

    struct Fields {
      bool enabled(this auto const& self) { return self.bit(28); }

      bool      nudge(this auto const& self) { return self.bit(20); }
      u8        phase(this auto const& self) { return self.get(17, 16); }
      bool      enable(this auto const& self) { return self.bit(11); }
      bool      kill(this auto const& self) { return self.bit(10); }
      AuxSource auxSource(this auto const& self) { return AuxSource(self.get(7, 5)); }

      auto& nudge(this auto const& self, bool v) { return self.bit(20, v); }
      auto& phase(this auto const& self, u8 v) { return self.get(17, 16, v); }
      auto& enable(this auto const& self, bool v) { return self.bit(11, v); }
      auto& kill(this auto const& self, bool v) { return self.bit(10, v); }
      auto& auxSource(this auto const& self, AuxSource v) { return self.set(7, 5, u8(v)); }
    };
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  struct DivStruct {
    struct Fields {};
    // clang-format off
    // Boilerplate for R, U classes
    struct U;
    struct R : Fields, Reg<R, U> { R(auto addr) : Reg(addr) {} };
    struct U : Fields, Update<U, R> { U(auto* reg) : Update(reg) {} };
    // clang-format on
  };

  using GPControl   = GPControlStruct::R;
  using RefControl  = RefControlStruct::R;
  using SysControl  = SysControlStruct::R;
  using PeriControl = PeriControlStruct::R;
  using HSTXControl = HSTXControlStruct::R;
  using USBControl  = USBControlStruct::R;
  using ADCControl  = ADCControlStruct::R;
  using Div         = DivStruct::R;
  using RefSelected = RefSelectedStruct::R;
  using SysSelected = SysSelectedStruct::R;

  template <class ControlT> struct Clock {
    u32 const base;
    ControlT  control() { return {base + 0}; }
    Div       div() { return {base + 4}; }
  };

  struct GPClock : Clock<GPControl> {
    // Ignore SELECTED, it's hardwired to 1
  };
  GPClock gp0 {kBase + 0x00};
  GPClock gp1 {kBase + 0x0c};
  GPClock gp2 {kBase + 0x18};
  GPClock gp3 {kBase + 0x24};

  struct RefClock : Clock<RefControl> {
    RefSelected selected() { return {base + 8}; }
  };
  RefClock ref {kBase + 0x30};

  struct SysClock : Clock<SysControl> {
    SysSelected selected() { return {base + 8}; }
  };
  SysClock sys {kBase + 0x3c};

  struct PeriClock : Clock<PeriControl> {};
  PeriClock peri {kBase + 0x48};

  struct HSTXClock : Clock<HSTXControl> {};
  HSTXClock hstx {kBase + 0x54};

  struct USBClock : Clock<USBControl> {};
  USBClock usb {kBase + 0x60};

  struct ADCClock : Clock<ADCControl> {};
  ADCClock adc {kBase + 0x6c};
};

struct XOSC {
  constexpr static u32 kBase = 0x40048000;

  Reg32 control {kBase + 0x00};
  Reg32 status {kBase + 0x04};
  Reg32 dormant {kBase + 0x08};
  Reg32 startup {kBase + 0x0c};
  Reg32 count {kBase + 0x10};
};

struct PLLs {
  struct PLL {
    u32 const base;

    Reg32 cs {base + 0x00};
    Reg32 power {base + 0x04};
    Reg32 fbdiv {base + 0x08};
    Reg32 prim {base + 0x0c};
  };

  PLL sysPLL {0x40050000};
  PLL usbPLL {0x40058000};
};
