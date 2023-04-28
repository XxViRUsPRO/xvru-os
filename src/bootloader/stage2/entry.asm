bits 16
section .entry

extern __bss_start
extern __end

extern start
global stage2_start
stage2_start:
    cli
    mov [disk_id], dl ; Save the disk id

    ; SETUP SEGMENTS
    mov ax, ds
    mov ss, ax
    ; SETUP STACK
    mov sp, 0xFFF0
    mov bp, sp

    ; PRINT MSG
    mov si, msg
.loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0e
    mov bx, 0x0007
    int 0x10
    jmp .loop
.done:
    ; HERE MAKE THE JUMP TO 32BIT
    call ENABLE_A20LINE ; Enbale A20 Line
    call LOAD_GDT ; Load GDT
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    jmp CODE_SEG:START_PROTECTED

%include "a20line.inc"
%include "gdt.inc"

;
; 32BIT PROTECTED MODE
;
START_PROTECTED:
    bits 32
    ; SETUP SEGMENTS
    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    ; CLEAR BSS
    mov edi, __bss_start
    mov ecx, __end
    sub ecx, edi
    xor eax, eax
    cld
    rep stosb
    
    xor edx, edx
    mov dl, [disk_id]
    push edx

    call start

halt:
    cli
    hlt

;
;   DEFINES
;
%define ENDL 0x0D, 0x0A, 0x00

;
;   GLOBAL VARIABLES
;
msg: db "Stage 2> Loaded!", ENDL
disk_id: db 0x00
