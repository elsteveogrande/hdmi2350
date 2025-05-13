#include "RP2350/Common.h"
#include "RP2350/SIO.h"

namespace hdmi {

u8 getSwitch() { return SIO().gpioIn.bit(0); }

[[gnu::used]] [[gnu::retain]] [[noreturn]] void run() {
  initGPIOOut(kPicoLED);
  initGPIOIn(0, true);

  SIO sio;
  u32 counter = 0;
  while (true) {
    if (getSwitch()) {
      sio.gpioOut.bit(kPicoLED, (counter & (1 << 14)));
    }
    counter = counter + 1;
    _BUSY_LOOP();
  }
}

} // namespace hdmi

extern "C" {
[[noreturn]] void start() { hdmi::run(); }
}
