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

See "5.5.2. UF2 Format Details" in the datasheet (page 400 or around there):
https://datasheets.raspberrypi.com/rp2350/rp2350-datasheet.pdf
"""

MAGIC_START0          = 0x0a324655
MAGIC_START1          = 0x9e5d5157
MAGIC_END             = 0x0ab16f30
# absolute: "Special family ID for content intended to be written directly to flash, ignoring partitions"
DIRECT_FLASH_ID       = 0xe48bff57
# rp2350_arm_s: "RP2350 Arm Secure image (i.e. one intended to be booted by the bootrom)"
# (This will also require the S flag = 0x02 in the image definition.)
RP2350_ARMSECURE_ID   = 0xe48bff59
FLAG_FAMILY_ID        = 0x00002000
TARGET                = 0x10000000


def bin2uf2(bin_path: Path, uf2_path: Path) -> None:
    with (open(bin_path, "rb") as bin,
          open(uf2_path, "wb") as uf2):

        def write(data: bytes) -> None:
            uf2.write(data)

        def write32(*ints: int) -> None:
            for i in ints:
                write(pack("<I", i))

        data = bin.read()
        assert len(data)

        # Pad to multiple of 256
        while len(data) % 256:
            data += b'\x00'

        blocks = len(data) // 256
        target = TARGET

        # Before writing the actual flash content, we need to write this dummy
        # block, as a workaround for some erratum (see E10 in the datasheet).
        # See thread at: https://forums.raspberrypi.com/viewtopic.php?t=375030
        """
# First UF2 block from:
# hexdump -C ~/Downloads/RPI_PICO2-20250415-v1.25.0.uf2
~/code/hdmi2350$ hexdump -C ~/Downloads/RPI_PICO2-20250415-v1.25.0.uf2 | cat -n | head -n 11
     1	00000000  55 46 32 0a 57 51 5d 9e  00 a0 00 00 00 ff ff 10  |UF2.WQ].........|
     2	00000010  00 01 00 00 00 00 00 00  02 00 00 00 57 ff 8b e4  |............W...|
     3	00000020  ef ef ef ef ef ef ef ef  ef ef ef ef ef ef ef ef  |................|
     4	*
     5	00000120  04 e3 57 99 00 00 00 00  00 00 00 00 00 00 00 00  |..W.............|
     6	00000130  00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00  |................|
     7	*
     8	000001f0  00 00 00 00 00 00 00 00  00 00 00 00 30 6f b1 0a  |............0o..|
     9	00000200  55 46 32 0a 57 51 5d 9e  00 20 00 00 00 00 00 10  |UF2.WQ].. ......|
    10	00000210  00 01 00 00 00 00 00 00  f2 04 00 00 59 ff 8b e4  |............Y...|
    11	00000220  00 20 08 20 5d 01 00 10  13 01 00 10 15 01 00 10  |. . ]...........|
"""
        write32(MAGIC_START0)
        write32(MAGIC_START1)
        write32(0x0000a000)   # Flags: ?
        write32(0x10ffff00)   # End of flash CS0; just a place to write 256 junk bytes
        write32(0x00000100)   # Size = 256
        write32(0x00000000)   # Block = 0
        write32(0x00000002)   # Blocks = 2 (?)
        write32(DIRECT_FLASH_ID)
        write(b'\xef' * 256)
        write32(0x9957e304)   # Unknown.  Random junk in the UF2 packet's padding, I guess?
        write(b'\x00' * (0x1fc - 0x124))
        write32(MAGIC_END)

        # And now our actual binary image:

        for block in range(blocks):
          write32(
              MAGIC_START0,
              MAGIC_START1,
              FLAG_FAMILY_ID,
              target,
              256,
              block,
              blocks,
              RP2350_ARMSECURE_ID)
          write(data[:256])
          write(b'\x00' * (476 - 256))
          write32(MAGIC_END)
          target += 256
          data = data[256:]


if __name__ == "__main__":
    bin_path = Path(sys.argv[1])
    uf2_path = Path(sys.argv[1] + ".uf2")
    bin2uf2(bin_path, uf2_path)
