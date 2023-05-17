TARGET_CFLAGS += -ffreestanding -nostdlib -I.

SOURCES_C=$(shell find . -type f -name '*.c')
SOURCES_HEADERS=$(shell find . -type f -name '*.h')
OBJECTS_C=$(patsubst %.c, $(BUILD_DIR)/%.o, $(SOURCES_C))

.PHONY: all clean

all: libs

libs: $(OBJECTS_C) $(OBJECTS_ASM)

$(BUILD_DIR)/%.o: %.c $(SOURCES_HEADERS)
	@echo "Compiling \e[33m\e[4m$<\e[0m..."
	@mkdir -p $(@D)
	@$(TARGET_CC) $(TARGET_CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR)/*