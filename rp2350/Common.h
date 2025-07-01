#pragma once

using u8  = unsigned char;
using u16 = unsigned short;
using u32 = unsigned long;
using u64 = unsigned long long;

static_assert(sizeof(u8) == 1);
static_assert(sizeof(u16) == 2);
static_assert(sizeof(u32) == 4);
static_assert(sizeof(u64) == 8);

using usize = u32;
static_assert(sizeof(usize) == sizeof(void*));

namespace {

[[gnu::pure]] __attribute__((always_inline)) constexpr u32 _u32_mask(auto hi, auto lo) {
  u32 ret {0};
  if (hi > 0) {
    ret = (hi == 32) ? ~0U : ((1U << hi) - 1);
    if (lo > 0) { ret ^= _u32_mask(lo, 0); }
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
    return (_u32_mask(hi, lo) & this->val()) >> lo;
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
    if (reg_) { reg_->val() = val; }
  }
};

struct Reg32Structs {
  // clang-format off
  // Boilerplate for R, U classes
  struct U;
  struct R : Reg<R, U> { R(auto addr) : Reg(addr) {} };
  struct U : Update<U, R> { U(auto* reg) : Update(reg) {} };
  // clang-format on
};
using Reg32 = Reg32Structs::R;

#if defined(__arm__)
struct ArmInsns {
  static void nop() { asm volatile("nop"); }
  static void wfi() { asm volatile("wfi"); }
  static void wfe() { asm volatile("wfe"); }
  static u8   currentInterrupt() { return u8(ipsr() & 0x1ff); }
  static void disableIRQs() { cpsid(); }
  static void enableIRQs() { cpsie(); }

  // ARM Specifics

  static void cpsid() { asm volatile("cpsid i"); }
  static void cpsie() { asm volatile("cpsie i"); }

  /** Bits 0..8: Interrupt number; IRQ numbers start at 16 (IRQ0 is interrupt 16, etc.) */
  static u32 ipsr() {
    u32 ret;
    asm volatile("mrs %0, ipsr" : "=r"(ret));
    return ret;
  }
};

using Insns = ArmInsns;

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
  Insns::nop();
#endif
}

// clang-format off
#define _BUSY_LOOP() { do { _busy_loop(__FILE__, __LINE__); } while(false); }
// clang-format on

void initGPIOOut(u8 index);
void initGPIOIn(u8 index, bool pullUp = false, bool schmitt = true);
void initHSTX(u8 index);

// Random assorted stuff.
// XXX Move these
void convU32Hex(char* out, u32 x);

constexpr u8 kPicoLED     = 25; // Pico2-specific: LED GPIO is number 25
constexpr u8 kPanicUARTTX = 0;  // Bit-bang panic info (9600 8N1); see Panic.cc

constexpr u32 kSysPLLMHz = 150000000;
constexpr u32 kPanicBaud = 9600;
