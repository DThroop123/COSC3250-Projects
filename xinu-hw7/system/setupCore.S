/**
 * @file setupCore.S
 * @provides setupCore
 */
/* Embedded Xinu. Copyright (C) 2018. All rights reserved. */

#include <arm.h>
#include <bcm2837.h>
#include <mmu.h>

/**
 * @fn void setupCore(void)
 *
 * Called from unparkcore. This code runs on a core and
 * properly prepares it so it can execute kernel code.
 *
 */
.globl setupCore
setupCore:
	/* change processor to SYSTEM mode */
	mrs		r0, cpsr
	orr		r0, r0, #ARM_MODE_SYS
	msr		spsr_cxsf, r0
	add		r0, pc, #4
	msr		ELR_hyp, r0
	eret

	/* disable interrupts */
	cpsid	aif

	/* set vector addresses */
	ldr		r1, =_vectors
	mcr		p15, 0, r1, c12, c0, 0 /* Write to VBAR */
	
	/* get CPUID from MPIDR register and put value in r0 */
	mrc		p15, 0, r0, c0, c0, 5		/* MPIDR */
	and		r0, r0, #7					/* last 3 bits is CPUID */

	/* multiply the CPUID by 4 to get the correct index for the arrays */
	mov		r1, r0
	mov		r2, #4
	mul		r1, r1, r2

	/* initialize Physical Timer */
	mrc		p15, 0, r2, c14, c2, 1		/* CNTP_CTL */
	bic		r2, #0b110	// ISTATUS and IMASK to 0
	orr		r2, #0b001	// enable to 1
	mcr		p15, 0, r2, c14, c2, 1

	ldr		r2, =CORE0_TIMER_IRQCNTL	/* Core x timer interrupt control */
	add		r2, r1				/* offset for proper core */
	ldr		r3, [r2]
	bic		r3, #0b11111100
	orr		r3, #0b11			/* nCNTPNSIRQ and nCNTPSIRQ irq enable */
	str		r3, [r2]

	/* core_init_sp array is for the initial stack pointer for each core */	
	ldr		r2, =core_init_sp
	ldr		sp, [r2, r1]

	/* move value to non-volatile registers before calling start_mmu */
	mov		r4, r1

	/* call start_mmu so the programmer does not have to manual call it each time they unpark a core 			*/
	mov		r0, #MMUTABLEBASE		/* MMUTABLEBASE from mmu.h */
	bl		start_mmu

	/* corestart array of addresses for the instruction for each core to execute once setup is done */
	ldr		r0, =init_args
	ldr		r0, [r0, r4]
	ldr		r2, =corestart
	ldr		pc, [r2, r4]	


/* Send EVent routine so we can use it in C code.. (unparkcore.c) */
.globl sev
sev:
	.func sev
	sev
	bx	lr
	.endfunc

/* getcpuid() */
.globl getcpuid
getcpuid:
	.func getcpuid
	mrc	p15, 0, r0, c0, c0, 5	// MPIDR REGISTER
	and	r0, r0, #7		// and with 7 to get last 3 bits
	bx	lr
	.endfunc

/* getmode() */
.globl getmode
getmode:
	.func getmode
	mrs	r0, cpsr
	bx	lr
	.endfunc
