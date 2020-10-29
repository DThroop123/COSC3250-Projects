/**
 * @file atomic.c
 * @provides atomic_decrement, atomic_increment
 *
 * Atomic operations for safe multicore coordination
 *
 * Embedded Xinu, Copyright (C) 2020. All rights reserved.
 */

#include <xinu.h>

/**
 * Use atomic machine operations to safely decrement a global
 * variable that might be shared between cores.
 * @param  pvar Pointer to a shared global variable
 * @return New value of shared variable pointed to by pvar
 */
int atomic_decrement(volatile int *pvar)
{
    int val, newVal = 0;
    while (TRUE)
    {
        val = *pvar;
        newVal = val - 1;

        if (TRUE == _atomic_compareAndSwapStrong(pvar, val, newVal))
            return newVal;
    }
}

/**
 * Use atomic machine operations to safely increment a global
 * variable that might be shared between cores.
 * @param  pvar Pointer to a shared global variable
 * @return New value of shared variable pointed to by pvar
 */
int atomic_increment(volatile int *pvar)
{
    int val, newVal = 0;
    while (TRUE)
    {
        val = *pvar;
        newVal = val + 1;

        if (TRUE == _atomic_compareAndSwapStrong(pvar, val, newVal))
            return newVal;
    }
}
