#include "../GPIO0.h"
#include "../Pads.h"
#include "../SIO.h"
#include "../Util.h"
#include "init.h"

namespace sys {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset() {
  GPIO0 bank0;
  bank0.control(25).funcSel(GPIO0::Control::FuncSel::SIO);

  Pads::UserBank bank;
  bank.gpio(25)
      .drive(Pads::UserBank::Drive::k12mA)
      .pullDownEnable(false)
      .pullUpEnable(true)
      .schmitt(false)
      .isolation(0);

  SIO sio;
  sio.gpioOutEnbSet.bit(25, true);
  sio.gpioOutSet.bit(25, true);

  while (true) { _BUSY_LOOP(); }

  // ::hdmi::run();

  // main `run` function should not return.  If it did, just reset.
  // reset(); // tail call to self
}

} // namespace sys