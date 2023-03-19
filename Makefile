BUILD_DIR=build
SRC_DIR=src

.PHONY: all bootloader kernel clean always debug run os

all: os run

os: $(BUILD_DIR)/os.img

$(BUILD_DIR)/os.img: bootloader kernel
	cat $(BUILD_DIR)/bootloader/bootloader.bin $(BUILD_DIR)/kernel/kernel.bin > $(BUILD_DIR)/os.img

bootloader: $(BUILD_DIR)/bootloader/bootloader.bin 

$(BUILD_DIR)/bootloader/bootloader.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader BUILD_DIR=$(abspath $(BUILD_DIR))

kernel: $(BUILD_DIR)/kernel/kernel.bin

$(BUILD_DIR)/kernel/kernel.bin: always
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR))

run: os
	qemu-system-i386 -hda build/os.img

always:
	mkdir -p $(BUILD_DIR)

clean:
	$(MAKE) -C $(SRC_DIR)/bootloader BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*
