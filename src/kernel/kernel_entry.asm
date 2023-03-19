section .entry

bits 32
global entry
extern _main32
entry:
    CALL _main32

    JMP $

TIMES 10240 DB 0
