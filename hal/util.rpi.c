/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <stdint.h>

#include <hal.h>


void prim_memset_zero(void *target, uint32_t size){
    
    uint8_t *byte_ptr = target;
    for(int counter=0; counter < size; ++counter){
        byte_ptr[counter]=0x0;
    }
    
}


//primitive mem copy - doesn't know if its real of virtual
void prim_mem_cpy(const void *origin , void *target, uint32_t target_size){
       //__builtin_memcpy(target,origin,target_size);
    int counter = 0;
    const uint8_t * origin_byte = origin;
    uint8_t * destination_byte = target;
    
    for(counter=0; counter < target_size; ++counter){
        *(destination_byte + counter) = *(origin_byte + counter);
    }
    
}

uint32_t prim_str_size(const char *string, uint32_t max_size){
    
    for(int counter = 0; counter < max_size; counter++){
        if(string[counter]=='\n')
            return counter+1;
    }
    
    return max_size;
}

//primitive string compare
uint32_t prim_str_cmp(const char *string_one, const char *string_two) {
    //Check if the address is the same
    if(string_one == string_two)
        return 0;
    
    uint32_t counter =0;
    while(string_one[counter]==string_two[counter]){
        ++counter;
        if(string_one[counter]=='\n')
            return 0;
    }
    return counter+0x4000;
    
}

void spin(int32_t spin_count) {
    asm volatile("__delay_%=: subs %[spin_count], %[spin_count], #1; bne __delay_%=\n"
	     : : [spin_count]"r"(spin_count) : "cc");
}
