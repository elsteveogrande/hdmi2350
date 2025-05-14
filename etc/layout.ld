MEMORY {
  FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 2048k
  
  /* split SRAM into separate regions: */
  VEC_TABLE(rw)   : ORIGIN = 0x20000000, LENGTH = 256
  STACK(rw)       : ORIGIN = 0x20000100, LENGTH = 768  /* sp = 0x20000400 */
  BSS(rw)         : ORIGIN = 0x20000400, LENGTH = 63k
  FRAMEBUFFER(rw) : ORIGIN = 0x20010000, LENGTH = 448k

  /*
  Use these separate, small, fast SRAM regions for line buffers.
  Generally, even/odd lines rendered into here prior to being sent over HSTX.
  */
  LINE0(rw)       : ORIGIN = 0x20080000, LENGTH = 4k
  LINE1(rw)       : ORIGIN = 0x20081000, LENGTH = 4k
}

__vec_table = ORIGIN(VEC_TABLE);
__stack_top = ORIGIN(STACK) + SIZEOF(STACK);
__fb        = ORIGIN(FRAMEBUFFER);
__line0     = ORIGIN(LINE0);
__line1     = ORIGIN(LINE1);

/* Not actually used but this will silence warnings */
ENTRY(0x10000000)

SECTIONS {
  .init_vec_table (0x10000000)      : {} > FLASH
  .image_def      (0x10000008)      : {} > FLASH
  .text                             : {} > FLASH
  .data                             : {} > FLASH
  .rodata                           : {} > FLASH

  .vec_table  (NOLOAD) : {} > VEC_TABLE
  .stack      (NOLOAD) : {} > STACK
  .bss        (NOLOAD) : {} > BSS
}
