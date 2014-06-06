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
CUR-DIR := $(shell pwd)
 
# build rules
all: kernel.img
 
include $(wildcard *.d)

libhw.a:
	$(ARMGNU)-gcc $(CFLAGS) -c hw/uart.c
	$(ARMGNU)-gcc $(CFLAGS) -c hw/mailbox.asm.s
	$(ARMGNU)-gcc $(CFLAGS) -c hw/mailbox.c
	$(ARMGNU)-gcc $(CFLAGS) -c hw/framebuffer.c
	$(ARMGNU)-ar rvs libhw.a uart.o mailbox.asm.o framebuffer.o mailbox.o
	cp libhw.a lib
	
	

interrupts.o:
	

rpi.boot.o: 
	$(ARMGNU)-gcc $(CFLAGS) -c boot/rpi.boot.S

libhal.a : 
	$(ARMGNU)-gcc $(CFLAGS) -c hal/hal.c
	$(ARMGNU)-gcc $(CFLAGS) -c hal/interrupts.c
	$(ARMGNU)-gcc $(CFLAGS) -c hal/util.rpi.c
	$(ARMGNU)-ar rvs libhal.a hal.o interrupts.o util.rpi.o
	cp libhal.a lib

main.o:
	$(ARMGNU)-gcc $(CFLAGS) -c main.c 
 
kernel.elf: main.o rpi.boot.o  libhw.a libhal.a 
	$(ARMGNU)-ld --verbose -L$(CUR-DIR)/lib rpi.boot.o main.o -lhal -lhw  -Tboot/rpi.link-arm-eabi.ld -o $@  
	mv *.o obj
 
kernel.img: kernel.elf
	$(ARMGNU)-objcopy kernel.elf -O binary kernel.img
 
clean:
	$(RM) -f $(OBJS) kernel.elf kernel.img
	$(RM) -f *.d
	$(RM) -f *.o
	$(RM) -f *.a
 
dist-clean: clean
	$(RM) -f *.d
	$(RM) -f *.o
 