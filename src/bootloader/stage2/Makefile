ASM?=nasm
ASMFLAGS?=-f elf
GCC?=/usr/local/i386elfgcc/bin/i386-elf-gcc
GCCFLAGS?=-ffreestanding -m32 -g -nostdlib -I. -I../../include
LD?=/usr/local/i386elfgcc/bin/i386-elf-ld
LDFLAGS?=-T linker.ld -melf_i386
BUILD_DIR?=build
SRC_DIR?=src
DIR=stage2

SOURCES_C=$(shell find . -type f -name '*.c')
SOURCES_ASM=$(shell find . -type f -name '*.asm')
SOURCES_HEADERS=$(shell find . -type f -name '*.h')
SOURCES_INCLUDES=$(shell find . -type f -name '*.inc')
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/$(DIR)/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/$(DIR)/asm/%.o, $(SOURCES_ASM))

.PHONY: all clean

all: stage2

stage2: $(BUILD_DIR)/$(DIR)/stage2.bin

$(BUILD_DIR)/$(DIR)/stage2.bin: $(OBJECTS_C) $(OBJECTS_ASM)
	@$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(DIR)/stage2.bin $(OBJECTS_C) $(OBJECTS_ASM)

$(BUILD_DIR)/$(DIR)/c/%.o: %.c $(SOURCES_HEADERS)
	@echo "Compiling \e[33m\e[4m$<\e[0m..."
	@mkdir -p $(@D)
	@$(GCC) $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/$(DIR)/asm/%.o: %.asm $(SOURCES_INCLUDES)
	@echo "Assembling \e[33m\e[4m$<\e[0m..."
	@mkdir -p $(@D)
	@$(ASM) $(ASMFLAGS) -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)/$(DIR)/*