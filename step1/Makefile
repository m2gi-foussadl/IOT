
# Copyright: Olivier Gruber (olivier dot gruber at acm dot org)
#
# This program is free software: you can redistribute it and/or modify it under the terms
# of the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
# without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program.
# If not, see <https://www.gnu.org/licenses/>.

#-----------------------------------------------
# This is the configurable part of the makefile:
#-----------------------------------------------
BOARD=versatile
CPU=cortex-a8
TOOLCHAIN=arm-none-eabi
DEBUG?=yes
BUILD=build/

objs= startup.o main.o exception.o uart.o

#===================================================================
# This is the non-configurable part of the makefile,
#===================================================================

OBJS = $(addprefix $(BUILD), $(objs)) 

ifeq ($(BOARD),versatile)
  # QEMU configuration arguments:
  VGA=-nographic
  SERIAL=-serial mon:stdio
  # memory size in kilo bytes.
  MEMSIZE=32
  MEMORY="$(MEMSIZE)K"  
  MACHINE=versatileab  
  # QEMU executable and QEMU arguments
  QEMU=qemu-system-arm
  QEMU_ARGS=-M $(MACHINE) -cpu $(CPU) -m $(MEMORY) $(VGA) $(SERIAL)
  # compilation and linking flags:
  CFLAGS= -c -mcpu=$(CPU) -nostdlib -ffreestanding 
  CFLAGS+=-DCPU=$(CPU) -DMEMORY="($(MEMSIZE)*1024)"  
  ASFLAGS= -mcpu=$(CPU) 
  LDFLAGS= -T kernel.ld -nostdlib -static
endif

ifeq ($(DEBUG),yes)
  CFLAGS+= -ggdb
  LDFLAGS+= -g
  ASFLAGS+= -g
endif

ifndef MACHINE
  $(error Must choose a board (e.g. Versatile AB or PB)) 
endif

#-------------------------------------------------------------------
# Compilation Rules
#-------------------------------------------------------------------
$(BUILD)%.o: %.c
	$(TOOLCHAIN)-gcc $(CFLAGS) -o $@ $<

$(BUILD)%.o: %.s
	$(TOOLCHAIN)-as $(ASFLAGS) -o $@ $<

#-------------------------------------------------------------------
# Build and link all
# Notice that we link with our own linker script: test.ld
#-------------------------------------------------------------------
all: build $(BUILD)kernel.elf $(BUILD)kernel.bin

$(BUILD)kernel.elf: $(OBJS)
	$(TOOLCHAIN)-ld $(LDFLAGS) $(OBJS) -o $(BUILD)kernel.elf

$(BUILD)kernel.bin: $(BUILD)kernel.elf
	$(TOOLCHAIN)-objcopy -O binary $(BUILD)kernel.elf $(BUILD)kernel.bin 

build:
	@mkdir $(BUILD)
	
clean: 
	rm -rf $(BUILD) 

#-------------------------------------------------------------------
# Targets to launch QEMU: running standalone or under gdb's control
#-------------------------------------------------------------------
ifeq ($(BOARD),versatile)
run: all
	@echo "\n\nBoard: Versatile Board...\n"
	$(QEMU) $(QEMU_ARGS) -device loader,file=$(BUILD)kernel.elf

debug: all
	@echo "\n\nBoard: Versatile Board...\n"
	$(QEMU) $(QEMU_ARGS) -device loader,file=$(BUILD)kernel.elf -gdb tcp::1234 -S
# -cpu $(CPU)

endif
