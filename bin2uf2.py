#!/usr/bin/env python3

import sys
from os import SEEK_SET, SEEK_END
from pathlib import Path
from struct import pack

"""
// Condensed UF2 structure, see original in pico-sdk:
// `src/common/boot_uf2_headers/include/boot/uf2.h`
#define UF2_MAGIC_START0            0x0A324655u
#define UF2_MAGIC_START1            0x9E5D5157u
#define UF2_MAGIC_END               0x0AB16F30u
#define UF2_FLAG_NOT_MAIN_FLASH     0x00000001u
#define UF2_FLAG_FILE_CONTAINER     0x00001000u
#define UF2_FLAG_FAMILY_ID_PRESENT  0x00002000u  // instead of file_size
#define UF2_FLAG_MD5_PRESENT        0x00004000u
#define RP2040_FAMILY_ID            0xe48bff56u
struct uf2_block {
    uint32_t magic_start0, magic_start1;    // magics
    uint32_t flags;                         // usu. 0x00002000
    uint32_t target_addr;                   // starts at 0x10000000 for ROM
    uint32_t payload_size;                  // usu. 256
    uint32_t block_no, num_blocks;          // block_no is in [0, num_blocks]
    uint32_t file_size_OR_familyID;         // usu. RP2040_FAMILY_ID
    uint8_t  data[476];                     // (payload_size) followed by 0's
    uint32_t magic_end;
};

Defined at: https://github.com/microsoft/uf2
"""

MAGIC_START0        = 0x0A324655
MAGIC_START1        = 0x9E5D5157
MAGIC_END           = 0x0AB16F30
RP2040_FAMILY_ID    = 0xe48bff56
FLAG_FAMILY_ID      = 0x00002000
TARGET              = 0x10000000


def bin2uf2(bin_path: Path, uf2_path: Path) -> None:
    with (open(bin_path, "rb") as bin,
          open(uf2_path, "wb") as uf2):

        def write(data: bytes) -> None:
            uf2.write(data)

        def write32(*ints: int) -> None:
            for i in ints:
                write(pack("<I", i))

        # Output of "binsum" should have produced at least 256 bytes
        # (it will have added padding + a checksum)
        data = bin.read()
        assert len(data) >= 256

        # Pad to multiple of 256
        while len(data) % 256:
            data += b'\x00'

        blocks = len(data) // 256
        target = TARGET

        for block in range(blocks):
          write32(
              MAGIC_START0,
              MAGIC_START1,
              FLAG_FAMILY_ID,
              target,
              256,
              block,
              blocks,
              RP2040_FAMILY_ID)
          write(data[:256])
          write(b'\x00' * (476 - 256))
          write32(MAGIC_END)
          target += 256
          data = data[256:]


if __name__ == "__main__":
    bin_path = Path(sys.argv[1])
    uf2_path = Path(sys.argv[1] + ".uf2")
    bin2uf2(bin_path, uf2_path)
