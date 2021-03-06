/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <stdint.h>


#ifndef MMIO_RPI_H
#define MMIO_RPI_H
 
// write to MMIO register
static inline void mmio_write(uint32_t reg, uint32_t data) {
    uint32_t *ptr = (uint32_t*)reg;
    asm volatile("str %[data], [%[reg]]"
	     : : [reg]"r"(ptr), [data]"r"(data));
}
 
// read from MMIO register
static inline uint32_t mmio_read(uint32_t reg) {
    uint32_t *ptr = (uint32_t*)reg;
    uint32_t data;
    asm volatile("ldr %[data], [%[reg]]"
		 : [data]"=r"(data) : [reg]"r"(ptr));
    return data;
}

#endif
