/* 
 * File:   util.c
 * Author: myrddian
 *
 * Created on 5 June 2014, 2:37 PM
 */

#ifndef UTIL_C
#define	UTIL_C

#ifdef	__cplusplus
extern "C" {
#endif

#define UNUSED(x) (void)(x)
    
void prim_mem_cpy(const void *origin , void *target, uint32_t target_size);
uint32_t prim_str_cmp(const char *string_one, const char *string_two);
uint32_t prim_str_size(const char *string, uint32_t max_size);
void prim_memset_zero(void *target, uint32_t size);

#ifdef	__cplusplus
}
#endif

#endif	/* UTIL_C */

