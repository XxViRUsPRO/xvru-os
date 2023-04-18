org 0x7C00
bits 16

; FAT HEADER
; BIOS PARAMETER BLOCK
JMP SHORT stage1_start
NOP
BPB_OEM: DB "MSWIN4.1"
BPB_BytesPerSec: DW 512
BPB_SecPerClus: DB 1
BPB_RsvdSecCnt: DW 1
BPB_NumFATs: DB 2
BPB_RootEntCnt: DW 0xE0
BPB_TotSec16: DW 2880
BPB_Media: DB 0xF0
BPB_FATSz16: DW 9
BPB_SecPerTrk: DW 18
BPB_NumHeads: DW 2
BPB_HiddSec: DD 0
BPB_TotSec32: DD 0

; EXTENDED BOOT RECORD
EBR_DrvNum: DB 0x80
EBR_Reserved1: DB 0
EBR_BootSig: DB 0x29
EBR_VolID: DD 0x12345678
EBR_VolLab: DB "NO NAME    "
EBR_FilSysType: DB "FAT12   "

stage1_start:
    MOV AX, 0
    MOV DS, AX
    MOV ES, AX
    MOV SS, AX
    MOV SP, 0x7C00

    PUSH ES
    PUSH WORD .stage1_continue
    RETF
.stage1_continue:
    MOV [EBR_DrvNum], DL

    ; PRINT MSG
    MOV SI, msg
    CALL PUTS

    ; READ DISK PARAMETERS
    PUSH ES
    MOV AH, 0x08
    INT 0x13
    JC DISK_READ_ERROR
    POP ES

    AND CL, 0x3F
    XOR CH, CH
    MOV [BPB_SecPerTrk], CX

    INC DH
    MOV [BPB_NumHeads], DH

    ; READING FAT ROOT DIRECTORY
    MOV AX, [BPB_FATSz16]
    MOV BL, [BPB_NumFATs]
    XOR BH, BH
    MUL BX
    ADD AX, [BPB_RsvdSecCnt]
    PUSH AX

    MOV AX, [BPB_FATSz16]
    SHL AX, 5
    XOR DX, DX
    DIV WORD [BPB_BytesPerSec]

    TEST DX, DX
    JZ .root_dir_ok
    INC AX

.root_dir_ok:
    MOV CL, AL
    POP AX
    MOV DL, [EBR_DrvNum] 
    MOV BX, BUFFER
    CALL DISK_READ

    ; SEARCHING FOR STAGE2
    XOR BX, BX
    MOV DI, BUFFER
.loop1:
    MOV SI, stage2_file_name
    MOV CX, 11
    PUSH DI
    REPE CMPSB
    POP DI
    JE .found

    ADD DI, 32
    INC BX
    CMP BX, [BPB_RootEntCnt]
    JL .loop1
    JMP .not_found
.found:
    MOV AX, [DI+26]
    MOV [stage2_cluster], AX

    ; LOADING FAT
    MOV AX, [BPB_RsvdSecCnt]
    MOV BX, BUFFER
    MOV CL, [BPB_FATSz16]
    MOV DL, [EBR_DrvNum]
    CALL DISK_READ

    ; LOADING STAGE2
    MOV BX, STAGE2_LOAD_SEG
    MOV ES, BX
    MOV BX, STAGE2_LOAD_OFF
.loop2:
    MOV AX, [stage2_cluster]

    ADD AX, 31 ; FIRST CLUSTER

    MOV CL, 1
    MOV DL, [EBR_DrvNum]
    CALL DISK_READ

    ADD BX, [BPB_BytesPerSec]

    ; LOCATION OF NEXT CLUSTER
    MOV AX, [stage2_cluster]
    MOV CX, 3
    MUL CX
    MOV CX, 2
    DIV CX

    MOV SI, BUFFER
    ADD SI, AX
    MOV AX, [DS:SI]

    OR DX, DX
    JZ .pair
.impair:
    SHR AX, 4
    JMP .next
.pair:
    AND AX, 0x0FFF
