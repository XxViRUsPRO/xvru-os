#include <types.h>

typedef void (*KernelEntry)(void);

void kernel_entry(void)
{
    u8 *video_memory = (u8 *)0xB8000;
    for (u32 i = 0; i < 80 * 25 * 2; i += 2)
    {
        video_memory[i] = ' ';
        video_memory[i + 1] = 0x0F;
    }
    char *str = "Hello, World!";
    for (u32 i = 0; str[i] != '\0'; i++)
    {
        video_memory[i * 2] = str[i];
    }
}

void __attribute__((cdecl)) start(void)
{
    // Call the main function of the kernel
    KernelEntry kmain = (KernelEntry)kernel_entry;
    kmain();
}
