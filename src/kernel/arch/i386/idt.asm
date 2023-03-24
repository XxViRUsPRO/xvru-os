global IDT_load
IDT_load:
    bits 32

    PUSH EBP
    MOV EBP, ESP

    MOV EAX, [EBP+8]
    LIDT [EAX]

    MOV ESP, EBP
    POP EBP
    RET