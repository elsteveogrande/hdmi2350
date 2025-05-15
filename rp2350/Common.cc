#include "RP2350/Common.h"
#include "RP2350/GPIO.h"
#include "RP2350/Pads.h"
#include "RP2350/SIO.h"

void initGPIOOut(u8 index) {
  GPIO gpio;
  gpio.control(index).funcSel(GPIO::Control::FuncSel::SIO);

  Pads::UserBank bank;
  bank.gpio(index)
      .update()
      .drive(Pads::UserBank::Drive::k12mA)
      .outputDisable(false)
      .pullDownEnable(false)
      .pullUpEnable(false)
      .schmitt(false)
      .slewfast(true)
      .isolation(0);

  SIO sio;
  sio.gpioOutEnbSet.bit(index, true);
  sio.gpioOutSet.bit(index, false);
}

void initGPIOIn(u8 index, bool pullUp, bool schmitt) {
  GPIO gpio;
  gpio.control(index).funcSel(GPIO::Control::FuncSel::SIO);

  Pads::UserBank bank;
  bank.gpio(index)
      .update()
      .inputEnable(true)
      .pullDownEnable(!pullUp)
      .pullUpEnable(pullUp)
      .schmitt(schmitt)
      .slewfast(true)
      .isolation(0);

  SIO sio;
  sio.gpioOutEnbSet.bit(index, false);
}

void initHSTX(u8 index) {
  GPIO gpio;
  gpio.control(index).funcSel(GPIO::Control::FuncSel::HSTX);

  Pads::UserBank bank;
  bank.gpio(index)
      .update()
      .drive(Pads::UserBank::Drive::k12mA)
      .outputDisable(false)
      .pullDownEnable(false)
      .pullUpEnable(false)
      .bit(1, false)
      .schmitt(false)
      .slewfast(true)
      .isolation(0);
}
