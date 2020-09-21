/**
 * @file     ctxsw.s
 * @provides ctxsw
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <arm.h>

.text
	.align 4
	.globl	ctxsw


/**
 * @fn void ctxsw(&oldstack, &newstack)
 *
 * Switch context (values in registers) to another process, saving the
 * current processes information. This function will not return as normally
 * thought as it will load in the stack pointer for a different process and
 * jump to that location and begin executing code.
 *
 * @param  &oldstack address of outgoing stack save area
 * @param  &newstack address of incoming stack save area
 * @return special case -- see above
 */
ctxsw:
				//what happens to the stuff in r0, r1 from the current process
	
	mov	r4, r0;		//moving old stack adress to non-volatile register
	mov	r5, r1;		//moving new stack adress to non-volatile register	
	
	str	r0, [r4, #0]    //storing r0 at first adress in stack pointer 
	str	r1, [r4, #4]	//storing r1 at 4 bytes above first adress in stack pointer
	str	r2, [r4, #8]	// ...
	str	r3, [r4, #12]	// ...
	str	r14, [r4, #56]  //storing r14 at 56 bytes above first adress in stack pointer
	str 	r15, [r4, #60]  //storing r15 at 60 bytes above first adress in stack pointer
	str			//how do we store our pads?

				//load new stack into registers
			
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	ldr	r2, [r5, #8]	// and so forth

				//do we jump to the link register?


// TODO: Save callee-save ("non-volatile") registers.
//		str --> storing to memory (arguments 1.Register Value 2.Where you want to store in the memory)
//		ldr --> takes what is in the address and moves it to the register (arguments 1.Address 2.Register)
//		
// TODO: Restore callee-save ("non-volatile") registers.
// TODO: Jump to next function.
	
	.end ctxsw
