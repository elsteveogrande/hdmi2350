#include "RP2350/Common.h"
#include "RP2350/SIO.h"

namespace hdmi {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void run() {
  initGPIO(kPicoLED);

  SIO sio;
  u32 counter = 0;
  while (true) {
    sio.gpioOut.bit(kPicoLED, (counter & (1 << 15)));
    counter = counter + 1;
    _BUSY_LOOP();
  }
}

} // namespace hdmi

extern "C" {
[[noreturn]] void _main() { hdmi::run(); }
}
