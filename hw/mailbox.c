#include <hal.h>
#include <stdint.h>
#include "mailbox.h"
#include "../util.h"

extern uint32_t MailboxRead(uint8_t channel);
extern void MailboxWrite(uint32_t value, uint8_t channel);

const char mbox_name[] = "MAILBOX_DEV\n";  

uint32_t init(uint32_t parent_id){
    UNUSED(parent_id);
    //We do nothing and return success
    return DEVICE_SUCESS;
    
}

uint32_t open(struct __reg_device *this){
    UNUSED(this);
    return DEVICE_SUCESS;
}

uint32_t close(struct __reg_device *this){
    UNUSED(this);
    return DEVICE_SUCESS;
}


//
uint32_t write(struct __reg_device *this, uint32_t address, uint32_t size){
    UNUSED(this);
    MailBoxBuffer * cmd = (MailBoxBuffer *) address;
    if(sizeof(MailBoxBuffer)==size){
       MailboxWrite(cmd->value,cmd->channel); 
       return DEVICE_SUCESS;
    }
    return DEVICE_ERR_UNK;
}

uint32_t read(struct __reg_device *this, uint32_t address, uint32_t size){
    UNUSED(this);
    MailBoxBuffer * buffer = (MailBoxBuffer *)address;
    if(sizeof(MailBoxBuffer)==size){
        buffer->value = MailboxRead(buffer->channel);
        return DEVICE_SUCESS;
    }
    return DEVICE_ERR_UNK;
}



void registerMailBoxDriver() {
    
    DeviceHandler mbox_device;
    prim_mem_cpy(mbox_name,mbox_device.name, prim_str_size(mbox_name,MAX_DEV_NAME));
    mbox_device.parent_id = HAL_PARENT;
    mbox_device.open = &open;
    mbox_device.close = &close;
    mbox_device.init = &init;
    mbox_device.read = &read;
    mbox_device.write = &write;
    
    registerDevice(&mbox_device,HAL_PARENT);
    
}