ASM?=nasm
BUILD_DIR?=build
SRC_DIR?=src
DIR=bootloader

MAIN_ASM=boot.asm
SOURCES_INCLUDES=$(shell find -name '*.inc')

.PHONY: all clean

all: bootloader

bootloader: $(BUILD_DIR)/$(DIR)/bootloader.bin 

$(BUILD_DIR)/$(DIR)/bootloader.bin: $(MAIN_ASM) $(SOURCES_INCLUDES) always
	@echo "Assembling \e[33m\e[4m$<\e[0m..."
	@$(ASM) $(MAIN_ASM) -f bin -o $(BUILD_DIR)/$(DIR)/bootloader.bin
	

always:
	@mkdir -p $(BUILD_DIR)/$(DIR)

clean:
	@rm -rf $(BUILD_DIR)/$(DIR)/*