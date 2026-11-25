ENTRY(_start)

MEMORY
{
  RAM (rwx) : ORIGIN = 0x80000000, LENGTH = 32K
}

PHDRS
{
  text PT_LOAD FLAGS(5); /* R + X */
  data PT_LOAD FLAGS(6); /* R + W */
}

SECTIONS
{
  /* Código */
  .text : {
    *(.init)
    *(.text*)
  } > RAM :text

  /* Constantes */
  .rodata : {
    *(.rodata*)
    *(.srodata*)
  } > RAM :text

  /* Dados inicializados */
  .data : {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    *(.sdata*)
    . = ALIGN(4);
    _edata = .;
  } > RAM :data

  /* Dados não inicializados */
  .bss (NOLOAD) : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(.sbss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM :data

  /* Topo da stack */
  _stack_top = ORIGIN(RAM) + LENGTH(RAM);
  ASSERT(_ebss <= _stack_top - 4096, "CPU image overlaps reserved 4 KiB stack")
}