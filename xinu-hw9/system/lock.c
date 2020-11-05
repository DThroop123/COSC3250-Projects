/**
 * @file lock.c
 * @provides lock_acquire, lock_release.
 *
 */
/* Embedded Xinu, Copyright (C) 2008, 2020. All rights reserved. */

#include <xinu.h>

/**
 * Acquire a lock.
 * @param lock	target lock
 * @return OK on success, SYSERR on failure
 */
syscall lock_acquire(volatile int *lock)
{
    irqmask im;
    im = disable();
    while (FALSE == _atomic_compareAndSwapStrong(lock,
                                                 LOCK_UNLOCKED,
                                                 LOCK_LOCKED | (im & ARM_I_BIT)))
    {                           // do nothing while cas fails
    }

    return OK;
}

/**
 * release a lock
 * @param lock	target lock
 * @return OK on success, SYSERR on failure
 */
syscall lock_release(volatile int *lock)
{
    irqmask im;
    im = *lock & ARM_I_BIT;

    *lock = LOCK_UNLOCKED;
    
    if(!im)
        enable();

    return OK;
}
