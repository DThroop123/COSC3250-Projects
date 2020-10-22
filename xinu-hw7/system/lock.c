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
    while (FALSE == _atomic_compareAndSwapStrong(lock,
                                                 LOCK_UNLOCKED,
                                                 LOCK_LOCKED))
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
    *lock = LOCK_UNLOCKED;
    return OK;
}
