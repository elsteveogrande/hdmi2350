MEMORY {
    FLASH(rx) : ORIGIN = 0x10000000, LENGTH = 2048k
    SRAM(rw)  : ORIGIN = 0x20000000, LENGTH = 512k
}

ENTRY (0x10000001)

SECTIONS {

    .bootsector : {
        KEEP(*(_reset));
        KEEP(*(_boot3));
        KEEP(*(.bootsector));
    } > FLASH
        
    .text : {
        KEEP(.text(_start));
        *(.text*);
    } > FLASH

    .const : {
        *(.const*);
    } > FLASH

    .bss : {
        *(.bss);
    } > SRAM

    .data : {
        *(.data*);
    } > SRAM

}
