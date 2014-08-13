/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <stdint.h>

#ifndef INTERRUPTS_H
#define	INTERRUPTS_H

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef R_PI
#include "rpi/hal.rpi.h"
#endif

#ifdef X86
#include "x86/interrupts.x86.h"
#include "x86/PIC_IRQ.h"
#endif


#define MAX_SYS_NAME 64
#define MAX_SYS_CALLS 64
#define SYS_FREE 0
#define SYS_USED  1
#define SYS_ERR 1
    
//#define INTERRUPT_HANDLER uint32_t *(handler)(uint32_t r0, uint32_t r1, uint32_t r2)
    
    
    typedef struct __sys_handler {
        uint32_t preferredLine;
        uint32_t sysStatus;
        char handlerName[MAX_SYS_NAME];
        uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2);
    }SysCallHandler;

    void registerSysHandler(SysCallHandler *handle);
    void interrupt_init();


    void enable_hw_interrupts();
    void disable_hw_interrupts();
    void hw_interrupt_end();

    void register_timer_interrupt(void (*handler)());
    void set_hw_timer_frequency(uint32_t frequency);
    void register_page_fault_hander();



    


#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTS_H */

