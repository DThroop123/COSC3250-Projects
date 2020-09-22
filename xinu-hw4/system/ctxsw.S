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
				//what do we want to put for r0 - r3 for the current running process (?)
				//should we increment by 4 (int) or 8 bytes (ulong) (?)




	str     r4, [r0, #CTX_R4 * 4]	//storing non-volatile memory
	str	r5, [r0, #CTX_R5 * 4]
	str	r6, [r0, #CTX_R6 * 4]
	str	r7, [r0, #CTX_R7 * 4]
	str 	r8, [r0, #CTX_R8 * 4]
	str	r9, [r0, #CTX_R9 * 4]
	str	r10, [r0, #CTX_R10 * 4]
	str	r11, [r0, #CTX_R11 * 4]
	str 	r12, [r0, #CTX_IP * 4]	
	str	r14, [r0, #CTX_LR * 4]  //storing r14 at 56 bytes above first adress in stack pointer/	
	str 	r15, [r0, #CTX_PC * 4]  //storing r15 at 60 bytes above first adress in stack pointer
	str	r13, [r0, #CTX_SP * 4]	//storing oldstack adress in r13


	ldr	r4, [r1, #CTX_R4 * 4]	//loading non-volatile memory
	ldr	r5, [r1, #CTX_R5 * 4]
	ldr	r6, [r1, #CTX_R6 * 4]
	ldr	r7, [r1, #CTX_R7 * 4]
	ldr	r8, [r1, #CTX_R8 * 4]
	ldr 	r9, [r1, #CTX_R9 * 4]
	ldr	r10, [r1, #CTX_R10 * 4]
	ldr	r11, [r1, #CTX_R11 * 4]
	ldr	r12, [r1, #CTX_IP * 4]
	ldr	r13, [r1, #CTX_SP * 4]	//
	ldr	r14, [r1, #CTX_LR * 4]	//loading link register into r14 
	ldr	r15, [r1, #CTX_PC * 4]	//loading program counter into r15 (jump)
		

				

// TODO: Save callee-save ("non-volatile") registers.
//		str --> storing to memory (arguments 1.Register Value 2.Where you want to store in the memory)
//		ldr --> takes what is in the address and moves it to the register (arguments 1.Address 2.Register)
//		
// TODO: Restore callee-save ("non-volatile") registers.
// TODO: Jump to next function.
	
	.end ctxsw
