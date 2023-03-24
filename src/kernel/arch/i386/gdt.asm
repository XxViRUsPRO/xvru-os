global GDT_load
GDT_load:
    bits 32

    PUSH EBP
    MOV EBP, ESP

    MOV EAX, [EBP+8]
    LGDT [EAX]

    ; Reload CS
    MOV EAX, [EBP+12]
    PUSH EAX
    PUSH .RELOAD_CS
    RETF
.RELOAD_CS:
    MOV AX, [EBP+16]
    MOV DS, AX
    MOV ES, AX
    MOV FS, AX
    MOV GS, AX
    MOV SS, AX

    MOV ESP, EBP
    POP EBP
    RET