#pragma once

#include "rp2350/Common.h"

struct [[gnu::aligned(64)]] RuntimeData {
  u32 millis {};
};

extern RuntimeData runtimeData;
