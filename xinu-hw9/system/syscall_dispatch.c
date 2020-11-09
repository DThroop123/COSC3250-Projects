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


/**
 *  * syscall wrapper for getmem().
 *   * @param args expands to: int descrp, ulong nbytes 
 *    */
syscall sc_getmem(int *args)
{
    int descrp = SCARG(int, args);
    ulong nbytes = SCARG(ulong, args);

    if (0 == descrp)
        return getmem(nbytes);
    return SYSERR;
}

syscall getmem(int descrp, ulong nbytes)
{
    SYSCALL(GETMEM);  //where do we get these constants? (?)
}

/**
 * syscall wrapper for freemem().
 * @param args expands to: int descrp, void *memptr, ulong nbytes 
 **/ 
syscall sc_freemem(int *args)
{
    int descrp = SCARG(int, args);
    void *memptr = SCARG(void *, args); //Is this how you unpack a void pointer? (?)
    ulong nbytes = SCARG(ulong, nbytes);

    if (0 == descrp)
        return freemem(memptr, nbytes);
    return SYSERR;
}

syscall freemem(int descrp, void *memptr, ulongnbytes)
{
    SYSCALL(FREEMEM);
}

/**
 * syscall wrapper for ptcreate().
 * @param args expands to: int descrp, pthread_t *thread, const pthread_attr_t *attr,
 *  void *(*start_routine) (void *), void *arg 
 **/
syscall sc_ptcreate(int *args)
{
    int descrp = SCARG(int, args);
    pthread_t *thread = SCARG(pthread_t, args); // Is this how you unpack pointer to pthread_t? (?)
    const pthread_attr_t *attr = SCARG(const pthread_attr_t, args);
    void *(*start_routine) (void *) = SCARG(//?, args);  I dont even know how to unpack this? (?)
    void *arg = SCARG(arg, args);

    if (0 == descrp)
        return pthread_create(thread, attr, start_routine, arg); //I think I passed this correctly? (?)
    return SYSERR;
}

syscall ptcreate(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg) 
{
    SYSCALL(PTCREATE);
}

/**
 * syscall wrapper for ptjoin().
 * @param args expands to: int descrp, pthread_t thread, void **retval
 *
 **/
syscall sc_ptjoin(int *args)
{
    int descrp = SCARG(int, args);
    pthread_t thread = SCARG(pthread_t, args);
    void **retval = SCARG(void, args); // not sure how ot unpack void poitner being derefernced? (?)

    if (0 == descrp)
        return pthread_join(thread, retval); //I think I passed this correctly? (?)
    return SYSERR;
}

syscall ptjoin(pthread_t thread, void **retval)  
{
    SYSCALL(PTJOIN);
}


syscall sc_ptlock(int *);
syscall sc_ptunlock(int *);
syscall sc_pttrylock(int *);
