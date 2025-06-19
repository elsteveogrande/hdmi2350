#include "rp2350/Common.h"
#include "rp2350/GPIO.h"
#include "rp2350/Pads.h"
#include "rp2350/SIO.h"

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
  sio.gpioOutEnbSet().bit(index, true);
  sio.gpioOutSet().bit(index, false);
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
  sio.gpioOutEnbSet().bit(index, false);
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

void convU32Hex(char* out, u32 x) {
  constexpr static char const hexTable[17] = "0123456789abcdef";
  for (u8 i = 0; i < 8; i++) { *(out + 7 - i) = hexTable[x & 0x0f]; }
}
