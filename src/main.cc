#include "SIO.h"
#include "Util.h"

namespace hdmi {

[[gnu::used]] [[gnu::retain]] [[noreturn]] void run() {
  SIO sio;
  u32 counter = 0;
  while (true) {
    sio.gpioOut.bit(25, (counter & (1 << 15)));
    counter = counter + 1;
    _BUSY_LOOP();
  }
}

} // namespace hdmi

extern "C" {
[[noreturn]] void _main() { hdmi::run(); }
}
