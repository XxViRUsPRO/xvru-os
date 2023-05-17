TARGET_ASMFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I. -I$(ROOT_DIR)/src/libs
TARGET_LIBS += -lgcc
TARGET_LINKFLAGS += -T linker.ld -nostdlib

SOURCES_C=$(shell find . -type f -name '*.c')
SOURCES_ASM=$(shell find . -type f -name '*.asm')
SOURCES_HEADERS=$(shell find . -type f -name '*.h')
SOURCES_INCLUDES=$(shell find . -type f -name '*.inc')
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/c/%.o, $(SOURCES_C))
OBJECTS_ASM=$(patsubst %.asm, $(BUILD_DIR)/asm/%.o, $(SOURCES_ASM))
EXTERNAL_OBJECTS=$(shell find $(ROOT_DIR)/build/libs -type f -name '*.o')

.PHONY: all clean

all: kernel

kernel: $(BUILD_DIR)/kernel.bin

$(BUILD_DIR)/kernel.bin: $(OBJECTS_C) $(OBJECTS_ASM)
	@$(TARGET_LD) $(TARGET_LINKFLAGS) -Wl,-Map=$(BUILD_DIR)/kernel.map -o $@ $^ $(EXTERNAL_OBJECTS) $(TARGET_LIBS)

$(BUILD_DIR)/c/%.o: %.c $(SOURCES_HEADERS)
	@echo "Compiling \e[33m\e[4m$<\e[0m..."
	@mkdir -p $(@D)
	@$(TARGET_CC) $(TARGET_CFLAGS) -c $< -o $@

$(BUILD_DIR)/asm/%.o: %.asm $(SOURCES_INCLUDES)
	@echo "Assembling \e[33m\e[4m$<\e[0m..."
	@mkdir -p $(@D)
	@$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ $<

clean:
	@rm -rf $(BUILD_DIR)/*