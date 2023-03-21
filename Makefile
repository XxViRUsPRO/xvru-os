BUILD_DIR=build
SRC_DIR=src
MAKEARGS=-j$(shell nproc) --no-print-directory

.PHONY: all bootloader kernel clean always debug run os

all: os run

os: $(BUILD_DIR)/os.img

$(BUILD_DIR)/os.img: bootloader kernel
	@echo "\e[94mCreating OS image...\e[0m"
	@cat $(BUILD_DIR)/bootloader/bootloader.bin $(BUILD_DIR)/kernel/kernel.bin > $(BUILD_DIR)/os.img
	@echo "\e[32mDone!\e[0m\n"

bootloader: $(BUILD_DIR)/bootloader/bootloader.bin 

$(BUILD_DIR)/bootloader/bootloader.bin: always
	@echo "\e[35mBuilding bootloader...\e[0m"
	@$(MAKE) -C $(SRC_DIR)/bootloader BUILD_DIR=$(abspath $(BUILD_DIR)) $(MAKEARGS)
	@echo "\e[32mDone!\e[0m\n"

kernel: $(BUILD_DIR)/kernel/kernel.bin

$(BUILD_DIR)/kernel/kernel.bin: always
	@echo "\e[35mBuilding kernel...\e[0m"
	@$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) $(MAKEARGS)
	@echo "\e[32mDone!\e[0m\n"

run: os
	@echo "\e[32mRunning...\e[0m"
	@qemu-system-i386 -hda build/os.img

always:
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "\e[31mCleaning...\e[0m"
	@$(MAKE) -C $(SRC_DIR)/bootloader BUILD_DIR=$(abspath $(BUILD_DIR)) clean $(MAKEARGS)
	@$(MAKE) -C $(SRC_DIR)/kernel BUILD_DIR=$(abspath $(BUILD_DIR)) clean $(MAKEARGS)
	@rm -f $(BUILD_DIR)/os.img
	@echo "\e[32mDone!\e[0m\n"