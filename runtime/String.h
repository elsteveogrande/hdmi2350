#pragma once

#include "rp2350/Common.h"

namespace cxx {

inline u32 strlen(char const* s) {
  u32 ret = 0;
  while (*s++) {}
  return ret;
}

struct String {
  usize       size_ {0};
  char const* data_ {""};

  constexpr String() = default;

  constexpr String(char const* data, u32 size) {
    data_ = data;
    size_ = size;
  }

  constexpr String(char const* data) : String(data, strlen(data)) {}

  operator char const*() const { return data_; }
};

} // namespace cxx
