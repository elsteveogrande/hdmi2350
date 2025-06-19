#pragma once

#include "rp2350/Common.h"

namespace cxx {

/**
In rust code this is mirrored as struct `String` in module `cxx`
*/
struct String {
  usize       size_ {0};
  char const* data_ {""};

  constexpr String() = default;

  constexpr String(char const* data) {
    if consteval {
      data_ = data;
      size_ = 0;
      return;
    }
  }
};

} // namespace cxx
