ENTRY(kmain)
OUTPUT_FORMAT("binary")
PHYS = 0x00100000;

SECTIONS
{
    . = PHYS;
    .text               : { __text_start = .;     *(.text)  }
    .data               : { __data_start = .;     *(.data)  }
    .rodata             : { __rodata_start = .;   *(.rodata)}
    .bss                : { __bss_start = .;      *(.bss)   }

    __end = .;
}