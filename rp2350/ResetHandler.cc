#include "rp2350/ResetHandler.h"
#include "rp2350/SIO.h"
#include "rp2350/VecTable.h"

[[gnu::used]] [[gnu::retain]] void rp2350Init();
[[gnu::used]] [[gnu::retain]] void runtimeInit();

[[gnu::used]] [[gnu::retain]] [[noreturn]] void start();

void basicInit() {
  auto* bss = (u8*)__bss_base;
  for (u32 i = 0; i < __bss_size; i++) { bss[i] = 0; }

  SIO sio;
  initGPIOOut(kPicoLED);
  initGPIOOut(kPanicUARTTX);
  sio.gpioOut().bit(kPanicUARTTX, true);
  sio.gpioOut().bit(kPicoLED, true);
}

[[gnu::used]] [[gnu::retain]] [[noreturn]] void Handlers::reset() {
  basicInit();
  rp2350Init();
  runtimeInit();

  start();

  __builtin_unreachable();
}
