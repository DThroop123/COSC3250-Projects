/**
 * @file pthread.h
 *
 * COSC 3250 
 */
/* Embedded XINU, Copyright (C) 2020.  All rights reserved. */

#ifndef _PTHREAD_H_
#define _PTHREAD_H_

#ifndef __ASSEMBLER__

typedef pid_typ pthread_t;
typedef int     pthread_attr_t;
typedef int	pthread_mutex_t;

syscall pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		       void *(*start_routine) (void *), void *arg);
syscall pthread_join(pthread_t thread, void **retval);
syscall pthread_mutex_lock(pthread_mutex_t *mutex);
syscall pthread_mutex_trylock(pthread_mutex_t *mutex);
syscall pthread_mutex_unlock(pthread_mutex_t *mutex);

#endif                          /*  __ASSEMBLER__  */

/* pthread initialization constants */
#define PTHREAD_MUTEX_INITIALIZER 	LOCK_UNLOCKED

#endif                          /* _PTHREAD_H_ */
