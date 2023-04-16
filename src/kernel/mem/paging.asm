;
; Enable Paging
;
global enablePaging
enablePaging:
    bits 32
    ; load page directory (eax has the address of the page directory) 
    MOV EAX, [ESP+4]
    MOV CR3, EAX        

    ; enable 4MBpage
    MOV EBX, CR4        ; read current cr4 
    OR  EBX, 0x00000010 ; set PSE  - enable 4MB page
    MOV CR4, EBX        ; update cr4

    ; enable paging 
    MOV EBX, cr0        ; read current cr0
    OR  EBX, 0x80000000 ; set PG .  set pages as read-only for both userspace and supervisor, replace 0x80000000 above with 0x80010000, which also sets the WP bit.
    MOV cr0, EBX        ; update cr0
    RET                 ; now paging is enabled
