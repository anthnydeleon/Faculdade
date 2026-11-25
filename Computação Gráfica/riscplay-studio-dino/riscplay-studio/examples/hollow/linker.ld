OUTPUT_ARCH(riscv)
ENTRY(_start)

MEMORY { CPU (rwx) : ORIGIN = 0x80000000, LENGTH = 32K }
PHDRS {
    code PT_LOAD FLAGS(5);
    writable PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = ORIGIN(CPU);
    .text : ALIGN(4) {
        KEEP(*(.text.start))
        *(.text .text.*)
    } > CPU :code
    .rodata : ALIGN(4) {
        *(.rodata .rodata.* .srodata .srodata.*)
    } > CPU :code
    .data : ALIGN(4) {
        *(.data .data.* .sdata .sdata.*)
    } > CPU :writable
    .bss (NOLOAD) : ALIGN(4) {
        __bss_start = .;
        *(.bss .bss.* .sbss .sbss.*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;
    } > CPU :writable
    __stack_top = ORIGIN(CPU) + LENGTH(CPU);
    ASSERT(__bss_end <= 0x80007000, "CPU image overlaps reserved 4 KiB stack")
    /DISCARD/ : { *(.comment .note .note.* .eh_frame .eh_frame.*) }
}
