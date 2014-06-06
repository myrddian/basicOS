/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <stdint.h>
#include <hal.h>

DeviceHandler devices[MAX_DEV]; ///just a device list.


int32_t getDeviceID(const char name[MAX_DEV_NAME]){
    
    for(int counter=0; counter < MAX_DEV; ++counter){
        if(prim_str_cmp(name,devices[counter].name,prim_str_size(name,MAX_DEV_NAME))==0){
            return counter;
        }
    }
    return DEV_FREE;
    
}

void hal_init() {
    int counter;
    for(counter=0; counter < MAX_DEV; ++counter){
        devices[counter].status = DEV_FREE;
    }
    interrupt_init();
    
}

uint32_t registerDevice(DeviceHandler *device, uint32_t parent_dev){
     //Find a free spot
     int location = 0;
     while(devices[location].status != DEV_FREE ){
           ++location;
           if(location==MAX_DEV){
              //We should panic here but return
              return 0;
           }
     }
     //Copy the handler
     prim_mem_cpy(device,&devices[location],sizeof(DeviceHandler));
     devices[location].init(parent_dev);
     devices[location].status = DEV_ALLOC;
     return location;
}


DeviceHandler * getDevice(const char name[MAX_DEV_NAME]){
    
    for(int counter=0; counter < MAX_DEV; ++counter){
        if(prim_str_cmp(name,devices[counter].name,prim_str_size(name,MAX_DEV_NAME))==0){
            return &devices[counter];
        }
    }
    
    return NULL;
    
}

DeviceHandler * getDevice_n(uint32_t alias) {
    
    if(alias > MAX_DEV)
        return NULL;
    
    if(devices[alias].status == DEV_FREE)
        return NULL;
    
    return &devices[alias];
    
}

uint32_t DeviceOpen(char name[MAX_DEV_NAME]){
    DeviceHandler *device;
    device = getDevice(name);
    return device->open(device);   
    
}
uint32_t DeviceClose(char name[MAX_DEV_NAME]){
    DeviceHandler *device;
    device = getDevice(name);
    return device->close(device);
}

uint32_t DeviceWrite(char name[MAX_DEV_NAME], uint32_t address, uint32_t size){
    DeviceHandler *device;
    device = getDevice(name);
    return device->write(device,address,  size);
}
uint32_t DeviceRead(char name[MAX_DEV_NAME], uint32_t address, uint32_t size){
    DeviceHandler *device;
    device = getDevice(name);
    return device->read(device, address,  size);
}
