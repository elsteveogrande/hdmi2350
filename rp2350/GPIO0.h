#pragma once

#include "rp2350/Reg.h"

/*
Section 9, GPIO
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
*/

struct GPIO0 {

  constexpr static u32 kBank0Base = 0x40028000;

  struct Status : Reg {
    /** IRQTOPROC: interrupt to processors, after override is applied */
    bool irqToProc(this auto& self) { return self.bit(26); }
    /** INFROMPAD: input signal from pad, before override is applied */
    bool inFromPad(this auto& self) { return self.bit(17); }
    /** OETOPAD: output enable to pad after register override is applied */
    bool outEnbToPad(this auto& self) { return self.bit(13); }
    /** OUTTOPAD: output signal to pad after register override is applied */
    bool outToPad(this auto& self) { return self.bit(9); }
  };

  struct Control : Reg {
    enum class Override : u8 {
      kNormal  = 0,
      kInvert  = 1,
      kLow     = 2,
      kDisable = 2,
      kHigh    = 3,
      kEnable  = 3,
    };
    /* 29:28 IRQOVER */
    Override irqOver(this auto& self) { return Override(self.get(30, 28)); }
    /* 17:16 INOVER */
    Override inOver(this auto& self) { return Override(self.get(18, 16)); }
    /* 15:14 OEOVER */
    Override oeOver(this auto& self) { return Override(self.get(16, 14)); }
    /* 13:12 OUTOVER */
    Override outOver(this auto& self) { return Override(self.get(14, 12)); }

    auto irqOver(this auto const& self, Override over) { return self.set(30, 28, u32(over)); }
    auto inOver(this auto const& self, Override over) { return self.set(18, 16, u32(over)); }
    auto oeOver(this auto const& self, Override over) { return self.set(15, 14, u32(over)); }
    auto outOver(this auto const& self, Override over) { return self.set(14, 12, u32(over)); }

    /** 9.4. Function Select */
    enum class FuncSel : u8 {
      // Note that the correct funcsel depends on the GPIO number.
      HSTX      = 0,
      SPI       = 1,
      UART2     = 2,
      I2C       = 3,
      PWM       = 4,
      SIO       = 5,
      PIO0      = 6,
      PIO1      = 7,
      PIO2      = 8,
      TRACEDATA = 9,
      QMI       = 9,
      CLOCK     = 9,
      USBVBUS   = 10,
      USBOVRCUR = 10,
      UART11    = 11,
      NULL_FN   = 31,
    };

    Control const& funcSel(this auto const& self, FuncSel v) { return self.set(5, 0, u8(v)); }
    u32            funcSel(this auto& self) { return self.get(5, 0); }
  };

  static Status  status(u8 index) { return {kBank0Base + (index * 8)}; }
  static Control control(u8 index) { return {kBank0Base + (index * 8) + 4}; }

  // TODO: many more registers
};
