/* 
 * File:   interrupts.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 10:32 AM
 */


#include <stdint.h>

#include <hal.h>


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
            return counter;
    }
    
    return max_size;
}

//primitive string compare
uint32_t prim_str_cmp(const char *string_one, const char *string_two, uint32_t size) {
    //Check if the address is the same
    if(string_one == string_two)
        return 0;
    
    uint32_t tally_one, tally_two;
    
    uint32_t str_one_size = prim_str_size(string_one,size);
    uint32_t str_two_size = prim_str_size(string_two,size);
    
    if(str_one_size!=str_two_size)
        return str_one_size;
    
    tally_one = 0;
    for(uint32_t counter=0; counter< str_one_size; ++counter)
        tally_one += string_one[counter];
    
    tally_two =0;
    
    for(uint32_t counter=0; counter<str_two_size; ++counter)
        tally_two += string_two[counter];
    
    return tally_one - tally_two;
}

void spin(int32_t spin_count) {
    asm volatile("__delay_%=: subs %[spin_count], %[spin_count], #1; bne __delay_%=\n"
	     : : [spin_count]"r"(spin_count) : "cc");
}
