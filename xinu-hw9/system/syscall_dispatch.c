/**
 * @file syscall_dispatch.c
 * Translate user_* functions to system calls, dispatch to proper sc_*
 * functions, and complete.
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/* for syscall argument parsing */
#define SCARG(type, args)  (type)(*args++)

/* macro to make user->kernel syscalls */
#define SYSCALL(num) int status; \
	asm("swi %0" : : "i"(SYSCALL_##num)); \
	asm("mov %0, r0" : "=r"(status)); \
	return status;


/* syscall wrapper prototypes */
syscall sc_none(int *);
syscall sc_yield(int *);
syscall sc_getc(int *);
syscall sc_putc(int *);
syscall sc_getmem(int *);
syscall sc_freemem(int *);
syscall sc_ptcreate(int *);
syscall sc_ptjoin(int *);
syscall sc_ptlock(int *);
syscall sc_ptunlock(int *);
syscall sc_pttrylock(int *);

/* table for determining how to call syscalls */
const struct syscall_info syscall_table[] = {
    { 5, (void *)sc_none },     /* SYSCALL_NONE      = 0  */
    { 0, (void *)sc_yield },    /* SYSCALL_YIELD     = 1  */
    { 1, (void *)sc_none },     /* SYSCALL_SLEEP     = 2  */
    { 1, (void *)sc_none },     /* SYSCALL_KILL      = 3  */
    { 2, (void *)sc_none },     /* SYSCALL_OPEN      = 4  */
    { 1, (void *)sc_none },     /* SYSCALL_CLOSE     = 5  */
    { 3, (void *)sc_none },     /* SYSCALL_READ      = 6  */
    { 3, (void *)sc_none },     /* SYSCALL_WRITE     = 7  */
    { 1, (void *)sc_getc },     /* SYSCALL_GETC      = 8  */
    { 2, (void *)sc_putc },     /* SYSCALL_PUTC      = 9  */
    { 2, (void *)sc_none },     /* SYSCALL_SEEK      = 10 */
    { 4, (void *)sc_none },     /* SYSCALL_CONTROL   = 11 */
    { 1, (void *)sc_none },     /* SYSCALL_NONE      = 12 */
    { 1, (void *)sc_none },     /* SYSCALL_NONE      = 13 */
    { 1, (void *)sc_none },     /* SYSCALL_NONE      = 14 */
    { 1, (void *)sc_none },     /* SYSCALL_NONE      = 15 */
    { 1, (void *)sc_none },     /* SYSCALL_NONE      = 16 */
    { 1, (void *)sc_getmem },   /* SYSCALL_GETMEM    = 17 */
    { 2, (void *)sc_freemem },  /* SYSCALL_FREEMEM   = 18 */
    { 4, (void *)sc_ptcreate }, /* SYSCALL_PTCREATE  = 19 */
    { 2, (void *)sc_ptjoin },   /* SYSCALL_PTJOIN    = 20 */
    { 1, (void *)sc_ptlock },   /* SYSCALL_PTLOCK    = 21 */
    { 1, (void *)sc_ptunlock }, /* SYSCALL_PTUNLOCK  = 22 */
    { 1, (void *)sc_ptrylock }, /* SYSCALL_PTTRYLOCK = 23 */
};

int nsyscall = sizeof(syscall_table) / sizeof(struct syscall_info);

/**
 * Syscall dispatch routine.  Given a syscall code and pointer to
 * arguments, change execution to function.  Otherwise, generate error
 * saying no such syscall.
 * @param code syscall code to execute
 * @param args pointer to arguments for syscall
 */
int syscall_dispatch(int code, int *args)
{
    if (0 <= code && code < nsyscall)
    {
        return (*syscall_table[code].handler) (args);
    }
    kprintf("ERROR: unknown syscall %d!\r\n", code);
    return SYSERR;
}

/**
 * syscall wrapper for *no* function, just a test/demo.
 * @param args expands to: none.
 */
syscall sc_none(int *args)
{
    return OK;
}

syscall user_none(void)
{
    SYSCALL(NONE);
}

/**
 * syscall wrapper for yield().
 * @param args expands to: none.
 */
syscall sc_yield(int *args)
{
    /* this may change thread of execution, allow exceptions */
    // exlreset();

    return resched();
}

syscall user_yield(void)
{
    SYSCALL(YIELD);
}

/**
 * syscall wrapper for getc().
 * @param args expands to: int descrp
 */
syscall sc_getc(int *args)
{
    int descrp = SCARG(int, args);
    if (0 == descrp)
    {
        return kgetc();
    }
    return SYSERR;
}

syscall user_getc(int descrp)
{
    SYSCALL(GETC);
}

/**
 * syscall wrapper for putc().
 * @param args expands to: int descrp, char character
 */
syscall sc_putc(int *args)
{
    int descrp = SCARG(int, args);
    char character = SCARG(char, args);

    if (0 == descrp)
        return kputc(character);
    return SYSERR;
}

syscall user_putc(int descrp, char character)
{
    SYSCALL(PUTC);
}


//we dont want to make getmem and freemem have user sycalls correct? (?)

//NOT UPDATED
/**
 *  * syscall wrapper for getmem().
 *   * @param args expands to: int descrp, ulong nbytes 
 *    */
syscall sc_getmem(int *args)
{
    ulong nbytes = SCARG(ulong, args);  //do we need to move everything that current is in getmem() to the system call version?

    return getmem_real(nbytes);
}

syscall getmem(ulong nbytes)
{
    SYSCALL(GETMEM);  //where do we get these constants? (?)
}

//NOT UPDATE
/**
 * syscall wrapper for freemem().
 * @param args expands to: int descrp, void *memptr, ulong nbytes 
 **/ 
syscall sc_freemem(int *args)
{
    void *memptr = SCARG(void *, args);
    ulong nbytes = SCARG(ulong, nbytes);

    return freemem_real(memptr, nbytes);
}

syscall freemem(void *memptr, ulongnbytes)
{
    SYSCALL(FREEMEM);
}

//UPDATED
/**
 * syscall wrapper for ptcreate().
 * @param args expands to: pthread_t *thread, const pthread_attr_t *attr,
 *  void *(*start_routine) (void *), void *arg 
 **/
syscall sc_ptcreate(int *args)
{
    pthread_t *thread = SCARG(pthread_t *, args);
    const pthread_attr_t *attr = SCARG(const pthread_attr_t, args);
    void *(*start_routine) (void *) = SCARG(void *, args);  //possible source of error
    void *arg = SCARG(arg, args);

    return create(thread, attr, start_routine, arg); //possible source of error
}


//UPDATED (?)
/**
 * syscall wrapper for ptjoin().
 * @param args expands to: pthread_t thread, void **retval
 **/
syscall sc_ptjoin(int *args)  //does all the work for the join() method occur in here? With out current setup (?)
{
    pthread_t thread = SCARG(pthread_t, args);
    void **retval = SCARG(void, args);

    //1. intialize new joinqueue in PCB struct and PRJOIN state in proc.h -> DONE
    
    //2. calling process->PRJOIN -> DONE
    (&proctab[currpid[getcpuid()]])->state = PRJOIN; 
    
    //3. Enqeue calling process in process B's joinqueue -> DONE
    enqueue(&proctab[currpid[getcpuid()]], (&proctab[thread])->joinqueue); // do we want to get teh adress of the joinqueue here? (?)i

    //4. yield the processor -> DONE
    user_yield();

    //5. Modify kill() to to dequeue the current process and make ready any processes waiting in the dying processes's joinqueue

    //NOTE: Be sure to allocate enough new entries in the global queue table, and to initialize all of these new queues in sysinit(). 

    return OK; 
}

//UPDATED (?)
/**
 * syscall wrapper for ptlock().
 * @param args expands to: pthread_mutex_t *mutex
 **/
syscall sc_ptlock(int *args)
{
	pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);       

	return lock_acquire(&mutex); //do we return this call? (?)
}

//UPDATED (?)
/**
 * syscall wrapper for ptunlock().
 * @param args expands to: pthread_mutex_t *mutex
 **/
syscall sc_ptunlock(int *args)
{
 	pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);

        return lock_release(&mutex); //do we return this call? (?)
}

//UPDATED
//NOTE: returns 0 on sucess and 1 on failure
/**
 * syscall wrapper for pttrylock().
 * @param args expands to: pthread_mutex_t *mutex
 **/
syscall sc_pttrylock(int *args)
{
        pthread_mutex_t *mutex = SCARG(pthread_mutex_t *, args);

	irqmask im;
        im = disable();
        if(FALSE == _atomic_compareAndSwapWeak(&mutex,
                                                 LOCK_UNLOCKED,
                                                 LOCK_LOCKED | (im & ARM_I_BIT)))
        {
                restore(im);
                return 1;                           //return 1 if failed
        }

        return 0; //return 0 if succesful
}