.next:
    CMP AX, 0x0FF8
    JAE .end
    MOV [stage2_cluster], AX
    JMP .loop2
.end:
    ; JUMP TO STAGE2
    MOV DL, [EBR_DrvNum]

    MOV AX, STAGE2_LOAD_SEG
    MOV DS, AX
    MOV ES, AX

    JMP STAGE2_LOAD_SEG:STAGE2_LOAD_OFF
.not_found:
    MOV SI, stage2_not_found_msg
    CALL PUTS
HALT:
    CLI
    HLT

;
;   FUNCTIONS
;

;  PUTS
;  PARAMS:
;   - DS:SI = STRING
PUTS:
    PUSH SI
    PUSH AX
    PUSH BX
.loop:
    LODSB
    OR al, al
    JZ .done
    MOV ah, 0x0e
    MOV bx, 0x0007
    INT 0x10
    JMP .loop
.done:
    POP BX
    POP AX
    POP SI
    RET

;  WAIT FOR KEY PRESS
;  RETURNS:
;   - AL = KEY
WAIT_KEY:
    MOV AH, 0x00
    INT 0x16
    RET

;  READ SECTORS FROM DISK
;  PARAMS:
;   - AX = LBA ADDRESS
;   - CL = NUMBER OF SECTORS
;   - DL = DISK NUMBER
;   - ES:BX = BUFFER
DISK_READ:
    PUSHA

    PUSH CX
    CALL LBA_TO_CHS
    POP AX

    MOV AH, 0x02
    MOV DI, 3 ; RETRY COUNTER
.loop:
    PUSHA
    STC
    INT 0x13
    JNC .done
    POPA
    CALL DISK_RESET
    DEC DI
    TEST DI, DI
    JNZ .loop
.error:
    JMP DISK_READ_ERROR
.done:
    POPA

    POPA
    RET

;  DISK RESET
;  PARAMS:
;   - DL = DISK NUMBER
DISK_RESET:
    PUSHA
    MOV AH, 0x00
    STC
    INT 0x13
    JC DISK_READ_ERROR
    POPA
    RET
    
; DISK READ ERROR
DISK_READ_ERROR:
    MOV SI, disk_read_error_msg
    CALL PUTS
    CALL WAIT_KEY
    JMP 0x0FFFF:0x0000

;  LBA TO CHS
;  PARAMS:
;   - AX = LBA
;  RETURNS:
;   - CX [0-5] = SECTOR
;   - CX [6-15] = CYLINDER
;   - DH = HEAD
LBA_TO_CHS:
    PUSH AX
    PUSH DX

    XOR DX, DX ; DX = 0
    DIV WORD [BPB_SecPerTrk] ; AX = LBA / BPB_SecPerTrk
                             ; DX = LBA % BPB_SecPerTrk

    INC DX ; DX = (LBA % BPB_SecPerTrk) + 1 => SECTOR
    MOV CX, DX ; CX = SECTOR

    XOR DX, DX ; DX = 0
    DIV WORD [BPB_NumHeads] ; AX = (LBA / BPB_SecPerTrk) / BPB_NumHeads => CYLINDER
                            ; DX = (LBA / BPB_SecPerTrk) % BPB_NumHeads => HEAD
    
    MOV DH, DL
    MOV CH, AL
    SHL AH, 6
    OR CL, AH

    POP AX
    MOV DL, AL
    POP AX
    RET

;
;   DEFINES
;
%define ENDL 0x0D, 0x0A, 0x00

;
;   GLOBAL VARIABLES
;
msg: DB "Stage 1> Loaded!", ENDL
disk_read_error_msg: DB "Stage 1> Disk read error!", ENDL
stage2_not_found_msg: DB "Stage 1> Stage 2 not found!", ENDL
stage2_file_name: DB "STAGE2  BIN"
stage2_cluster: DW 0

;
;   CONSTANTS
;
STAGE2_LOAD_SEG EQU 0x0000
STAGE2_LOAD_OFF EQU 0x0500


TIMES 510-($-$$) DB 0
DW 0xAA55
BUFFER: