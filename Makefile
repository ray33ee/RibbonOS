# ARM g++ compiler
GCC=arm-none-eabi-g++

# Add Pi specific options
ifeq ($(RASPI_MODEL), 1)
	CPU = arm1176jzf-s
	DIRECTIVES = -D MODEL_1
	IMG_NAME = kernel.img
	QEMU_PI = raspi1
else
	CPU = cortex-a7
	IMG_NAME = kernel7.img
	QEMU_PI = raspi2
endif

ELF_NAME = RibbonOS.elf
VM_SIZE = 256

# Defines the DEBUG preprosessor directive
DEBUG = -D DEBUG

# Flags for compiling, linking and emulator
CFLAGS = -mcpu=$(CPU) -fpic -ffreestanding -fno-exceptions -std=c++14 $(DIRECTIVES) $(DEBUG)
SRCFLAGS = -O2 -Wall -Wextra
LINK_FLAGS = -ffreestanding -O2 -nostdlib -fno-exceptions -lgcc
QEMU_FLAGS = -m $(VM_SIZE) -M $(QEMU_PI) -serial stdio -kernel

# Folder locations
KERNEL_SOURCE_DIR = src/kernel
KERNEL_HEADER_DIR = include
COMMON_SOURCE_DIR = src/common
OBJ_DIR = obj
BIN_DIR = bin
BUILD_DIR = build

#File lists
KERNEL_FILES = $(wildcard $(KERNEL_SOURCE_DIR)/*.cpp)
COMMON_FILES = $(wildcard $(COMMON_SOURCE_DIR)/*.cpp)
ASM_FILES = $(wildcard $(KERNEL_SOURCE_DIR)/*.s)

OBJECTS = $(patsubst $(KERNEL_SOURCE_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(KERNEL_FILES))
OBJECTS += $(patsubst $(COMMON_SOURCE_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(COMMON_FILES))
OBJECTS += $(patsubst $(KERNEL_SOURCE_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_FILES))

HEADERS = $(wildcard $(KERNEL_HEADER_DIR)/*.h)

build: $(OBJECTS) $(HEADERS)
	@echo ========================================
	@echo                  LINKING
	@echo ========================================
	mkdir -p $(BIN_DIR)
	$(GCC) -T $(BUILD_DIR)/linker.ld -o $(BIN_DIR)/$(ELF_NAME) $(LINK_FLAGS) $(OBJECTS)
	arm-none-eabi-objcopy $(BIN_DIR)/$(ELF_NAME) -O binary $(BIN_DIR)/$(IMG_NAME)


$(OBJ_DIR)/%.o: $(KERNEL_SOURCE_DIR)/%.cpp
	@echo ========================================
	@echo                  COMPILING $<
	@echo ========================================
	mkdir -p $(OBJ_DIR)
	$(GCC) $(CFLAGS) -I$(KERNEL_SOURCE_DIR) -I$(KERNEL_HEADER_DIR) -c $< -o $@ $(CSRCFLAGS)

$(OBJ_DIR)/%.o: $(KERNEL_SOURCE_DIR)/%.s
	@echo ========================================
	@echo                  COMPILING $<
	@echo ========================================
	mkdir -p $(OBJ_DIR)
	$(GCC) $(CFLAGS) -I$(KERNEL_SOURCE_DIR) -c $< -o $@

$(OBJ_DIR)/%.o: $(COMMON_SOURCE_DIR)/%.cpp
	@echo ========================================
	@echo                  COMPILING $<
	@echo ========================================
	mkdir -p $(OBJ_DIR)
	$(GCC) $(CFLAGS) -I$(KERNEL_SOURCE_DIR) -I$(KERNEL_HEADER_DIR) -c $< -o $@ $(CSRCFLAGS)

clean: 
	@echo ========================================
	@echo                  CLEANING
	@echo ========================================
	rm -f $(BIN_DIR)/*.img
	rm -f $(BIN_DIR)/*.elf
	rm -f $(OBJ_DIR)/*.o

rebuild: clean build
	@echo ========================================
	@echo                  REBUILD
	@echo ========================================

run:
	@echo ========================================
	@echo                  RUN
	@echo ========================================
	qemu-system-arm $(QEMU_FLAGS) $(BIN_DIR)/$(ELF_NAME)
