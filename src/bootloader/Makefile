ASM?=nasm
BUILD_DIR?=build
SRC_DIR?=src
DIR=bootloader

.PHONY: all clean

all: bootloader

bootloader: $(BUILD_DIR)/$(DIR)/bootloader.bin 

$(BUILD_DIR)/$(DIR)/bootloader.bin: always
	$(ASM) boot.asm -f bin -o $(BUILD_DIR)/$(DIR)/bootloader.bin
	

always:
	mkdir -p $(BUILD_DIR)/$(DIR)

clearn:
	rm -rf $(BUILD_DIR)/$(DIR)/*