#include "Util.h"
#include "../hardware/GPIO0.h"
#include "../hardware/Pads.h"
#include "../hardware/SIO.h"

/** Initialize the LED GPIO (number 25) */
void initLED() {
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
}
