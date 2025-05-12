#pragma once

#include "rp2350/Reg.h"

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

    struct Control : Reg {
      bool  isolation() const { return bit(8); }
      bool  outputDisable() const { return bit(7); }
      bool  inputEnable() const { return bit(6); }
      Drive drive() const { return Drive(get(6, 4)); }
      bool  pullUpEnable() const { return bit(3); }
      bool  pullDownEnable() const { return bit(2); }
      bool  schmitt() const { return bit(1); }
      bool  slewfast() const { return bit(0); }

      auto isolation(bool v) const { return bit(8, v); }
      auto outputDisable(bool v) const { return bit(7, v); }
      auto inputEnable(bool v) { return bit(6, v); }
      auto drive(Drive v) const { return set(6, 4, unsigned(v)); }
      auto pullUpEnable(bool v) { return bit(3, v); }
      auto pullDownEnable(bool v) { return bit(2, v); }
      auto schmitt(bool v) const { return bit(1, v); }
      auto slewfast(bool v) const { return bit(0, v); }
    };

    Reg     voltage {kBase + 0x00};
    Control gpio(u8 i) { return {kBase + 4 + (4 * i)}; }
  };

  // constexpr static u32 kBase {0x40020000}; // PADS_QSPI_BASE

  // constexpr static Reg     voltage {kBase + 0x00};
  // constexpr static Control qspiSCLK {kBase + 0x04};
  // constexpr static Control qspiSD0 {kBase + 0x08};
  // constexpr static Control qspiSD1 {kBase + 0x0c};
  // constexpr static Control qspiSD2 {kBase + 0x10};
  // constexpr static Control qspiSD3 {kBase + 0x14};
  // constexpr static Control qspiSS {kBase + 0x18};
};
