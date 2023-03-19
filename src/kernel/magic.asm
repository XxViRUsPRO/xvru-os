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
