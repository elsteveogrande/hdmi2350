# pico-cxx-base


## Dependencies

* LLVM (Clang, LLD)
* `make`


## Memory map of an RP2040 or RP2350 binary

Well-known addresses, at the start of `boot2`, which is invoked by Pico's boot ROM:

| Base address   | Last byte      | Size (bytes)   | Executable |                                 |
| -------------- | -------------- | -------------- | -          | ------------------------------- |
| `0x1000_0000`  | `0x0000_00ff`  | 256            | √          | `.bootsector`                   | 
| `0x1000_0100`  | `0x0000_0fff`  | 3584           | √          | `.start0` section               | 
| `0x1000_1000`  | ...            | ...            | √          | Remaining ELF code              | 
| ...            | ...            | ...            |            | ELF readonly data               | 
| `0x2000_0000`  | `0x2000_0fff`  | 4096           |            | System stack                    |
| `0x2000_1000`  | `0x2000_1fff`  | 4096           |            | System description struct       |
| `0x2000_2000`  | `0x2000_2000`  | ...            |            | .bss uninitialized data         |
