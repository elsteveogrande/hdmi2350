#pragma once

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned long;
using u64 = unsigned long long;

static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

/** These are declared extern here but the addresses are actually defined in the linker script. */
extern void* __vec_table;
extern void* __stack_top;

namespace {

[[gnu::pure]] __attribute__((always_inline)) constexpr u32 _u32_mask(auto hi, auto lo) {
  u32 ret {0};
  if (hi > 0) {
    ret = (hi == 32) ? ~0U : ((1U << hi) - 1);
    if (lo > 0) {
      ret ^= _u32_mask(lo, 0);
    }
  }
  return ret;
}

static_assert(_u32_mask(32, 0) == 0xffffffff);
static_assert(_u32_mask(31, 0) == 0x7fffffff);
static_assert(_u32_mask(32, 1) == 0xfffffffe);
static_assert(_u32_mask(31, 1) == 0x7ffffffe);
static_assert(_u32_mask(32, 16) == 0xffff0000);
static_assert(_u32_mask(16, 8) == 0x0000ff00);
static_assert(_u32_mask(16, 0) == 0x0000ffff);
static_assert(_u32_mask(8, 0) == 0x000000ff);

} // namespace

struct Reg {
  u32 const addr_;

  constexpr u32 volatile* reg() const { return (u32 volatile*)(addr_); }

  __attribute__((always_inline)) u32 get(this auto&& self, u8 hi, u8 lo) {
    return (_u32_mask(hi, lo) & *self.reg()) >> lo;
  }

  __attribute__((always_inline)) auto& set(this auto&& self, u32 z) {
    *self.reg() = z;
    return self;
  }

  __attribute__((always_inline)) auto& set(this auto&& self, u8 hi, u8 lo, u32 z) {
    *self.reg() = ((~_u32_mask(hi, lo)) & *self.reg()) | (_u32_mask(hi, lo) & (z << lo));
    return self;
  }

  __attribute__((always_inline)) bool bit(this auto&& self, u8 b) {
    return bool(self.get(b + 1, b));
  }

  __attribute__((always_inline)) auto& bit(this auto&& self, u8 b, bool v) {
    return self.set(b + 1, b, v);
  }
};

struct RegBlock : Reg {
  __attribute__((always_inline)) auto& flip(this auto&& self, u32 bits) {
    *(Reg {self.addr_ + 0x1000}.reg()) = bits;
    return self;
  }

  __attribute__((always_inline)) auto& ones(this auto&& self, u32 bits) {
    *(Reg {self.addr_ + 0x2000}.reg()) = bits;
    return self;
  }

  __attribute__((always_inline)) auto& zeros(this auto&& self, u32 bits) {
    *(Reg {self.addr_ + 0x3000}.reg()) = bits;
    return self;
  }

  __attribute__((always_inline)) auto& bit(this auto&& self, u8 b, bool v) {
    return v ? self.ones(1 << b) : self.zeros(1 << b);
  }

  __attribute__((always_inline)) bool bit(this auto&& self, u8 b) {
    return bool(self.get(b + 1, b));
  }
};

#if defined(__arm__)
struct ArmInsns {
  static void nop() { asm volatile("nop"); }
  static void wfi() { asm volatile("wfi"); }
  static void wfe() { asm volatile("wfe"); }
};
#endif

/**
Use like: `_busy_loop(__FILE__, __LINE__);`.
TODO: actually record where the busy loop happens, for diagnostics;
TODO: later, make a proper stacktrace object instead.
*/
inline void _busy_loop(char const* file = nullptr, unsigned line = 0) {
  (void)file;
  (void)line;
#if defined(__arm__)
  ArmInsns::nop();
#endif
}

// clang-format off
#define _BUSY_LOOP() { do { _busy_loop(__FILE__, __LINE__); } while(false); }
// clang-format on

/** Pico2-specific: LED GPIO is number 25 */
constexpr u8 kPicoLED = 25;

void initGPIOOut(u8 index);
void initGPIOIn(u8 index, bool pullUp = false, bool schmitt = true);
void initHSTX(u8 index);
