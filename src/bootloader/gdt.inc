;
; Load GDT
;
LOAD_GDT:
    LGDT [GDT_DESC]
    RET

; GDT Segment Descriptors
GDT_START:
    NULL_DESC: 
        DD 0, 0
    CODE_DESC: 
        DW 0xFFFF
        DW 0x00
        DB 0x00
        DB 10011010b
        DB 11001111b
        DB 0x00
    DATA_DESC: 
        DW 0xFFFF
        DW 0x00
        DB 0x00
        DB 10010010b
        DB 11001111b
        DB 0x00
    CODE16_DESC:
        DW 0xFFFF
        DW 0x00
        DB 0x00
        DB 10011010b
        DB 00001111b
        DB 0x00
    DATA16_DESC:
        DW 0xFFFF
        DW 0x00
        DB 0x00
        DB 10010010b
        DB 00001111b
        DB 0x00
GDT_END:
GDT_DESC:
    DW GDT_END - GDT_START - 1
    DD GDT_START

; OUR CODE/DATA SEGMENTS
CODE_SEG EQU CODE_DESC - GDT_START
DATA_SEG EQU DATA_DESC - GDT_START
CODE16_SEG EQU CODE16_DESC - GDT_START
DATA16_SEG EQU DATA16_DESC - GDT_START