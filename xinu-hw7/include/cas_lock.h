/**
 * @file cas_lock.h
 *
 */
/* Embedded Xinu, Copyright (C) 2008, 2020. All rights reserved. */

#ifndef _CAS_LOCK_H_
#define _CAS_LOCK_H_

/* Spinlock state definitions */
#define LOCK_UNLOCKED		0x00
#define LOCK_LOCKED		    0x01

#ifndef __ASSEMBLER__

/* CAS Spinlock function prototypes */
syscall lock_acquire(volatile int *);
syscall lock_release(volatile int *);

#endif                          /* __ASSEMBLER__ */

#endif                          /* _CAS_LOCK_H_ */
