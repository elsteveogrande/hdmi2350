#include "../GPIO0.h"
#include "../Resets.h"
#include "../SIO.h"
#include "init.h"

namespace sys {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void reset() {
  Resets resets;
  auto   ioBank0Bit = u8(Resets::Periph::kIOBANK0); // IO_BANK0 is bit 5
  resets.resetControl.bit(ioBank0Bit, false);       // Clear this bit to take out of reset
  while (!resets.resetStatus.bit(ioBank0Bit)) {}    // Wait for reset to finish

  GPIO0 bank0;
  bank0.control(25).funcSel(GPIO0::Control::FuncSel::SIO);

  SIO sio;
  sio.gpioOutEnableSet.bit(25, true);
  sio.gpioOutSet.bit(25, true);

  ::hdmi::run();
}

} // namespace sys