/**
 * @file atomic.h
 *
 * This file includes the function prototypes for atomic ARM assembly routines.
 *
 */
/* Embedded Xinu, Copyright (C) 2019. All rights reserved. */

#ifndef _ATOMIC_H_
#define _ATOMIC_H_

/**
 * @fn int atomic_increment(volatile int *var)
 *  
 * Atomically increments specified integer.
 * (Equivalent to ++var in C).
 * 
 * @param *var pointer to variable
 * @return value the variable held after
 *         being incremented.
 */
int atomic_increment(volatile int *var);

/**
 * @fn int atomic_decrement(volatile int *var)
 * 
 * Atomically decrements specified integer.
 * (Equivalent to --var in C).
 * 
 * @param *var pointer to variable
 * @return value the variable held after
 *         being decremented.
 *         
 */
int atomic_decrement(volatile int *var);

/**
 * @fn bool_atomic_compareAndSwap(volatile int *var, int old, int new)
 *
 * Atomically compare and swap old value for new value.
 *
 * @param *var  pointer to variable
 * @param old   expected old value in var
 * @param new   new value for var if old value matches
 * @return true of old value atomically replaced with new, else false
 */
bool _atomic_compareAndSwapStrong(volatile int *var, int old, int new);

/**
 * @fn bool_atomic_compareAndSwapWeak(volatile int *var, int old, int new)
 *
 * Atomically compare and swap old value for new value.
 *
 * @param *var  pointer to variable
 * @param old   expected old value in var
 * @param new   new value for var if old value matches
 * @return true of old value atomically replaced with new, else false
 */
bool _atomic_compareAndSwapWeak(volatile int *var, int old, int new);

#endif                          /* _ATOMIC_H_ */
