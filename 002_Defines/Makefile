# GCC toolchain programs.
CC = riscv64-unknown-elf-gcc

# Common RISC-V architecture flags
ARCH_FLAGS = -march=rv32imac_zicsr -mabi=ilp32 -mcmodel=medlow

# Include directories
INCLUDE_DIRS = -I.

# Common compilation flags
COMMON_FLAGS = -Wall -O0 -fmessage-length=0 $(ARCH_FLAGS) $(INCLUDE_DIRS)

# Assembly directives
ASFLAGS = -c $(COMMON_FLAGS)

# C compilation directives
CFLAGS = -c -g -fno-builtin -ffreestanding $(COMMON_FLAGS)

# Linker directives
LFLAGS = -Wall -Wl,--no-relax -Wl,--gc-sections -nostdlib -nostartfiles -lgcc $(ARCH_FLAGS) -T gd32vf103xb.ld

# Header files (dependencies)
HEADERS = gd32vf103.h 

# Source files
AS_SRC  = gd32vf103xb_boot.S
C_SRC   = main.c 

# Object files to build.
OBJS  = $(AS_SRC:.S=.o)
OBJS += $(C_SRC:.c=.o)

# Default rule - only build ELF
.PHONY: all
all: main.elf

# Rule to build assembly files.
%.o: %.S $(HEADERS)
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

# Rule to compile C files.
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

# Rule to create an ELF file from the compiled object files.
main.elf: $(OBJS)
	$(CC) $^ $(LFLAGS) -o $@

# Rule to flash the ELF file to the microcontroller
.PHONY: flash
flash: main.elf
	sudo openocd -f interface/ftdi/openocd_ft2232.cfg -f target/gd32vf103-with-reset-run-improved.cfg -c "init; reset halt; program main.elf verify reset exit"

# Rule to clear out generated build files.
.PHONY: clean
clean:
	rm -f $(OBJS)
	rm -f main.elf