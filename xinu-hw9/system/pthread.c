#include <xinu.h>

pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg)
{
	ulong pid; 

    
        //not really sure what to do here? Where does the thread pointer come into play? Do we pass it to create()? 


        pid = create(start_routine, INITSTK, 5, "PROBABLY WRONG", 1, arg);
	return pid;
}

pthread_join(pthread_t thread, void **retval);
{


}

//wrapper function to lock pthread mutex
//returns call to lock_acquire(&mutex)
syscall pthread_mutex_lock(pthread_mutex_t *mutex)
{
	lock_acquire(&mutex);
}

//function that determines if lock is in use
//returns 0 if lock of &mutex is currently locked, and 1 if it is not
syscall pthread_mutex_trylock(pthread_mutex_t *mutex)
{	
	irqmask im;
   	im = disable();
  	if(FALSE == _atomic_compareAndSwapWeak(&mutex,
                                                 LOCK_UNLOCKED,
                                                 LOCK_LOCKED | (im & ARM_I_BIT)))
  	{
		return 0;                           // do nothing while cas fails
   	}

   	return 1;
}


//wrapper function to unlock pthread mutex
//returns call to lock_release(&mutex)
syscall pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	lock_release(&mutex);
}








