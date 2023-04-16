section .text
extern _main32

; extern __bss_start
; extern __end

global entry
entry:
    bits 32

    ; Clear BSS
    ; MOV EDI, __bss_start
    ; MOV ECX, __end
    ; SUB ECX, EDI
    ; MOV AL, 0
    ; CLD ; Clear direction flag
    ; REP STOSB ; Fill memory with 0

    CALL _main32

    JMP $

TIMES 10240 DB 0
