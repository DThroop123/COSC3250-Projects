#include <xinu.h>

pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{

}

pthread_join(pthread_t thread, void **retval);
{


}

syscall pthread_mutex_lock(pthread_mutex_t *mutex)
{
	lock_acquire(&mutex);
}

syscall pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	
	irqmask im;
   	im = disable();
  	while (FALSE == _atomic_compareAndSwapWeak(&mutex,
                                                 LOCK_UNLOCKED,
                                                 LOCK_LOCKED | (im & ARM_I_BIT)))
  	{                           // do nothing while cas fails
   	}

   	return OK;

}

syscall pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	lock_release(&mutex);
}








