#include "RP2350/Common.h"
#include "RP2350/GPIO.h"
#include "RP2350/Pads.h"
#include "RP2350/SIO.h"

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
