#include "Util.h"
#include "rp2350/GPIO0.h"
#include "rp2350/Pads.h"
#include "rp2350/SIO.h"

void initGPIO(u8 index) {
  GPIO0 bank0;
  bank0.control(index).funcSel(GPIO0::Control::FuncSel::SIO);

  Pads::UserBank bank;
  bank.gpio(index)
      .drive(Pads::UserBank::Drive::k12mA)
      .pullDownEnable(false)
      .pullUpEnable(false)
      .schmitt(false)
      .slewfast(true)
      .isolation(0);

  SIO sio;
  sio.gpioOutEnbSet.bit(index, true);
  sio.gpioOutSet.bit(index, false);
}
