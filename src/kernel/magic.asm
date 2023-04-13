global outb:
outb:
    bits 32
    MOV DX, [ESP+4]
    MOV AL, [ESP+8]
    OUT DX, AL
    RET

global inb:
inb:
    bits 32
    MOV DX, [ESP+4]
    XOR EAX, EAX
    IN AL, DX
    RET

global outw:
outw:
    bits 32
    MOV DX, [ESP+4]
    MOV AX, [ESP+8]
    OUT DX, AX
    RET

global inw:
inw:
    bits 32
    MOV DX, [ESP+4]
    XOR EAX, EAX
    IN AX, DX
    RET

global CLI
CLI:
    CLI
    RET

global STI
STI:
    STI
    RET

global panic:
panic:
    CLI
    HLT
