
#include <interrupts.h>
#include <hal.h>
#include <util.h>
#include "idt.h"



SysCallHandler syscalls[MAX_SYS_CALLS];

void interrupt_init(){
    
    for(int counter=0; counter < MAX_SYS_CALLS; ++counter){
        syscalls[counter].sysStatus = SYS_FREE;
    }
    
}
