/* boot.S - assembly startup code */
 
// To keep this in the first portion of the binary.
.section ".text.boot"
 
// Make Start global.
.globl Start

// Entry point for the kernel.
// r15 -> should begin execution at 0x8000.
// r0 -> 0x00000000
// r1 -> 0x00000C42
// r2 -> 0x00000100 - start of ATAGS
// preserve these registers as argument for kernel_main
Start:
        ldr pc,reset_handler
        ldr pc,undefined_handler
        ldr pc,swi_handler
        ldr pc,prefetch_handler
        ldr pc,data_handler
        ldr pc,unused_handler
        ldr pc,irq_handler
        ldr pc,fiq_handler 

        reset_handler:      .word reset
        undefined_handler:  .word uhandler
        swi_handler:        .word swi
        prefetch_handler:   .word hang
        data_handler:       .word hang
        unused_handler:     .word hang
        irq_handler:        .word uhandler
        fiq_handler:        .word hang

reset:
    mov r0,#0x8000
    mov r1,#0x0000
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}
    ldmia r0!,{r2,r3,r4,r5,r6,r7,r8,r9}
    stmia r1!,{r2,r3,r4,r5,r6,r7,r8,r9}

    ;@ (PSR_IRQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD2
    msr cpsr_c,r0
    mov sp,#0x8000

    ;@ (PSR_FIQ_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD1
    msr cpsr_c,r0
    mov sp,#0x4000

    ;@ (PSR_SVC_MODE|PSR_FIQ_DIS|PSR_IRQ_DIS)
    mov r0,#0xD3
    msr cpsr_c,r0
    mov sp,#0x8000000

    ;@ SVC MODE, IRQ ENABLED, FIQ DIS
    ;@mov r0,#0x53
    ;@msr cpsr_c, r0

    bl kernel_start
hang: b hang

        
kernel_start:
	// Setup the stack.
	mov	sp, #0x8000
 
	// Clear out bss.
	ldr	r4, =_bss_start
	ldr	r9, =_bss_end
	mov	r5, #0
	mov	r6, #0
	mov	r7, #0
	mov	r8, #0
        b       2f
 
1:
	// store multiple at r4.
	stmia	r4!, {r5-r8}
 
	// If we are still below bss_end, loop.
2:
	cmp	r4, r9
	blo	1b
 
	// Call kernel_main
        //Set up vectors
       

	ldr	r3, =kernel_main
	blx	r3
 
	// halt
halt:
	wfe
	b	halt



.global enable_irq
enable_irq:
    mrs r0,cpsr
    bic r0,r0,#0x80
    msr cpsr_c,r0

uhandler:
    push {r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl irqhandler
    pop  {r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    subs pc,lr,#4

swi:
    stmfd   sp!, {r12}
    bl swiHandler
    LDMFD   sp!, {r12}
    MOVS    pc, r14
    

