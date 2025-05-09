#include "../GPIO0.h"
#include "../Pads.h"
#include "../SIO.h"
#include "init.h"

extern "C" {
extern void _main();
}

namespace sys {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset() {

  GPIO0 bank0;
  bank0.control(25).funcSel(GPIO0::Control::FuncSel::SIO);

  Pads::UserBank bank;
  bank.gpio(25)
      .drive(Pads::UserBank::Drive::k12mA)
      .pullDownEnable(false)
      .pullUpEnable(false)
      .schmitt(false)
      .slewfast(true)
      .isolation(0);

  SIO sio;
  sio.gpioOutEnbSet.bit(25, true);
  sio.gpioOutSet.bit(25, false);

  _main();

  __builtin_unreachable();
}

} // namespace sys