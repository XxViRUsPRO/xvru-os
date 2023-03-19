BUILD_DIR=build
SRC_DIR=src

.PHONY: all bootloader clean

all: os

os: $(BUILD_DIR)/os.img

$(BUILD_DIR)/os.img: bootloader kernel
	cat $(BUILD_DIR)/bootloader/bootloader.bin $(BUILD_DIR)/kernel/kernel.bin > $(BUILD_DIR)/os.img

bootloader: $(BUILD_DIR)/bootloader/bootloader.bin 

$(BUILD_DIR)/bootloader/bootloader.bin: always
	$(MAKE) -C $(SRC_DIR)/bootloader BUILD_DIR=$(abspath $(BUILD_DIR))

kernel: $(BUILD_DIR)/kernel/kernel.bin

$(BUILD_DIR)/kernel/kernel.bin: always
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR))


always:
	mkdir -p $(BUILD_DIR)

clean:
	$(MAKE) -C $(SRC_DIR)/bootloader BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) clean
	rm -rf $(BUILD_DIR)/*
