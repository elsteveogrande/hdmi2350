#pragma once

#include "rp2350/Common.h"

extern "C" {

// C/C++ ABI-specified functions

void __aeabi_memcpy(u8* dest, const u8* src, unsigned n);
void __aeabi_memcpy4(u8* dest, const u8* src, unsigned n);
}
