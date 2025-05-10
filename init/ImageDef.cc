#include "../base/Types.h"
#include "init.h"

namespace sys {

/*
Image definition: section 5.9, "Metadata Block Details".

This contains the IMAGE_DEF, a binary blob the ROM bootloader uses
to identify and validate program images in flash.

5.9.1. Blocks And block loops:
"Blocks consist of a fixed 32-bit header, one or more items, a 32-bit
relative offset to the next block, and a fixed 32-bit footer."

In this implementation:
* we only have one block, so `link` will be zero.
* `size` can be one or two bytes (specified by a flag); but nothing below
  will be more than 255 bytes so we'll just use a "size size" of 1,
  even for the `LAST_ITEM` (which contains the total size of prior items).
*/

[[gnu::section(".init_vec_table")]] [[gnu::used]] [[gnu::retain]] constexpr struct {
  u32 const sp {0x20000400};
  void (*pc)() {&sys::reset};
} initialVecTable;

[[gnu::section(".image_def")]] [[gnu::used]] [[gnu::retain]] constexpr struct [[gnu::packed]] {
  struct [[gnu::packed]] StartMarker {
    u32 const val = 0xffffded3;
  };

  struct [[gnu::packed]] EndMarker {
    u32 const val = 0xab123579;
  };

  /**  5.9.3.1, "IMAGE_DEF item" */
  struct [[gnu::packed]] ImageDef {
    u8 const  type {0x42};
    u8 const  size {0x01};
    u16 const flags {};
  };

  // /** 5.9.3.4, "ENTRY_POINT item" */
  // struct [[gnu::packed]] EntryPoint {
  //   u8 const  type {0x44};
  //   u8 const  size {0x04}; // 4 words
  //   u16 const _pad {};
  //   u32 const sp {0x20000100};
  //   void (*pc)() {};
  //   u32 const spLimit {0x20000000};
  // };

  struct [[gnu::packed]] LastItem {
    u8  type {0xff};  // BLOCK_ITEM_LAST has a 2-byte size
    u16 totalSize {}; // Total of preceding items' sizes (in words)
    u8  _pad {};
  };

  // Start laying out the IMAGE_DEF bytes

  StartMarker start {};                // Start magic
  ImageDef    image {.flags = 0x1021}; // Item 0: CHIP=2350, CPU=ARM, EXE=1, S=2
  // EntryPoint  entry {.pc = sys::reset};  // Initial SP and PC locations
  LastItem    lastItem {.totalSize = 1}; // Total of preceding items' sizes
  u32         link {0};                  // Single-block loop, so there's no link
  EndMarker   end {};                    // End magic
} imageDefARM;

} // namespace sys
