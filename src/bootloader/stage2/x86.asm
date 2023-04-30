;
;   MACROS
;
%macro EnterRealMode 0
    bits 32
    jmp word 0x18:.pmode16

.pmode16:
    bits 16
    
    mov eax, cr0
    and al, ~1
    mov cr0, eax

    jmp word 0x00:.rmode

.rmode:
    mov ax, 0
    mov ds, ax
    mov ss, ax

    sti
%endmacro


%macro EnterProtectedMode 0
    cli

    mov eax, cr0
    or al, 1
    mov cr0, eax

    jmp dword 0x08:.pmode

.pmode:
    bits 32
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

; _cdecl void outb(u16 port, u8 value);
global outb
outb:
    bits 32
    mov dx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret

; _cdecl u8 inb(u16 port);
global inb
inb:
    bits 32
    mov dx, [esp+4]
    xor EAX, EAX
    in al, dx
    ret

; _cdecl bool x86_disk_get_params_(u8 drive, u8 *drive_type, u16 *cylinders, u16 *sectors, u16 *heads);
global x86_disk_get_params_
x86_disk_get_params_:
    bits 32
    push ebp    
    mov ebp, esp

    EnterRealMode

    bits 16
    push es
    push bx
    push esi
    push di

    ; Call Disk Get Parameters interrupt
    mov dl, [bp+8]
    mov ah, 0x08
    mov di, 0
    mov es, di
    stc
    int 13h

    ; Set return value (1 success, 0 failure)
    mov eax, 1
    sbb eax, 0

    ; Drive Type
    LinearToSegOffset [bp+12], es, esi, si
    mov [es:si], bl

    ; Cylinders
    mov bl, ch
    mov bh, cl
    shr bh, 6
    inc bx

    LinearToSegOffset [bp+16], es, esi, si
    mov [es:si], bx

    ; Sectors
    xor ch, ch
    and cl, 3Fh
    
    LinearToSegOffset [bp+20], es, esi, si
    mov [es:si], cx

    ; Heads
    mov cl, dh
    inc cx
    LinearToSegOffset [bp+24], es, esi, si
    mov [es:si], cx

    pop di
    pop esi
    pop bx
    pop es

    ; Return
    push eax

    EnterProtectedMode

    bits 32
    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl bool x86_disk_reset_(u8 drive);
global x86_disk_reset_
x86_disk_reset_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    ; Call Disk Reset interrupt
    mov ah, 0
    mov dl, [bp+8]
    stc
    int 0x13

    ; Set return value (1 success, 0 failure)
    mov eax, 1
    sbb eax, 0

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl bool x86_disk_read_(u8 drive, u16 cylinder, u16 sector, u16 head, u8 count, void *buffer);
global x86_disk_read_
x86_disk_read_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    push ebx
    push es

    ; Setup args
    mov dl, [bp+8]

    mov ch, [bp+12]
    mov cl, [bp+13]
    shl cl, 6
    
    mov al, [bp+16]
    and al, 3Fh
    or cl, al

    mov dh, [bp+20]

    mov al, [bp+24]

    LinearToSegOffset [bp+28], es, ebx, bx

    ; Call Disk Read interrupt
    mov ah, 02h
    stc
    int 13h

    ; Set return value (1 success, 0 failure)
    mov eax, 1
    sbb eax, 0

    pop es
    pop ebx

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl void x86_video_init_(u16 mode);
global x86_video_init_
x86_video_init_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    ; Call Video Mode interrupt
    mov ax, [bp+8]
    int 0x10

    EnterProtectedMode

    mov esp, ebp
    pop ebp
    ret

; _cdecl u16 x86_vbe_get_info_(void *info);
global x86_vbe_get_info_
x86_vbe_get_info_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    push es
    push edi
    push ebp

    ; Setup args
    LinearToSegOffset [bp+8], es, edi, di

    ; Call VBE Get Info interrupt
    mov ax, 0x4F00
    int 0x10

    pop ebp
    pop edi
    pop es

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl u16 x86_vbe_get_mode_info_(u16 mode, void *info);
global x86_vbe_get_mode_info_
x86_vbe_get_mode_info_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    push es
    push edi
    push ecx
    push ebp

    ; Setup args
    mov cx, [bp+8]
    LinearToSegOffset [bp+12], es, edi, di

    ; Call VBE Get Mode Info interrupt
    mov ax, 0x4F01
    int 0x10

    pop ebp
    pop ecx
    pop edi
    pop es

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl void x86_vbe_set_mode_(u16 mode);
global x86_vbe_set_mode_
x86_vbe_set_mode_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    push es
    push edi
    push ebx
    push ebp

    ; Call VBE Set Mode interrupt
    mov ax, 0
    mov es, ax
    mov edi, 0
    mov ax, 0x4F02
    mov bx, [bp+8]
    int 0x10

    pop ebp
    pop ebx
    pop edi
    pop es

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl void x86_vbe_get_mode_(u16 *mode);
global x86_vbe_get_mode_
x86_vbe_get_mode_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    push es
    push edi
    push ebx
    push ebp

    LinearToSegOffset [bp+8], es, edi, di

    ; Call VBE Get Mode interrupt
    mov ax, 0x4F03
    int 0x10

    mov [es:di], bx

    pop ebp
    pop ebx
    pop edi
    pop es

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

; _cdecl i32 x86_mem_map_(x86_mem_map_entry_t *entries, u32 *cont_id);
global x86_mem_map_
x86_mem_map_:
    bits 32
    push ebp
    mov ebp, esp

    EnterRealMode

    push es
    push ds
    push esi
    push edi
    push ebx
    push ecx
    push edx

    LinearToSegOffset [bp+8], es, edi, di
    LinearToSegOffset [bp+12], ds, esi, si
    mov ebx, ds:[si]

    ; Call Memory Map interrupt
    mov eax, 0xE820
    mov ecx, 24
    mov edx, E820Signature
    int 0x15

    cmp eax, E820Signature
    jne .error

.success:
    mov eax, ecx
    mov ds:[si], ebx
    jmp .end
.error:
    mov eax, -1
.end:

    pop edx
    pop ecx
    pop ebx
    pop edi
    pop esi
    pop ds
    pop es

    ; Return
    push eax

    EnterProtectedMode

    pop eax

    mov esp, ebp
    pop ebp
    ret

;
;   Constants
;
E820Signature equ 0x534D4150