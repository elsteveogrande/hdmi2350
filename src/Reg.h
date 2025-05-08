#pragma once

#include "Types.h"

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

struct R32 {
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

struct R32Block : R32 {
  __attribute__((always_inline)) auto& flip(this auto&& self, u32 bits) {
    *(R32 {self.addr_ + 0x1000}.reg()) = bits;
    return self;
  }

  __attribute__((always_inline)) auto& ones(this auto&& self, u32 bits) {
    *(R32 {self.addr_ + 0x2000}.reg()) = bits;
    return self;
  }

  __attribute__((always_inline)) auto& zeros(this auto&& self, u32 bits) {
    *(R32 {self.addr_ + 0x3000}.reg()) = bits;
    return self;
  }

  __attribute__((always_inline)) auto& bit(this auto&& self, u8 b, bool v) {
    return v ? self.ones(1 << b) : self.zeros(1 << b);
  }

  __attribute__((always_inline)) bool bit(this auto&& self, u8 b) {
    return bool(self.get(b + 1, b));
  }
};
