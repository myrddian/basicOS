/* 
 * File:   framebuffer.h
 * Author: myrddian
 *
 * Created on 6 June 2014, 3:33 PM
 */
#include <stdint.h>
#ifndef FRAMEBUFFER_H
#define	FRAMEBUFFER_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct __FrameBuferDescription {
  uint32_t width; uint32_t height; uint32_t vWidth; uint32_t vHeight; uint32_t pitch; uint32_t bitDepth;
  uint32_t x; uint32_t y; void * pointer; uint32_t size;
}FrameBuferDescription;

void registerFramebuffer();
extern const char framebuffer_dev[]; 

#ifdef	__cplusplus
}
#endif

#endif	/* FRAMEBUFFER_H */

