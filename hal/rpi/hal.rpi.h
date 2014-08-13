/*
 * hal.rpi.h
 *
 *  Created on: Jun 25, 2014
 *      Author: myrddian
 */

#ifndef HAL_RPI_H_
#define HAL_RPI_H_

#include <stdint.h>


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


#endif /* HAL_RPI_H_ */
