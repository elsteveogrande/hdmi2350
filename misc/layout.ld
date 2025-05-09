MEMORY {
    FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 2048k
    SRAM(rw)  : ORIGIN = 0x20000000, LENGTH = 512k

    /* SRAM8 and SRAM9 are omitted here, because we won't deal with them
    at the linker-script level.  But the application is free to use the
    memory at 0x20048xxx and 0x20049xxx however it wants. */
}

/* Not actually used but this will silence warnings */
ENTRY(0x10000000)

SECTIONS {
  .init_vec_table (0x10000000)      : {} > FLASH
  .image_def      (0x10000008)      : {} > FLASH
  .text                             : {} > FLASH
  .data                             : {} > FLASH
  .rodata                           : {} > FLASH

  .stack      (0x20000000) (NOLOAD) : {} >  SRAM
  .bss        (0x20000400) (NOLOAD) : {} >  SRAM
}
