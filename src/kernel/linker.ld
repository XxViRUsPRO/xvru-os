ENTRY(entry)
OUTPUT_FORMAT("binary")
PHYS = 0X7EF0;

SECTIONS
{
    . = PHYS;
    .entry              : { __entry_start = .;    *(.entry) }
    .text               : { __text_start = .;     *(.text)  }
    .data               : { __data_start = .;     *(.data)  }

    __end = .;
}