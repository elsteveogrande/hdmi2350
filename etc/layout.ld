MEMORY {
  FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 2048k
  
  /* split SRAM into separate regions: */
  STACK(rw)       : ORIGIN = 0x20000100, LENGTH = 3072 /* sp = 0x20000c00 */
  PANICSTACK(rw)  : ORIGIN = 0x20000c00, LENGTH = 1024 /* sp = 0x20001000 when panicking */
  BSS(rw)         : ORIGIN = 0x20001000, LENGTH = 60k
  FRAMEBUFFER(rw) : ORIGIN = 0x20010000, LENGTH = 448k

  /*
  Use these separate, small, fast SRAM regions for line buffers.
  Even/odd lines rendered into here prior to being sent over HSTX.
  */
  LINE0(rw)       : ORIGIN = 0x20080000, LENGTH = 4k
  LINE1(rw)       : ORIGIN = 0x20081000, LENGTH = 4k
}

__reset_sp  = ORIGIN(STACK) + SIZEOF(STACK);
__panic_sp  = ORIGIN(PANICSTACK) + SIZEOF(PANICSTACK);
__fb_base   = ORIGIN(FRAMEBUFFER);
__tx_line0  = ORIGIN(LINE0);
__tx_line1  = ORIGIN(LINE1);

__bss_base  = ORIGIN(BSS);
__bss_size  = SIZEOF(BSS);

/* Not actually used but this will silence warnings */
ENTRY(0x10000000)

SECTIONS {
  .vec_table  (0x10000000)  : {} > FLASH
  .image_def  (0x10000100)  : {} > FLASH
  .text                     : {} > FLASH
  .data                     : {} > FLASH
  .rodata                   : {} > FLASH

  .stack      (NOLOAD)      : {} > STACK
  .bss        (NOLOAD)      : {} > BSS

  .framebuf   (NOLOAD)      : {} > FRAMEBUFFER
}
