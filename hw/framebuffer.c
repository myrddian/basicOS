#include <hal.h>
#include <stdint.h>
#include "mailbox.h"
#include "framebuffer.h"
#include "../util.h"

const char framebuffer_dev[]="FRAMEBUFFER_DEV"; 

uint32_t fb_init(uint32_t parent_id){
    UNUSED(parent_id);
    //We do nothing and return success
    return DEVICE_SUCESS;
    
}

uint32_t fb_open(struct __reg_device *this){
    if(this->status==DEV_USE)
       return DEVICE_USE_ERR;
    
    this->status = DEV_USE;
    return DEVICE_SUCESS;
}

uint32_t fb_close(struct __reg_device *this){
    if(this->status!=DEV_USE)
        return DEVICE_USE_ERR;
    
    this->status = DEV_ALLOC;
    return DEVICE_SUCESS;
}


//
uint32_t fb_write(struct __reg_device *this, uint32_t address, uint32_t size){
    UNUSED(this);
    UNUSED(address);
    UNUSED(size);
    return DEVICE_SUCESS;
}


//Fetch the frame buffer and return its address
uint32_t fb_read(struct __reg_device *this, uint32_t address, uint32_t size){
    //the address contains the FB descriptor
    //the function returns FB address
    UNUSED(size);
    //FrameBuferDescription * descriptor = (FrameBuferDescription*)address;
    uint32_t mbox_offset = 0x40000000;
    
    //Get the parent device
    DeviceHandler * mailbox = getDevice_n(this->parent_id);
    MailBoxBuffer mailbox_write;
    uint32_t mailbox_write_address = (uint32_t)&mailbox_write;
    mailbox_write.channel = 1;
    mailbox_write.value = address + mbox_offset;
    
    //Write to the driver
    mailbox->write(mailbox,mailbox_write_address,sizeof(MailBoxBuffer));
    spin(1); //lets spin
    MailBoxBuffer mailbox_read;
    uint32_t mailbox_read_address = (uint32_t)&mailbox_read;
    mailbox_read.channel = 1;
    mailbox_read.value = NULL;
    mailbox->read(mailbox,mailbox_read_address,sizeof(MailBoxBuffer));
    
    if(mailbox_read.value==NULL)
        return NULL;
    
    return mailbox_read.value;
    
}



void registerFramebuffer(){
    
    //Get the mailbox driver interface
    //DeviceHandler *mailbox = getDevice(mbox_name);
    DeviceHandler framebuffer;
    prim_memset_zero(&framebuffer, sizeof(DeviceHandler));
    //prim_mem_cpy(framebuffer_dev,framebuffer.name, prim_str_size(framebuffer_dev,MAX_DEV_NAME));
    framebuffer.name = framebuffer_dev;

    framebuffer.open = &fb_open;
    framebuffer.close = &fb_close;
    framebuffer.init = &fb_init;
    framebuffer.read = &fb_read;
    framebuffer.write = &fb_write;
    
    registerDevice(&framebuffer,getDeviceID(mbox_name));
    
    //framebuffer.parent_id = mailbox->
    
    
}
