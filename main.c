#include <hal.h>
#include <uart.h>
#include "util.h"

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    
    char hello_world[]= "\r\nHello World!\r\n";
    char uart_dev[]="UART_DEV\n";
    
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    hal_init();
    create_uart_device();
    
    //DeviceHandler * console = getDevice("UART_DEV");
    DeviceOpen(uart_dev);
    DeviceWrite(uart_dev,*hello_world,sizeof(hello_world));
    while(1){
        
    }
    
}