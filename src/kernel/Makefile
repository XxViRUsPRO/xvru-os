ASM?=nasm
ASMFLAGS?=-f elf
GCC?=/usr/local/i386elfgcc/bin/i386-elf-gcc
GCCFLAGS?=-ffreestanding -m32 -g
LD?=/usr/local/i386elfgcc/bin/i386-elf-ld
LDFLAGS?=-T linker.ld -melf_i386
BUILD_DIR?=build
SRC_DIR?=src
DIR=kernel

SOURCES_CPP=$(wildcard *.cpp)
SOURCES_ASM=$(wildcard *.asm)
OBJECTS_CPP=$(patsubst %.cpp, $(BUILD_DIR)/$(DIR)/cpp/%.o, $(SOURCES_CPP))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/$(DIR)/asm/%.o, $(SOURCES_ASM))

.PHONY: all clean

all: kernel

kernel: $(BUILD_DIR)/$(DIR)/kernel.bin 

$(BUILD_DIR)/$(DIR)/kernel.bin: $(OBJECTS_CPP) $(OBJECTS_ASM) 
	$(LD) $(LDFLAGS) -o $(BUILD_DIR)/$(DIR)/kernel.bin $(OBJECTS_CPP) $(OBJECTS_ASM)

$(BUILD_DIR)/$(DIR)/cpp/%.o: %.cpp always
	$(GCC) $(GCCFLAGS) -c $< -o $@

$(BUILD_DIR)/$(DIR)/asm/%.o: %.asm always
	$(ASM) $(ASMFLAGS) -o $@ $<


always:
	mkdir -p $(BUILD_DIR)/$(DIR)/cpp
	mkdir -p $(BUILD_DIR)/$(DIR)/asm

clearn:
	rm -rf $(BUILD_DIR)/$(DIR)/*