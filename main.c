/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <hal.h>
#include <uart.h>
#include "util.h"
#include <mailbox.h>
#include <framebuffer.h>

void kernel_main(uint32_t r0, uint32_t r1, uint32_t atags) {
    
    char hello_world[]= "\r\nHello World!\r\n";
    char uart_dev[]="UART_DEV\n";
    
    UNUSED(r0);
    UNUSED(r1);
    UNUSED(atags);
    
    hal_init();
    create_uart_device();
    registerMailBoxDriver();
    registerFramebuffer();
    
    //DeviceHandler * console = getDevice("UART_DEV");
    DeviceOpen(uart_dev);
    DeviceWrite(uart_dev,(uint32_t)(&hello_world),sizeof(hello_world));
    while(1){
        
    }
    
}