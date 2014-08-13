/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#ifndef _HAL_
#define _HAL_

#include <gpio.h>
#include <mmio.h>
#include <interrupts.h>



#define HAL_PARENT 0

#define DEV_FREE -1
#define DEV_ALLOC 0
#define DEV_USE 1
#define NULL 0x0

#define MAX_DEV 32
#define MAX_DEV_NAME 64

#define DEVICE_SUCESS 0
#define DEVICE_ERR_UNK 1
#define DEVICE_USE_ERR 2

#define HAL_X86_PIC 0
#define HAL_ARM_RPI 1
#define HAL_X86_APIC_UP 2
#define HAL_X86_APIC_MP 3



typedef struct __reg_device {
     
       int32_t  status; 
       uint32_t parent_id;
       const char     *name;
       uint32_t (*init)(uint32_t parent_id);
       uint32_t (*open)(struct __reg_device *this);
       uint32_t (*close)(struct __reg_device *this);
       uint32_t (*write)(struct __reg_device *this, uint32_t address, uint32_t size);
       uint32_t (*read)(struct __reg_device *this, uint32_t address, uint32_t size);
}DeviceHandler;


typedef struct __hal_service {

	uint32_t (*get_hal_type)();
	uint32_t (*register_interrupt)(uint32_t interrupt, void (*handler)());


}HalService;

void spin(int32_t spin_count);
void hal_init();
uint32_t registerDevice(DeviceHandler *device, uint32_t parent_dev);
int32_t getDeviceID(const char name[MAX_DEV_NAME]);
DeviceHandler * getDevice(const char name[MAX_DEV_NAME]);
DeviceHandler * getDevice_n(uint32_t alias);

uint32_t DeviceOpen(char name[MAX_DEV_NAME]);
uint32_t DeviceClose(char name[MAX_DEV_NAME]);
uint32_t DeviceWrite(char name[MAX_DEV_NAME], uint32_t address, uint32_t size);
uint32_t DeviceRead(char name[MAX_DEV_NAME], uint32_t address, uint32_t size);
uint32_t DeviceIOCTL(char name[MAX_DEV_NAME], uint8_t * data, uint32_t size);

void kernel_main();





#endif
