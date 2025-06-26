# GCC toolchain programs.
CC = riscv64-unknown-elf-gcc

# Common RISC-V architecture flags
ARCH_FLAGS = -march=rv32imac_zicsr -mabi=ilp32 -mcmodel=medlow

# Include directories and board definition
INCLUDE_DIRS = -IInclude
BOARD_DEF = -DGD32VF103C_START  # Define board type for 8MHz crystal

# Common compilation flags
COMMON_FLAGS = -Wall -O0 -fmessage-length=0 $(ARCH_FLAGS) $(INCLUDE_DIRS) $(BOARD_DEF)

# Assembly directives
ASFLAGS = -c $(COMMON_FLAGS)

# C compilation directives
CFLAGS = -c -g -fno-builtin -ffreestanding $(COMMON_FLAGS)

# Linker directives
LFLAGS = -Wall -Wl,--no-relax -Wl,--gc-sections -nostdlib -nostartfiles -lgcc $(ARCH_FLAGS) -T gd32vf103xb.ld

# Header files (dependencies)
HEADERS = Include/gd32vf103.h Include/gd32vf103_rcu.h Include/gd32vf103_gpio.h

# Source files
AS_SRC  = Src/gd32vf103xb_boot.S
C_SRC   = Src/main.c Src/gd32vf103_rcu.c Src/gd32vf103_gpio.c

# Object files to build
OBJS = gd32vf103xb_boot.o main.o gd32vf103_rcu.o gd32vf103_gpio.o

# Default rule - only build ELF
.PHONY: all
all: main.elf

# Assembly file compilation
gd32vf103xb_boot.o: Src/gd32vf103xb_boot.S $(HEADERS)
	$(CC) -x assembler-with-cpp $(ASFLAGS) $< -o $@

# C file compilation
main.o: Src/main.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

gd32vf103_rcu.o: Src/gd32vf103_rcu.c $(HEADERS)
	$(CC) $(CFLAGS) $< -o $@

gd32vf103_gpio.o: Src/gd32vf103_gpio.c $(HEADERS)
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
	rm -f *.o
	rm -f main.elf
