#include "gdt.h"

typedef struct
{
    u16 LimitLow;
    u16 BaseLow;
    u8 BaseMid;
    u8 Access;
    u8 FlagsLimitHigh;
    u8 BaseHigh;
} __attribute__((packed)) GDT_Entry;

typedef struct
{
    u16 Limit;
    GDT_Entry *Ptr;
} __attribute__((packed)) GDT_Descriptor;

typedef enum
{
    GDT_ACCESS_CODE_READABLE = 0x02,
    GDT_ACCESS_DATA_WRITEABLE = 0x02,

    GDT_ACCESS_CODE_CONFORMING = 0x04,
    GDT_ACCESS_DATA_DIRECTION_NORMAL = 0x00,
    GDT_ACCESS_CODE_DIRECTION_DOWN = 0x04,

    GDT_ACCESS_DATA_SEGMENT = 0x10,
    GDT_ACCESS_CODE_SEGMENT = 0x18,

    GDT_ACCESS_DESCRIPTOR_TSS = 0x00,

    GDT_ACCESS_RING0 = 0x00,
    GDT_ACCESS_RING1 = 0x20,
    GDT_ACCESS_RING2 = 0x40,
    GDT_ACCESS_RING3 = 0x60,

    GDT_ACCESS_PRESENT = 0x80,
} GDT_ACCESS;

typedef enum
{
    GDT_FLAG_64BIT = 0x20,
    GDT_FLAG_32BIT = 0x40,
    GDT_FLAG_16BIT = 0x00,

    GDT_FLAG_GRANULARITY_1B = 0x00,
    GDT_FLAG_GRANULARITY_4KB = 0x80,
} GDT_FLAG;

#define GDT_LIMIT_LOW(limit) ((limit)&0xFFFF)
#define GDT_BASE_LOW(base) ((base)&0xFFFF)
#define GDT_BASE_MID(base) (((base) >> 16) & 0xFF)
#define GDT_BASE_HIGH(base) (((base) >> 24) & 0xFF)
#define GDT_FLAGS_LIMIT_HIGH(flags, limit) (((flags)&0xF0) | (((limit) >> 16) & 0x0F))

#define GDT_ENTRY(base, limit, access, flags)   \
    {                                           \
        GDT_LIMIT_LOW(limit),                   \
            GDT_BASE_LOW(base),                 \
            GDT_BASE_MID(base),                 \
            access,                             \
            GDT_FLAGS_LIMIT_HIGH(flags, limit), \
            GDT_BASE_HIGH(base)                 \
    }

GDT_Entry gdt[] = {
    GDT_ENTRY(0, 0, 0, 0),                                                                                                                                                                               // Null segment
    GDT_ENTRY(0, 0xFFFFFFFF, GDT_ACCESS_CODE_SEGMENT | GDT_ACCESS_CODE_READABLE | GDT_ACCESS_CODE_CONFORMING | GDT_ACCESS_RING0 | GDT_ACCESS_PRESENT, GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4KB),        // Kernel 32bit code segment
    GDT_ENTRY(0, 0xFFFFFFFF, GDT_ACCESS_DATA_SEGMENT | GDT_ACCESS_DATA_WRITEABLE | GDT_ACCESS_DATA_DIRECTION_NORMAL | GDT_ACCESS_RING0 | GDT_ACCESS_PRESENT, GDT_FLAG_32BIT | GDT_FLAG_GRANULARITY_4KB), // Kernel 32bit data segment
};

GDT_Descriptor gdt_descriptor = {
    sizeof(gdt) - 1,
    gdt};

void __attribute__((cdecl)) GDT_load(GDT_Descriptor *gdt_descriptor, u16 code_segment, u16 data_segment);

void GDT_init(void)
{
    GDT_load(&gdt_descriptor, GDT_CODE_SEG, GDT_DATA_SEG);
}