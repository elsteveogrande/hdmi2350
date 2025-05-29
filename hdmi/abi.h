#pragma once

#include "RP2350/Common.h"

extern "C" {

// C/C++ ABI-specified functions

void __aeabi_memcpy(u8* dest, const u8* src, unsigned n) {
  for (unsigned i = 0; i < n; i++) {
    // Read from source and write into dest; the do-nothing `asm volatile`
    // is only to separate the read and write, to prevent fusing them and optimizing
    // into a "memcpy" operation involving a call to `__aeabi_memcpy`, the very thing
    // we're trying to define.
    u8 x    = src[i];
    // asm volatile("");  // XXX actually needed??
    dest[i] = x;
  }
}

void __aeabi_memcpy4(u8* dest, const u8* src, unsigned n) {
  [[assume(!(u32(dest) & 3))]];
  [[assume(!(u32(src) & 3))]];
  __aeabi_memcpy(dest, src, n);
}
}
