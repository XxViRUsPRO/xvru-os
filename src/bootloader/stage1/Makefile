
.PHONY: all clean

all: stage1

stage1: $(BUILD_DIR)/stage1.bin

$(BUILD_DIR)/stage1.bin: boot.asm
	@echo "Assembling \e[33m\e[4m$<\e[0m..."
	@mkdir -p $(@D)
	@$(TARGET_ASM) $< -f bin -o $@

clean:
	@rm -f $(BUILD_DIR)/stage1.bin