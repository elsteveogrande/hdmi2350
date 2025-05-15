#pragma once

#include "RP2350/Common.h"

/*
Section 9.11.3. Pad Control - User Bank
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct Pads {

  struct UserBank {
    constexpr static u32 kBase {0x40038000}; // PADS_BANK0_BASE

    enum class Drive : u8 {
      k2mA  = 0,
      k4mA  = 1,
      k8mA  = 2,
      k12mA = 3,
    };

    Reg32 voltage {kBase + 0x00};

    struct ControlStructs {
      struct Fields {
        bool  isolation(this auto const& self) { return self.bit(8); }
        bool  outputDisable(this auto const& self) { return self.bit(7); }
        bool  inputEnable(this auto const& self) { return self.bit(6); }
        Drive drive(this auto const& self) { return Drive(self.get(6, 4)); }
        bool  pullUpEnable(this auto const& self) { return self.bit(3); }
        bool  pullDownEnable(this auto const& self) { return self.bit(2); }
        bool  schmitt(this auto const& self) { return self.bit(1); }
        bool  slewfast(this auto const& self) { return self.bit(0); }

        auto& isolation(this auto const& self, bool v) { return self.bit(8, v); }
        auto& outputDisable(this auto const& self, bool v) { return self.bit(7, v); }
        auto& inputEnable(this auto const& self, bool v) { return self.bit(6, v); }
        auto& drive(this auto const& self, Drive v) { return self.set(6, 4, u8(v)); }
        auto& pullUpEnable(this auto const& self, bool v) { return self.bit(3, v); }
        auto& pullDownEnable(this auto const& self, bool v) { return self.bit(2, v); }
        auto& schmitt(this auto const& self, bool v) { return self.bit(1, v); }
        auto& slewfast(this auto const& self, bool v) { return self.bit(0, v); }
      };
      // clang-format off
      // Boilerplate for R, U classes
      struct U;
      struct R : Fields, Reg<R, U> { explicit R(auto addr) : Reg(addr) {} };
      struct U : Fields, Update<U, R> { explicit U(auto* reg) : Update(reg) {} };
      // clang-format on
    };
    using Control = ControlStructs::R;

    Control gpio(u8 i) { return Control {kBase + 4 + (4 * i)}; }
  };
};
