;
; Enable A20 line
;
ENABLE_A20LINE:
    bits 16
    ; Disable keyboard
    CALL A20INPUTWAIT
    MOV AL, KBD_DISABLE
    OUT KBD_CMND_PORT, AL

    ; Read control output
    CALL A20INPUTWAIT
    MOV AL, KBD_READ_CONTROLER_OUTPUT_PORT
    OUT KBD_CMND_PORT, AL

    CALL A20OUTPUTWAIT
    IN AL, KBD_DATA_PORT
    PUSH EAX

    ; Write control output
    CALL A20INPUTWAIT
    MOV AL, KBD_WRITE_CONTROLER_OUTPUT_PORT
    OUT KBD_CMND_PORT, AL

    CALL A20OUTPUTWAIT
    POP EAX
    OR AL, 2
    OUT KBD_DATA_PORT, AL

    ; Enable keyboard
    CALL A20INPUTWAIT
    MOV AL, KBD_ENABLE
    OUT KBD_CMND_PORT, AL

    CALL A20INPUTWAIT
    RET

A20INPUTWAIT:
    bits 16
    IN AL, KBD_CMND_PORT
    TEST AL, 2
    JNZ A20INPUTWAIT
    RET
A20OUTPUTWAIT:
    bits 16
    IN AL, KBD_DATA_PORT
    TEST AL, 1
    JNZ A20OUTPUTWAIT
    RET

; KBD PORTS
KBD_DATA_PORT                       EQU 0x60
KBD_CMND_PORT                       EQU 0x64
KBD_ENABLE                          EQU 0xAE
KBD_DISABLE                         EQU 0xAD
KBD_READ_CONTROLER_OUTPUT_PORT      EQU 0xAD
KBD_WRITE_CONTROLER_OUTPUT_PORT     EQU 0xAD
