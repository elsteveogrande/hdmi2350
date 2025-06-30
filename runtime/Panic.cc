#include "runtime/Panic.h"
#include "rp2350/Common.h"
#include "rp2350/M33.h"
#include "rp2350/SIO.h"
#include "runtime/RuntimeData.h"

namespace {

struct PanicTX {
  [[gnu::noinline]] void baudDelay() {
    constexpr u32 kLoopsPerBaud = kSysPLLMHz / (kPanicBaud * 9);
    // TODO: should asm volatile this
    for (u32 i = 0; i < kLoopsPerBaud; i++) { asm volatile("nop"); }
  }

  void bit(bool v) {
    SIO sio;
    sio.gpioOut().bit(kPanicUARTTX, v);
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

[[gnu::used]] [[gnu::noinline]] [[gnu::aligned(16)]] [[noreturn]]
void __panic(PanicData const& pd) {

  M33 m33;

  // Disable all interrupts immediately
  // m33.nvic.cer0.set(31, 0, 0xffffffff);

  constexpr static char const* RED    = "\x1b[0;41;1;37m";
  constexpr static char const* YEL    = "\x1b[1;33;48;5;236m";
  constexpr static char const* NORMAL = "\x1b[0m";

  PanicTX tx;
  for (u32 i = 0; i < 16; i++) { tx << '\n'; }

  auto& rd = runtimeData;

  tx << RED << "=== panic @ " << rd.millis << NORMAL << "ms\n";
  tx << "  r0:" << pd.r0 << "  r1:" << pd.r1 << "  r2:" << pd.r2 << "  r3:" << pd.r3 << '\n'
     << "  r4:" << pd.r4 << "  r5:" << pd.r5 << "  r6:" << pd.r6 << "  r7:" << pd.r7 << '\n'
     << "  r8:" << pd.r8 << "  r9:" << pd.r9 << " r10:" << pd.r10 << " r11:" << pd.r11 << '\n'
     << " r12:" << pd.r12 << "  sp:" << pd.sp << "  lr:" << pd.lr << "  pc:" << pd.pc << '\n'
     << " psr:" << pd.psr << " exc:" << pd.exc << "  type:" << pd.type << '\n';

  tx << "\nM33 SCB:\n";
  tx << "  ACTLR:" << m33.ACTLR.val();
  tx << "  CPUID:" << m33.CPUID.val();
  tx << "   ICSR:" << m33.icsr.val();
  tx << "   VTOR:" << m33.VTOR.val();
  tx << "  AIRCR:" << m33.AIRCR.val() << '\n';
  tx << "    SCR:" << m33.SCR.val();
  tx << "    CCR:" << m33.ccr.val();
  tx << "  SHPR1:" << m33.SHPR1.val();
  tx << "  SHPR2:" << m33.SHPR2.val();
  tx << "  SHPR3:" << m33.SHPR3.val() << '\n';
  tx << "  SHCSR:" << m33.SHCSR.val();
  tx << "  CPACR:" << m33.CPACR.val();
  tx << "  NSACR:" << m33.NSACR.val() << '\n';
  tx << YEL;
  u32 sr;

  sr = m33.HFSR.val();
  tx << "   HFSR:" << sr;
  if (sr & 0x80000000) { tx << " DEBUGEVT"; }
  if (sr & 0x40000000) { tx << " FORCED"; }
  if (sr & 0x00000002) { tx << " VECTTBL"; }
  tx << '\n';

  auto cfsr = m33.CFSR.val();
  sr        = cfsr & 0xff;
  tx << "   MFSR:" << sr;
  if (!(sr & 0x80)) { tx << " !MMARVALID"; }
  if (sr & 0x20) { tx << " MLSPERR"; }
  if (sr & 0x10) { tx << " MSTKERR"; }
  if (sr & 0x08) { tx << " MUNSTKERR"; }
  if (sr & 0x02) { tx << " DACCVIOL"; }
  if (sr & 0x01) { tx << " IACCVIOL"; }
  tx << '\n';
  if (sr ^ 0x80) { tx << "  MMFAR:" << m33.MMFAR.get(31, 0) << '\n'; }

  sr = (cfsr >> 8) & 0xff;
  tx << "   BFSR:" << sr;
  if (!(sr & 0x80)) { tx << " !BFARVALID"; }
  if (sr & 0x20) { tx << " LSPERR"; }
  if (sr & 0x10) { tx << " STKERR"; }
  if (sr & 0x08) { tx << " UNSTKERR"; }
  if (sr & 0x02) { tx << " PRECISERR"; }
  if (sr & 0x01) { tx << " IBUSERR"; }
  tx << '\n';
  if (sr ^ 0x80) { tx << "   BFAR:" << m33.BFAR.get(31, 0) << '\n'; }

  sr = cfsr >> 16;
  tx << "   UFSR:" << sr;
  if (sr & 0x0200) { tx << " DIVBYZERO"; }
  if (sr & 0x0100) { tx << " UNALIGNED"; }
  if (sr & 0x0010) { tx << " STKOF"; }
  if (sr & 0x0008) { tx << " NOCP"; }
  if (sr & 0x0004) { tx << " INVPC"; }
  if (sr & 0x0002) { tx << " INVSTATE"; }
  if (sr & 0x0001) { tx << " UNDEFINSTR"; }
  tx << NORMAL;
  tx << '\n';

  Reg32 bork {u32(0x400d800c)};
  if (bork.val()) { tx << RED << "BORK " << bork.val() << NORMAL << '\n'; }

  SIO sio;
  u32 i = 0;
  while (true) { sio.gpioOut().bit(kPicoLED, (i++ >> 21) & 1); };

  __builtin_unreachable();
}

} // namespace cxx
