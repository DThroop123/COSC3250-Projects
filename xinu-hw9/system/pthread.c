#include <xinu.h>

//UPDATED
syscall pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	SYSCALL(PTCREATE);
}

//UPDATED
syscall pthread_join(pthread_t thread, void **retval);
{
	SYSCALL(PTJOIN);
}

//UPDATED
syscall pthread_mutex_lock(pthread_mutex_t *mutex)
{
	SYSCALL(PTLOCK);
}

//UPDATED
syscall pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	SYSCALL(PTTRYLOCK);
}

//UPDATED
syscall pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	SYSCALL(PTUNLOCK);
}








