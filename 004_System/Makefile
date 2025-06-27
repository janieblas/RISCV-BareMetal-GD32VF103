# GCC toolchain programs.
CC = riscv64-unknown-elf-gcc

# Common RISC-V architecture flags
ARCH_FLAGS = -march=rv32imac_zicsr -mabi=ilp32 -mcmodel=medlow

# Include directories and board definition
INCLUDE_DIRS = -IFirmware/Include
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
HEADERS = Firmware/Include/gd32vf103.h Firmware/Include/gd32vf103_rcu.h Firmware/Include/gd32vf103_gpio.h

# Object files to build
OBJS = gd32vf103xb_boot.o main.o gd32vf103_rcu.o gd32vf103_gpio.o system_gd32vf103.o

# Disable implicit rules
.SUFFIXES:

# Default rule - only build ELF
.PHONY: all
all: main.elf

# Explicit rules for each object file
gd32vf103xb_boot.o: gd32vf103xb_boot.S $(HEADERS)
	$(CC) -x assembler-with-cpp $(ASFLAGS) gd32vf103xb_boot.S -o gd32vf103xb_boot.o

main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) main.c -o main.o

gd32vf103_rcu.o: Firmware/Src/gd32vf103_rcu.c $(HEADERS)
	$(CC) $(CFLAGS) Firmware/Src/gd32vf103_rcu.c -o gd32vf103_rcu.o

gd32vf103_gpio.o: Firmware/Src/gd32vf103_gpio.c $(HEADERS)
	$(CC) $(CFLAGS) Firmware/Src/gd32vf103_gpio.c -o gd32vf103_gpio.o

system_gd32vf103.o: Firmware/Src/system_gd32vf103.c $(HEADERS)
	$(CC) $(CFLAGS) Firmware/Src/system_gd32vf103.c -o system_gd32vf103.o

# Rule to create an ELF file from the compiled object files.
main.elf: $(OBJS)
	$(CC) $(OBJS) $(LFLAGS) -o main.elf

# Rule to flash the ELF file to the microcontroller
.PHONY: flash
flash: main.elf
	sudo openocd -f interface/ftdi/openocd_ft2232.cfg -f target/gd32vf103-with-reset-run-improved.cfg -c "init; reset halt; program main.elf verify reset exit"

# Rule to clear out generated build files.
.PHONY: clean
clean:
	rm -f *.o
	rm -f main.elf