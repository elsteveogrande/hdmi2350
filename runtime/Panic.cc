#include "runtime/Panic.h"
#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/SIO.h"

namespace {

struct PanicTX {
  [[gnu::noinline]] void baudDelay() {
    // Empirically (i.e. not thoroughly tested or calibrated), delay about 1/9600 sec
    for (u32 i = 0; i < 400; i++) { asm volatile("nop"); }
  }

  void bit(bool v) {
    SIO sio;
    sio.gpioOut().bit(kPicoLED, v).bit(kPanicUARTTX, v);
    baudDelay();
  }

  void byte(u8 v) {
    u32 x = u32(7 << 9) | (u32(v) << 1);
    for (int i = 0; i < 12; i++, x >>= 1) { bit(x & 1); }
  }

  friend PanicTX& operator<<(PanicTX& tx, char v) {
    if (v == '\n') { tx << '\r'; }
    tx.byte(v);
    return tx;
  }

  friend PanicTX& operator<<(PanicTX& tx, char const* s) {
    while (*s) { tx << *(s++); }
    return tx;
  }

  friend PanicTX& operator<<(PanicTX& tx, u32 x) {
    constexpr static char const* kTable = "0123456789abcdef";
    for (u32 i = 0; i < 8; i++) {
      tx << kTable[0x0f & (x >> 28)];
      x <<= 4;
    }
    return tx;
  }
};

} // namespace

namespace cxx {

[[gnu::used]] [[gnu::noinline]] [[noreturn]]
void __panic(PanicData const& pd) {

  // Disable all interrupts immediately
  M33 m33;
  m33.nvicICER0.set(31, 0, 0xffffffff);

  PanicTX tx;
  for (u32 i = 0; i < 16; i++) { tx << '\n'; }
  tx << "\x1b[0;41;1;37m=== panic\x1b[0m\n";
  tx << "  r0:" << pd.r0 << "  r1:" << pd.r1 << "  r2:" << pd.r2 << "  r3:" << pd.r3 << '\n'
     << "  r4:" << pd.r4 << "  r5:" << pd.r5 << "  r6:" << pd.r6 << "  r7:" << pd.r7 << '\n'
     << "  r8:" << pd.r8 << "  r9:" << pd.r9 << " r10:" << pd.r10 << " r11:" << pd.r11 << '\n'
     << " r12:" << pd.r12 << "  sp:" << pd.sp << "  lr:" << pd.lr << "  pc:" << pd.pc << '\n'
     << " psr:" << pd.psr << " exc:" << pd.exc << "  type:" << pd.type << '\n';

  SIO sio;
  u32 i = 0;
  while (true) { sio.gpioOut().bit(kPicoLED, (i++ >> 19) & 1); };
  __builtin_unreachable();
}

} // namespace cxx
