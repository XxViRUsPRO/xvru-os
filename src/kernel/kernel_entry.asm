section .entry
extern _main32

global entry
entry:
    bits 32
    CALL _main32

    JMP $

TIMES 10240 DB 0
