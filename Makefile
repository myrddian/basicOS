# Makefile - build script */
 
# build environment
PREFIX ?= /Users/myrddian/arm-cs-tools
ARMGNU ?= $(PREFIX)/bin/arm-none-eabi
 
# source files
SOURCES_ASM := $(wildcard *.S)
#SOURCES_ASM += hal/$(wildcard *.S)
#SOURCES_ASM += boot/$(wildcard *.S)
SOURCES_C   := $(wildcard *.c)
#SOURCES_C   += hal/$(wildcard *.c)
#SOURCES_C   += hw/$(wildcard *.c)
#SOURCES_C   += mem/$(wildcard *.c)
#SOURCES_C   += sched/$(wildcard *.c)
#SOURCES_C   += shell/$(wildcard *.c)
 
# object files
OBJS        := $(patsubst %.S,%.o,$(SOURCES_ASM))
OBJS        += $(patsubst %.c,%.o,$(SOURCES_C))
 
# Build flags
DEPENDFLAGS := -MD -MP
INCLUDES    := -I hal -I hw -I mem -I sched -I shell 
BASEFLAGS   := -O2 -fpic -pedantic -pedantic-errors -nostdlib
BASEFLAGS   += -ffreestanding -fomit-frame-pointer -mcpu=arm1176jzf-s
WARNFLAGS   := -Wall -Wextra -Wshadow -Wcast-align -Wwrite-strings
WARNFLAGS   += -Wredundant-decls -Winline
WARNFLAGS   += -Wno-attributes -Wno-deprecated-declarations
WARNFLAGS   += -Wno-div-by-zero -Wno-endif-labels -Wfloat-equal
WARNFLAGS   += -Wformat=2 -Wno-format-extra-args -Winit-self
WARNFLAGS   += -Winvalid-pch -Wmissing-format-attribute
WARNFLAGS   += -Wmissing-include-dirs -Wno-multichar
WARNFLAGS   += -Wredundant-decls -Wshadow
WARNFLAGS   += -Wno-sign-compare -Wswitch -Wsystem-headers -Wundef
WARNFLAGS   += -Wno-pragmas -Wno-unused-but-set-parameter
WARNFLAGS   += -Wno-unused-but-set-variable -Wno-unused-result
WARNFLAGS   += -Wwrite-strings -Wdisabled-optimization -Wpointer-arith
WARNFLAGS   += -Werror
ASFLAGS     := $(INCLUDES) $(DEPENDFLAGS) -D__ASSEMBLY__
CFLAGS      := $(INCLUDES) $(DEPENDFLAGS) $(BASEFLAGS) $(WARNFLAGS)
CFLAGS      += -std=gnu99 -ggdb
 
# build rules
all: kernel.img
 
include $(wildcard *.d)


rpi.boot.o: 
	$(ARMGNU)-gcc $(CFLAGS) -c boot/rpi.boot.S

hal.o : 
	$(ARMGNU)-gcc $(CFLAGS) -c hal/hal.c  hal/drivers.c 
	
util.rpi.o:
	$(ARMGNU)-gcc $(CFLAGS) -c hal/util.rpi.c

uart.o : 
	$(ARMGNU)-gcc $(CFLAGS) -c hw/uart.c 	

main.o:
	$(ARMGNU)-gcc $(CFLAGS) -c main.c 
 
kernel.elf: main.o hal.o uart.o util.rpi.o rpi.boot.o
	$(ARMGNU)-ld rpi.boot.o uart.o hal.o main.o util.rpi.o -Tboot/rpi.link-arm-eabi.ld -o $@  
 
kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
 
clean:
	$(RM) -f $(OBJS) kernel.elf kernel.img
	$(RM) -f *.d
	$(RM) -f *.o
 
dist-clean: clean
	$(RM) -f *.d
	$(RM) -f *.o
 