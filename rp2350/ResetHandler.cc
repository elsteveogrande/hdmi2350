#include "rp2350/ResetHandler.h"
#include "rp2350/Clocks.h"
#include "rp2350/Common.h"
#include "rp2350/HSTX.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "rp2350/Ticks.h"
#include "rp2350/VecTable.h"

namespace init {

void clearBSS() {
  auto* bss = (u8*)__bss_base;
  for (u32 i = 0; i < __bss_size; i++) { bss[i] = 0; }
}

void bork() {}

} // namespace init

[[gnu::used]] [[gnu::retain]] [[noreturn]] void Handlers::reset() {
  init::clearBSS();

  Reg32 bork {u32(0x400d800c)};

  SIO sio;
  initGPIOOut(kPicoLED);
  initGPIOOut(kPanicUARTTX);
  sio.gpioOut().bit(kPanicUARTTX, true);
  sio.gpioOut().bit(kPicoLED, true);

  Resets resets;
  // resets.cyclePADSBANK0();
  // resets.cycleTIMER0();

  XOSC x;
  x.startup.set(20, 20, 0).set(13, 0, 1000); // delay = 256 * 1000 / 12M = appx 21 ms
  x.control.set(11, 0, 0xaa0);               // 0xaa0 means 1-15 MHz
  x.control.set(23, 12, 0xfab);              // 0xfab means enable
  x.dormant.set(31, 0, 0x77616b65);          // "wake"
  while (!x.status.bit(31)) { _BUSY_LOOP(); }

  // https://github.com/Chalandi/Blinky_Pico2_dual_core_nosdk/blob/master/Code/Mcal/Clock/Clock.c
  // https://github.com/dougsummerville/Bare-Metal-Raspberry-Pi-Pico-2/blob/dot/baremetal/_crt0.c

  resets.cyclePLLSYS();

  Clocks c;

  // c.sys.control()
  //     .set(31, 0, 0)
  //     .auxSource(Clocks::SysControlStruct::AuxSource::PLL_SYS)
  //     .clkSource(Clocks::SysControlStruct::ClkSource::CLK_SYS_AUX);
  // while (!u8(c.sys.selected().source())) { _BUSY_LOOP() }
  c.sys.div().set(31, 16, 1).set(15, 0, 0); // divide by 1.0

  c.ref.control()
      .set(31, 0, 0) // clear
      .clkSource(Clocks::RefControlStruct::ClkSource::XOSC);
  while (!u8(c.ref.selected().source())) { _BUSY_LOOP() }
  c.ref.div().set(31, 16, 12).set(15, 0, 0); // divide by 12.0

  // c.refSelected().set(3, 0, 4); // select bit 2
  // while (c.refSelected().get(3, 0) != 4) { ArmInsns::nop(); }
  // c.sysControl().set(1, 0, 1);  // SRC <- CLKSRC_CLK_REF_AUX
  // c.sysSelected().set(3, 0, 2); // select bit 1
  // while (c.sysSelected().get(3, 0) != 2) { ArmInsns::nop(); }

  // Ticks ticks;
  // ticks.timer0Cycles().set(8, 0, 12);
  // ticks.timer0Control().set(0, 0, true);

  // p569: SDK expects nominal 1uS system ticks, as does Arm internals.
  // Although we don't use the SDK we'll assume 1uS everywhere as well.
  // resets.reset.resetTIMER0(false);
  // while (!resets.resetDone.resetDoneTIMER0()) { _BUSY_LOOP(); }

  // ticks.proc0Control().enable(0); // disable for config
  // ticks.proc0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
  // ticks.proc0Control().enable(1);
  // while (!ticks.proc0Control().running()) { _BUSY_LOOP(); }

  // ticks.timer0Control().enable(0); // disable for config
  // ticks.timer0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
  // ticks.timer0Control().enable(1);
  // while (!ticks.timer0Control().running()) { _BUSY_LOOP(); }

  // resets.cycleTIMER0();
  // auto t0 = Timers::timer0();

  Reg32 {0xaaaaaaaa}.bit(0, 0);

  start();
  __builtin_unreachable();
}
