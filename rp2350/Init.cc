#include "rp2350/Clocks.h"
#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "rp2350/Ticks.h"

[[gnu::used]] [[gnu::retain]] void rp2350Init() {

  Resets resets;

  XOSC x;
  x.startup.set(20, 20, 0).set(13, 0, 1000);  // delay = 256 * 1000 / 12M = appx 21 ms
  x.control.set(11, 0, 0xaa0);                // 0xaa0 means 1-15 MHz
  x.control.set(23, 12, 0xfab);               // 0xfab means enable
  x.dormant.set(31, 0, 0x77616b65);           // "wake"
  while (!x.status.bit(31)) { _BUSY_LOOP(); } // wait for stable

  // https://github.com/Chalandi/Blinky_Pico2_dual_core_nosdk/blob/master/Code/Mcal/Clock/Clock.c
  // https://github.com/dougsummerville/Bare-Metal-Raspberry-Pi-Pico-2/blob/dot/baremetal/_crt0.c

  resets.cyclePLLSYS();
  auto pll = PLLs {}.sysPLL;
  pll.cs.set(5, 0, 1);                      // REFDIV=1
  pll.fbdiv.set(11, 0, 125);                // set VCO to (12*125) = 1500MHz
  pll.prim.set(18, 16, 5).set(14, 12, 2);   // divide clock by (5*2) to get 150MHz
  pll.power.set(5, 0, 0);                   // turn off power-down bits
  while (!pll.cs.bit(31)) { _BUSY_LOOP(); } // wait for LOCK

  Clocks c;

  c.sys.control()
      .set(31, 0, 0)
      .auxSource(Clocks::SysControlStruct::AuxSource::PLL_SYS)
      .clkSource(Clocks::SysControlStruct::ClkSource::CLK_SYS_AUX);
  while (!u8(c.sys.selected().source())) { _BUSY_LOOP(); }
  c.sys.div().set(31, 16, 1).set(15, 0, 0); // divide by 1.0

  c.ref.control()
      .set(31, 0, 0) // clear
      .clkSource(Clocks::RefControlStruct::ClkSource::XOSC);
  while (!u8(c.ref.selected().source())) { _BUSY_LOOP(); }
  c.ref.div().set(31, 16, 12).set(15, 0, 0); // divide by 12.0

  // p569: SDK expects nominal 1uS system ticks, as does Arm internals.
  // Although we don't use the SDK we'll assume 1uS everywhere as well.

  Ticks ticks;
  ticks.proc0Control().enable(0); // disable for config
  ticks.proc0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
  ticks.proc0Control().enable(1);
  while (!ticks.proc0Control().running()) { _BUSY_LOOP(); }

  M33 m33;
  m33.ccr.div0Trap(true).unalignedTrap(true);
}
