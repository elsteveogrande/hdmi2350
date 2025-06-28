#include "rp2350/ResetHandler.h"
#include "rp2350/Common.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "rp2350/Ticks.h"
#include "rp2350/Timers.h"
#include "rp2350/VecTable.h"

namespace init {

void clearBSS() {
  auto* bss = (u8*)__bss_base;
  for (u32 i = 0; i < __bss_size; i++) { bss[i] = 0; }
}

struct XOSC {
  constexpr static u32 kBase = 0x40048000;

  Reg32 control {kBase + 0x00};
  Reg32 status {kBase + 0x04};
  Reg32 dormant {kBase + 0x08};
  Reg32 startup {kBase + 0x0c};
  Reg32 count {kBase + 0x10};
};

struct SYS_PLL {
  constexpr static u32 kBase = 0x40050000;

  Reg32 cs {kBase + 0x00};
  Reg32 power {kBase + 0x04};
  Reg32 fbdiv {kBase + 0x08};
  Reg32 prim {kBase + 0x0c};
};

void initXOSC() {
  XOSC x;
  x.startup.set(20, 20, 0).set(13, 0, 1000);      // delay = 256 * 1000 / 12M = appx 21 ms
  x.control.set(23, 12, 0xfab).set(11, 0, 0xaa0); // 0xfab means enable; 0xaa0 means 1-15 MHz
  x.dormant.set(31, 0, 0x77616b65);               // "wake"
  while (!x.status.bit(31)) { _BUSY_LOOP(); }
}

void initSysPLL() {
  SYS_PLL pll;
  Resets  resets;
  resets.reset.resetPLLSYS(false);
}

void initTicks() {
  // p569: SDK expects nominal 1uS system ticks, as does Arm internals.
  // Although we don't use the SDK we'll assume 1uS everywhere as well.
  Ticks  ticks;
  Resets resets;

  resets.reset.resetTIMER0(false);
  while (!resets.resetDone.resetDoneTIMER0()) { _BUSY_LOOP(); }

  ticks.proc0Control().enable(0); // disable for config
  ticks.proc0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
  ticks.proc0Control().enable(1);
  while (!ticks.proc0Control().running()) { _BUSY_LOOP(); }

  ticks.timer0Control().enable(0); // disable for config
  ticks.timer0Cycles().count(12);  // 12 clocks @ 12 MHz xtal -> 1 µs
  ticks.timer0Control().enable(1);
  while (!ticks.timer0Control().running()) { _BUSY_LOOP(); }
}

struct Clocks {
  constexpr static u32 kBase = 0x40010000;

  Reg32 refControl() { return {kBase + 0x30}; }
  Reg32 refDiv() { return {kBase + 0x34}; }
  Reg32 refSelected() { return {kBase + 0x38}; }

  Reg32 sysControl() { return {kBase + 0x3c}; }
  Reg32 sysDiv() { return {kBase + 0x40}; }
  Reg32 sysSelected() { return {kBase + 0x44}; }
};

// https://github.com/igormichalak/bare-metal-rp2350/blob/master/prelude.s

void initClockRef() {
  Clocks c;
  c.refControl().set(1, 0, 2);  // SRC <- XOSC_CLKSRC
  c.refSelected().set(3, 0, 4); // select bit 2
  while (c.refSelected().get(3, 0) != 4) { ArmInsns::nop(); }
}

void initClockSys() {
  Clocks c;
  c.sysControl().set(1, 0, 1);  // SRC <- CLKSRC_CLK_REF_AUX
  c.sysSelected().set(3, 0, 2); // select bit 1
  while (c.sysSelected().get(3, 0) != 2) { ArmInsns::nop(); }
}

// struct Global {
//   static void initMSecTimer() {
//     millis = 0;

//     Handlers::handlers[0] = timer0Callback;
//     // timer0Callback(); // schedules alarm and (re-)enables
//     // Timers().timer0().intEnable().bit(0, true);
//   }

//   static void init() {
//     initGPIOOut(kPicoLED);
//     initMSecTimer();
//     __enable_irq();
//   }
// };

// void sleepMS(u32 ms) {
// }

} // namespace init

[[gnu::used]] [[gnu::retain]] [[noreturn]] void Handlers::reset() {
  init::clearBSS();

  SIO sio;
  initGPIOOut(kPicoLED);
  initGPIOOut(kPanicUARTTX);
  sio.gpioOut().bit(kPanicUARTTX, true);
  sio.gpioOut().bit(kPicoLED, true);

  Resets {}.reset.resetIOBANK0(false).resetPADSBANK0(false);
  // init::initXOSC();
  // init::initSysPLL();
  // init::initTicks();
  // init::initClockRef();
  // init::initClockSys();
  // Reg32 {0xaaaaaaaa}.bit(0, 0);

  // Resets resets;
  // resets.reset.resetTIMER0(false);
  // while (!resets.resetDone.resetDoneTIMER0()) { _BUSY_LOOP(); }

  // Ticks ticks;
  // ticks.timer0Cycles().set(8, 0, 12);
  // ticks.timer0Control().set(0, 0, true);

  // auto t0 = Timers::timer0();

  start();
  __builtin_unreachable();
}
