org 0x7C00

_main16:
    bits 16
    MOV [BOOT_DISK], DL

    MOV AX, 0
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV SP, 0x7C00

    ; SETUP VIDEO MODE
    ; MOV AX, 0x0013
    ; INT 0x10

    ; READ FIRST 100 SECTOR AT KERNEL LOCATION
    MOV BX, KERNEL_ADDR
    MOV DH, 100

    ; DISK READ
    MOV AH, 0x02
    MOV AL, DH
    MOV CH, 0x00
    MOV DH, 0x00
    MOV CL, 0x02
    MOV DL, [BOOT_DISK]
    INT 0x13
    
    ; HERE MAKE THE JUMP TO 32BIT
    CLI
    CALL ENABLE_A20LINE ; Enbale A20 Line
    CALL LOAD_GDT ; Load GDT
    MOV EAX, CR0
    OR EAX, 0x1
    MOV CR0, EAX
    JMP CODE_SEG:START_PROTECTED

%include "a20line.inc"
%include "gdt.inc"

;
; 32BIT PROTECTED MODE
;
START_PROTECTED:
    bits 32
    CLI
    MOV AX, DATA_SEG
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV FS, AX
    MOV GS, AX

    ; SETUP THE STACK
    MOV EBP, 0x90000
    MOV ESP, EBP

    JMP KERNEL_ADDR


;
;   GLOBAL VARIABLES
;
BOOT_DISK: DB 0x00

;
;   CONSTANTS
;
KERNEL_ADDR EQU 0x00000500

TIMES 510-($-$$) DB 0
DW 0xAA55