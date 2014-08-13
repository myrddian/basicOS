
#include "../hal/hal.h"
#include "../hw/rpi/uart.h"
#include "../hw/rpi/mailbox.h"
#include "../hw/rpi/framebuffer.h"

/*
 * This is the kernel entry point
 * sets up the basic hardware for the
 * system to start
 *
 */

void entry_point(){
	hal_init();
	create_uart_device();
	registerMailBoxDriver();
	registerFramebuffer();
	kernel_main();
}
