#include "../hal.h"

/*
 * This is the kernel entry point
 * sets up the basic hardware for the
 * system to start
 *
 */

void entry_point(void){

	//set up gdt
	//set up idt
	//set up software interrupts
	//set up hardware interrupts

	//call the next stage in HAL initialisation

	hal_init();

	//Call the kernel
	kernel_main();


}
