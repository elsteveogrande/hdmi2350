#include "rp2350/Clocks.h"
#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/SIO.h"
#include "rp2350/Ticks.h"

[[gnu::used]] [[gnu::retain]] void rp2350Init() {

  XOSC xosc;
  xosc.init();

  SysPLL sysPLL;
  sysPLL.reset();
  sysPLL.init();

  Clocks c;

  c.sys.control()
      .auxSource(Clocks::SysControlStruct::AuxSource::PLL_SYS)
      .clkSource(Clocks::SysControlStruct::ClkSource::CLK_SYS_AUX)
      .enable(true);
  c.sys.div().set(31, 16, 1).set(15, 0, 0); // divide by 1.0

  c.ref.control().clkSource(Clocks::RefControlStruct::ClkSource::XOSC);
  c.ref.div().set(31, 16, 1).set(15, 0, 0); // divide by 1.0

  c.peri.control().auxSource(Clocks::PeriControlStruct::AuxSource::XOSC).enable(true);
  c.peri.div().set(31, 16, 1).set(15, 0, 0); // divide by 1.0

  // p569: SDK expects nominal 1uS system ticks, as does Arm internals.
  // Although we don't use the SDK we'll assume 1uS everywhere as well.

  Ticks ticks;
  ticks.proc0Control().enable(0); // disable for config
  ticks.proc0Cycles().count(12);
  ticks.proc0Control().enable(1);
  while (!ticks.proc0Control().running()) { _BUSY_LOOP(); }
  ticks.proc1Control().enable(0); // disable for config
  ticks.proc1Cycles().count(12);
  ticks.proc1Control().enable(1);
  while (!ticks.proc1Control().running()) { _BUSY_LOOP(); }

  M33 m33;
  m33.ccr.div0Trap(true);
  m33.ccr.unalignedTrap(true);

  m33.sysTick.rvr.set(1000);
  m33.sysTick.csr.enable(1).tickInt(1);
}
