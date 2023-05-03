; Utility macros
%macro EnterRealMode 0
    [bits 32]
    jmp word 18h:.pmode16         ; 1 - jump to 16-bit protected mode segment

.pmode16:
    [bits 16]
    ; 2 - disable protected mode bit in cr0
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    ; 3 - jump to real mode
    jmp word 00h:.rmode

.rmode:
    ; 4 - setup segments
    mov ax, 0
    mov ds, ax
    mov ss, ax

    ; 5 - enable interrupts
    sti

%endmacro


%macro EnterProtectedMode 0
    cli

    ; 4 - set protection enable flag in CR0
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ; 5 - far jump into protected mode
    jmp dword 08h:.pmode


.pmode:
    ; we are now in protected mode!
    [bits 32]
    
    ; 6 - setup segment registers
    mov ax, 0x10
    mov ds, ax
    mov ss, ax

%endmacro

; Convert linear address to segment:offset address
; Args:
;    1 - linear address
;    2 - (out) target segment (e.g. es)
;    3 - target 32-bit register to use (e.g. eax)
;    4 - target lower 16-bit half of #3 (e.g. ax)

%macro LinearToSegOffset 4

    mov %3, %1      ; linear address to eax
    shr %3, 4
    mov %2, %4
    mov %3, %1      ; linear address to eax
    and %3, 0xf

%endmacro


; Kernel functions
global outb
outb:
    bits 32
    MOV DX, [ESP+4]
    MOV AL, [ESP+8]
    OUT DX, AL
    RET

global inb
inb:
    bits 32
    MOV DX, [ESP+4]
    XOR EAX, EAX
    IN AL, DX
    RET

global outw
outw:
    bits 32
    MOV DX, [ESP+4]
    MOV AX, [ESP+8]
    OUT DX, AX
    RET

global inw
inw:
    bits 32
    MOV DX, [ESP+4]
    XOR EAX, EAX
    IN AX, DX
    RET

global cli
cli:
    CLI
    RET

global sti
sti:
    STI
    RET

global panic
panic:
    CLI
    HLT

; void x86_enable_paging(void);
global x86_enable_paging:
x86_enable_paging:
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    ret

; void x86_load_pdbr(void *pdbr);
global x86_load_pdbr:
x86_load_pdbr:
    mov eax, [esp+4]
    mov cr3, eax
    ret

; void x86_flush_tlb(void *addr);
global x86_flush_tlb:
x86_flush_tlb:
    cli
    mov eax, [esp+4]
    invlpg [eax]
    sti
    ret
