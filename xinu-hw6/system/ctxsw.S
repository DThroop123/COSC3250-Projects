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


	add	sp, sp, #-64	//pushes 16 more register spaces onto the current stack
	

	str     r4, [sp, #CTX_R4 * 4]	//storing non-volatile memory
	str	r5, [sp, #CTX_R5 * 4]
	str	r6, [sp, #CTX_R6 * 4]
	str	r7, [sp, #CTX_R7 * 4]
	str 	r8, [sp, #CTX_R8 * 4]
	str	r9, [sp, #CTX_R9 * 4]
	str	r10, [sp, #CTX_R10 * 4]
	str	r11, [sp, #CTX_R11 * 4]
        mrs     r12, cpsr
        str 	r12, [sp, #CTX_IP * 4] 	
	str	r14, [sp, #CTX_LR * 4]  //storing r14 at 56 bytes above first adress in stack pointer/	
	str 	r14, [sp, #CTX_PC * 4]  //storing r15 at 60 bytes above first adress in stack pointer
	str	r13, [sp, #CTX_SP * 4]	//storing oldstack adress in r13
	str	sp, [r0] //stack pointer stores in r0
	
	ldr	sp, [r1] //stack pointer into r1 can do math to locate the sp by add sp, sp, #64 
	ldr	r0, [sp, #CTX_R0 *4]
	ldr	r1, [sp, #CTX_R1 *4]
	ldr	r2, [sp, #CTX_R2 *4]
	ldr	r3, [sp, #CTX_R3 *4]
	ldr	r4, [sp, #CTX_R4 * 4] //loading non-volatile memory
	ldr	r5, [sp, #CTX_R5 * 4]
	ldr	r6, [sp, #CTX_R6 * 4]
	ldr	r7, [sp, #CTX_R7 * 4]
	ldr	r8, [sp, #CTX_R8 * 4]
	ldr 	r9, [sp, #CTX_R9 * 4]
	ldr	r10, [sp, #CTX_R10 * 4]
	ldr	r11, [sp, #CTX_R11 * 4]
	msr     cpsr_c, r12
	ldr	r12, [sp, #CTX_IP * 4]
	ldr	r14, [sp, #CTX_LR * 4]	//loading link register into r14 
	ldr	r12, [sp, #CTX_PC * 4]	//loading program counter into r12 (safe to store here)
//	ldr	sp,  [sp, #CTX_SP * 4]
	add	sp, sp, #64	//pushes 16 more register spaces off of the stack

	bx	r12	//moved the pc content to r12 for easier jump process
				

// TODO: Save callee-save ("non-volatile") registers.
//		str --> storing to memory (arguments 1.Register Value 2.Where you want to store in the memory)
//		ldr --> takes what is in the address and moves it to the register (arguments 1.Address 2.Register)
//		
// TODO: Restore callee-save ("non-volatile") registers.
// TODO: Jump to next function.
	
	.end ctxsw
