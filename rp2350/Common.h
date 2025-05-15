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

template <typename W, typename T> struct Word {
  u32 const addr_ {};

  T* addr() const { return (T*)addr_; }
  T& val() const { return *(addr()); }

  __attribute__((always_inline)) u32 get(u8 hi, u8 lo) const {
    return (_u32_mask(hi, lo) & *this->val()) >> lo;
  }

  __attribute__((always_inline)) auto& set(u32 z) const {
    this->val() = z;
    return (W const&)*this;
  }

  __attribute__((always_inline)) auto& set(u8 hi, u8 lo, u32 z) const {
    this->val() = ((~_u32_mask(hi, lo)) & this->val()) | (_u32_mask(hi, lo) & (z << lo));
    return (W const&)*this;
  }

  __attribute__((always_inline)) bool bit(u8 i) const {
    u32 m = 1 << i;
    return this->val() & m;
  }

  __attribute__((always_inline)) auto& bit(u8 i, bool v) const {
    u32 m = 1 << i;
    if (v) {
      this->val() |= m;
    } else {
      this->val() &= ~m;
    }
    return (W const&)*this;
  }
};

template <typename U, typename R> struct Update;

template <typename R, typename U> struct Reg : Word<R, u32 volatile> {
  explicit constexpr Reg(u32 addr) : Word<R, u32 volatile>(addr) {}
  U update() { return U {this}; }
};

template <typename U, typename R> struct Update : Word<U, u32> {
  u32        val {};
  Reg<R, U>* reg_ {};

  Update()                         = default;
  Update(Update const&)            = delete;
  Update& operator=(Update const&) = delete;
  explicit Update(auto* reg) : Word<U, u32>(u32(&val)), val(reg->val()), reg_(reg) {}

  Update(Update&& rhs) {
    val      = rhs.val;
    reg_     = rhs.reg_;
    rhs.reg_ = nullptr;
  }

  ~Update() {
    if (reg_) {
      reg_->val() = val;
    }
  }
};

struct Reg32Structs {
  // clang-format off
  // Boilerplate for R, U classes
  struct U;
  struct R : Reg<R, U> { explicit constexpr R(auto addr) : Reg(addr) {} };
  struct U : Update<U, R> { explicit U(auto* reg) : Update(reg) {} };
  // clang-format on
};
using Reg32 = Reg32Structs::R;

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
