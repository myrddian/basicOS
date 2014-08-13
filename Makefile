# Makefile - build script */
 
# build environment
# PREFIX ?= /Users/myrddian/arm-cs-tools
# ARMGNU ?= $(PREFIX)/bin/arm-none-eabi
ARMGNU ?= armv6j-hardfloat-linux-gnueabi

X86GNU ?= gcc
ARMCPUFLAG := -mcpu=arm1176jzf-s
X86CPUFLAG := -m32
 
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
INCLUDES    := -O0 -I hal -I hw -I mem -I sched -I shell -I lib
BASEFLAGS   := -fPIC -DPIC -pedantic -pedantic-errors -nostdlib -shared -Bsymbolic
BASEFLAGS   += -ffreestanding -fomit-frame-pointer -z defs
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
OBJ_DIR := obj
	
ARMCPUFLAG += -DR_PI	
X86CPUFLAG += -DX86
 
# build rules
all: kernel_rpi.img kernel_x86.img
 
include $(wildcard *.d)


libhwx86.a:
	

libosx86.a:
	$(X86GNU) $(CFLAGS) $(X86CPUFLAG)  -c lib/util.c
	ar rvs libbosx86.a util.o
	mv libbosx86.a $(OBJ_DIR)
	
libhalx86.a:
	$(X86GNU) $(CFLAGS) $(X86CPUFLAG)  -c hal/hal.c
	$(X86GNU) $(CFLAGS) $(X86CPUFLAG)  -c hal/x86/interrupts.c
	nasm -g -felf  hal/x86/assembler.S
	ar rvs libhalx86.a hal.o interrupts.o hal/x86/assembler.o
	mv libhalx86.a $(OBJ_DIR)
	
main_x86.o:
	$(X86GNU) $(CFLAGS) $(X86CPUFLAG)  -c main.c 
	mv main.o main_x86.o
	
boot.x86.o:
	as boot/boot.x86.S
	
	

kernel_x86.elf: main_x86.o libosx86.a libosx86.a libhalx86.a libhwx86.a boot.x86.o

kernel_x86.img: kernel_x86.elf
	




libhwrpi.a:
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hw/rpi/uart.c 
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hw/rpi/mailbox.asm.s
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hw/rpi/mailbox.c
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hw/rpi/framebuffer.c
	$(ARMGNU)-ar rvs libhwrpi.a uart.o mailbox.asm.o framebuffer.o mailbox.o
	mv libhwrpi.a $(OBJ_DIR)
	

libbosrpi.a:
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c lib/util.c
	$(ARMGNU)-ar rvs libbosrpi.a util.o
	mv libbosrpi.a $(OBJ_DIR)
	

rpi.boot.o: 
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c boot/rpi.boot.S

libhalrpi.a : 
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hal/hal.c
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hal/rpi/interrupts.c
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hal/rpi/drivers.c
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c hal/rpi/entry_point.c
	$(ARMGNU)-ar rvs libhalrpi.a hal.o interrupts.o entry_point.o drivers.o
	mv libhalrpi.a $(OBJ_DIR)

main_rpi.o:
	$(ARMGNU)-gcc $(CFLAGS) $(ARMCPUFLAG)  -c main.c 
	mv main.o main_rpi.o
 
kernel.rpi.elf: main_rpi.o rpi.boot.o  libhwrpi.a libhalrpi.a libbosrpi.a
	$(ARMGNU)-ld -L$(CUR-DIR)/$(OBJ_DIR) rpi.boot.o main_rpi.o -lhalrpi -lhwrpi -lbosrpi -Tboot/rpi.link-arm-eabi.ld -o $@  
	mv *.o obj
	mv *.d debug
 
kernel_rpi.img: kernel.rpi.elf
	$(ARMGNU)-objcopy kernel.rpi.elf -O binary kernel_rpi.img
 
clean:
	$(RM) -f $(OBJS) kernel.elf kernel.img
	$(RM) -f *.d
	$(RM) -f *.o
	$(RM) -f *.a
	$(RM) -f obj/*.o
	$(RM) -f boot/*.o
	$(RM) -f obj/*.a
 
dist-clean: clean
	$(RM) -f *.d
	$(RM) -f *.o
 