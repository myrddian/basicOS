/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <hal.h>
#include <uart.h>
#include "../util.h"


void uart_putc(uint8_t byte) {
    // wait for UART to become ready to transmit
    while (1) {
        if (!(mmio_read(UART0_FR) & (1 << 5))) {
	    break;
	}
    }
    mmio_write(UART0_DR, byte);
}



uint32_t uart_close(DeviceHandler *this){
    
    if(this->status!=DEV_USE)
        return DEVICE_USE_ERR;
    
    this->status = DEV_ALLOC;
    return DEVICE_SUCESS;
    
}

uint32_t uart_open(DeviceHandler *this){
    
    if(this->status==DEV_USE)
        return DEVICE_USE_ERR;
    
    this->status = DEV_USE;
    return DEVICE_SUCESS;
    
}

uint8_t uart_getc() {
    // wait for UART to have recieved something
    while(1) {
	if (!(mmio_read(UART0_FR) & (1 << 4))) {
	    break;
	}
    }
    return mmio_read(UART0_DR);
}

uint32_t uart_read(DeviceHandler *this, uint32_t address, uint32_t size){
    if(this->status!=DEV_USE)
        return DEVICE_USE_ERR;
    
    uint8_t * byte = (uint8_t *)address;
    for(int counter=0; counter < size; ++counter){
        *(byte+counter) = uart_getc(); 
    }
    return DEVICE_SUCESS;
}

uint32_t uart_write(DeviceHandler *this, uint32_t address, uint32_t size) {
    
    if(this->status!=DEV_USE)
        return DEVICE_USE_ERR;
    
    uint8_t * byte = (uint8_t *)address;
    for(int counter=0; counter < size; ++counter){
        uart_putc(*byte+counter);
    }
    return DEVICE_SUCESS;
}


uint32_t uart_init(uint32_t parent_id){
    //We ignore the parent
    UNUSED(parent_id);
     // Disable UART0.
    mmio_write(UART0_CR, 0x00000000);
    // Setup the GPIO pin 14 && 15.
 
    // Disable pull up/down for all GPIO pins & delay for 150 cycles.
    mmio_write(GPPUD, 0x00000000);
    spin(150);
 
    // Disable pull up/down for pin 14,15 & delay for 150 cycles.
    mmio_write(GPPUDCLK0, (1 << 14) | (1 << 15));
    spin(150);
 
    // Write 0 to GPPUDCLK0 to make it take effect.
    mmio_write(GPPUDCLK0, 0x00000000);
 
    // Clear pending interrupts.
    mmio_write(UART0_ICR, 0x7FF);
 
    // Set integer & fractional part of baud rate.
    // Divider = UART_CLOCK/(16 * Baud)
    // Fraction part register = (Fractional part * 64) + 0.5
    // UART_CLOCK = 3000000; Baud = 115200.
 
    // Divider = 3000000/(16 * 115200) = 1.627 = ~1.
    // Fractional part register = (.627 * 64) + 0.5 = 40.6 = ~40.
    mmio_write(UART0_IBRD, 1);
    mmio_write(UART0_FBRD, 40);
 
    // Enable FIFO & 8 bit data transmissio (1 stop bit, no parity).
    mmio_write(UART0_LCRH, (1 << 4) | (1 << 5) | (1 << 6));
 
    // Mask all interrupts.
    mmio_write(UART0_IMSC, (1 << 1) | (1 << 4) | (1 << 5) |
		    (1 << 6) | (1 << 7) | (1 << 8) |
		    (1 << 9) | (1 << 10));
 
    // Enable UART0, receive & transfer part of UART.
    mmio_write(UART0_CR, (1 << 0) | (1 << 8) | (1 << 9));
    
    return DEVICE_SUCESS;
       
}

void create_uart_device(){
    
    DeviceHandler uart_device;
    prim_mem_cpy("UART_DEV\n",uart_device.name, sizeof(char)*8);
    uart_device.parent_id = HAL_PARENT;
    uart_device.open = &uart_open;
    uart_device.close = &uart_close;
    uart_device.init = &uart_init;
    uart_device.read = &uart_read;
    uart_device.write = &uart_write;
    
    registerDevice(&uart_device,HAL_PARENT);
  
}

