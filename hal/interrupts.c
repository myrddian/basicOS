#include <interrupts.h>
#include <hal.h>

uint32_t (*irqhandler)(uint32_t r0, uint32_t r1, uint32_t r2);
uint32_t (*undefinehandler)(uint32_t r0, uint32_t r1, uint32_t r2);
uint32_t (*fiqhandler)(uint32_t r0, uint32_t r1, uint32_t r2);
uint32_t (*dataAbortHandler)(uint32_t r0, uint32_t r1, uint32_t r2);
uint32_t (*prefetchAbortHandler)(uint32_t r0, uint32_t r1, uint32_t r2);

SysCallHandler syscalls[MAX_SYS_CALLS];

void interrupt_init(){
    
    for(int counter=0; counter < MAX_SYS_CALLS; ++counter){
        syscalls[counter].sysStatus = SYS_FREE;
    }
    
}

void registerSysHandler(SysCallHandler *handle){
    
    if(syscalls[handle->preferredLine].sysStatus!=SYS_USED){
        prim_mem_cpy(handle,&syscalls[handle->preferredLine], sizeof(SysCallHandler));
        syscalls[handle->preferredLine].sysStatus=SYS_USED;          
    }
    
}

uint32_t swiHandler(uint32_t r0, uint32_t r1, uint32_t r2){
    uint8_t int_vector = 0;
    asm volatile ("ldrb %0, [lr, #-2]" : "=r" (int_vector));
    
    if(syscalls[int_vector].sysStatus==SYS_USED){
        return syscalls[int_vector].handler(r0,r1,r2);
    }
    
    return SYS_ERR;
    
}

uint32_t IRQHandler(uint32_t r0, uint32_t r1, uint32_t r2){
    return irqhandler(r0,r1,r2);
}

uint32_t UndefineHandler(uint32_t r0, uint32_t r1, uint32_t r2){
    return undefinehandler(r0,r1,r2);
}

uint32_t FIQHandler(uint32_t r0, uint32_t r1, uint32_t r2){
    return fiqhandler(r0,r1,r2);
}
uint32_t DataAbortHandler(uint32_t r0, uint32_t r1, uint32_t r2){
    return dataAbortHandler(r0,r1,r2);
}
uint32_t PrefetchAbortHandler(uint32_t r0, uint32_t r1, uint32_t r2){
    return prefetchAbortHandler(r0,r1,r2);
}


void registerIRQHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2)){
    irqhandler = handler;
}
    
void registerUndefinedHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2)){
    undefinehandler = handler;
}
    
void registerFIQHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2)){
    fiqhandler = handler;
}
    
void registerDataAbortHandler(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2)){
    dataAbortHandler = handler;
}
    
void registerPrefetchAbort(uint32_t (*handler)(uint32_t r0, uint32_t r1, uint32_t r2)){
    prefetchAbortHandler = handler;
}

