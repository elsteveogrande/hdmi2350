#include "rp2350/Common.h"
#include "rp2350/VecTable.h"
#include "runtime/RuntimeData.h"

[[gnu::aligned(16)]] void __attribute__((interrupt)) Handlers::sysTick() {
  ++runtimeData.millis;
  Reg32 {0xcccccccc}.bit(0, 0x1111aaaa);
}
