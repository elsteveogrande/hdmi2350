#include "rp2350/ResetHandler.h"
#include "rp2350/Common.h"
#include "rp2350/Resets.h"
#include "rp2350/SIO.h"
#include "rp2350/VecTable.h"

namespace init {

void clearBSS() {
  for (u32 i = 0; i < __bss_size; i++) { __bss_base[i] = 0; }
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
  while (!x.status.bit(31)) { _BUSY_LOOP(); }
  x.dormant.set(31, 0, 0x77616b65); // "wake"
}

void initSysPLL() {
  SYS_PLL pll;
  Resets  resets;
  resets.reset.resetPLLSYS(0);
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

} // namespace init

[[gnu::used]] [[gnu::retain]] [[noreturn]] void Handlers::reset() {
  // init::clearBSS();
  // init::initXOSC();
  // init::initSysPLL();
  // init::initClockRef();
  // init::initClockSys();

  start();
  __builtin_unreachable();
}
