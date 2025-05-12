#include "Util.h"
#include "rp2350/GPIO.h"
#include "rp2350/Pads.h"
#include "rp2350/SIO.h"

void initGPIO(u8 index) {
  GPIO gpio;
  gpio.control(index).funcSel(GPIO::Control::FuncSel::SIO);

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
