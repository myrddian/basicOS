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

    
    void interrupt_init();
    
    void registerSysHandler(SysCallHandler *handle);
    
    void registerIRQHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r3));
    
    void registerUndefinedHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2));
    
    void registerFIQHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2));
    
    void registerDataAbortHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2));
    
    void registerPrefetchAbort(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2));
    
    uint32_t swiHandler(uint32_t r0, uint32_t r1, uint32_t r2);
    uint32_t IRQHandler(uint32_t r0, uint32_t r1, uint32_t r2);
    uint32_t UndefineHandler(uint32_t r0, uint32_t r1, uint32_t r2);
    uint32_t FIQHandler(uint32_t r0, uint32_t r1, uint32_t r2);
    uint32_t DataAbortHandler(uint32_t r0, uint32_t r1, uint32_t r2);
    uint32_t PrefetchAbortHandler(uint32_t r0, uint32_t r1, uint32_t r2);
    extern void enable_irq ( void );   

#ifdef	__cplusplus
}
#endif

#endif	/* INTERRUPTS_H */

