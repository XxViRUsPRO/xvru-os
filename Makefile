include config.mk

.PHONY: all bootloader kernel clean always debug run os

all: os run

include toolchain.mk

os: $(BUILD_DIR)/os.img

$(BUILD_DIR)/os.img: bootloader kernel
	@echo "\e[94mCreating OS image...\e[0m"
	@dd if=/dev/zero of=$@ bs=512 count=2880 > /dev/null
	@mkfs.fat -F 12 -n "OS" $@ > /dev/null
	@dd if=$(BUILD_DIR)/bootloader/stage1.bin of=$@ conv=notrunc > /dev/null
	@mcopy -i $@ $(BUILD_DIR)/bootloader/stage2.bin "::stage2.bin"
	@mcopy -i $@ $(BUILD_DIR)/kernel/kernel.bin "::kernel.bin"
	@echo "\e[32mDone!\e[0m\n"

bootloader: stage1 stage2

stage1: $(BUILD_DIR)/bootloader/stage1.bin

$(BUILD_DIR)/bootloader/stage1.bin: always
	@echo "\e[35mBuilding stage1 bootloader...\e[0m"
	@$(MAKE) -C src/bootloader/stage1 BUILD_DIR=$(abspath $(BUILD_DIR)/bootloader) $(MAKEARGS)
	@echo "\e[32mDone!\e[0m\n"

stage2: $(BUILD_DIR)/bootloader/stage2.bin

$(BUILD_DIR)/bootloader/stage2.bin: always
	@echo "\e[35mBuilding stage2 bootloader...\e[0m"
	@$(MAKE) -C src/bootloader/stage2 BUILD_DIR=$(abspath $(BUILD_DIR)/bootloader) $(MAKEARGS)
	@echo "\e[32mDone!\e[0m\n"

kernel: $(BUILD_DIR)/kernel/kernel.bin

$(BUILD_DIR)/kernel/kernel.bin: always
	@echo "\e[35mBuilding kernel...\e[0m"
	@$(MAKE) -C src/kernel BUILD_DIR=$(abspath $(BUILD_DIR)/kernel) $(MAKEARGS)
	@echo "\e[32mDone!\e[0m\n"

run: os
	@echo "\e[32mRunning...\e[0m"
	@qemu-system-i386 -fda build/os.img -vga std

debug: os
	@echo "\e[32mDebug...\e[0m"
	@./utils/scripts/bochs.sh $(BUILD_DIR)/os.img

always:
	@mkdir -p $(BUILD_DIR)

clean:
	@echo "\e[31mCleaning...\e[0m"
	@rm -rf $(BUILD_DIR)/*
	@echo "\e[32mDone!\e[0m\n"