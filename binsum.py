#!/usr/bin/env python3

import sys
from os import SEEK_SET, SEEK_END
from pathlib import Path
from struct import pack

from crc import Crc32, Calculator, Configuration

def padbin(bin_path) -> None:
    # Read in whole binary
    with open(bin_path, "rb") as bin:
        data = bin.read()

    # Ensure there are >= 256 bytes, pad with zeroes if needed
    if len(data) < 256:
        data += (b'\x00' * (256 - len(data)))

    # Overwrite [252:256] with the computed checksum (of prior 252 bytes)
    crcbytes = pack("<I", Calculator(Configuration(
              width=32,
              polynomial=0x04c11db7,
              init_value=0xffffffff,
              final_xor_value=0x00000000,
              reverse_input=False,
              reverse_output=False,
          )).checksum(data[:252]))

    # Splice in the checksum
    data = data[:252] + crcbytes + data[256:]

    # Rewrite (maybe-padded) and checksummed data
    with open(bin_path, "wb") as bin:
        bin.write(data)

if __name__ == "__main__":
    bin_path = Path(sys.argv[1])
    padbin(bin_path)
