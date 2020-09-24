/**
 ** COSC 3250 - Project 4
 ** Creates new process
 ** @author Daniel Throop and Brea Brennan 
 ** Instructor Brylow
 ** TA-BOT:MAILTO daniel.throop@marquette.edu brea.brennan@marquette.edu 
 **/

/**
 * @file create.c
 * @provides create, newpid, userret
 *
 * COSC 3250 / COEN 4820 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

static pid_typ newpid(void);
void userret(void);
void *getstk(ulong);

/**
 * Create a new process to start running a function.
 * @param funcaddr address of function that will begin in new process
 * @param ssize    stack size in bytes
 * @param name     name of the process, used for debugging
 * @param nargs    number of arguments that follow
 * @return the new process id
 */
syscall create(void *funcaddr, ulong ssize, char *name, ulong nargs, ...)
{
    ulong *saddr;               /* stack address                */
    ulong pid;                  /* stores new process id        */
    pcb *ppcb;                  /* pointer to proc control blk  */
    ulong i;
    va_list ap;                 /* points to list of var args   */
    ulong pads = 0;             /* padding entries in record.   */
    void INITRET(void);

    if (ssize < MINSTK)
        ssize = MINSTK;
    ssize = (ulong)(ssize + 3) & 0xFFFFFFFC; //rounding up the stack size to a nice number
   
    /* round up to even boundary    */
    saddr = (ulong *)getstk(ssize);     /* allocate new stack and pid   */
    pid = newpid(); //gets the next pid in the queue to be used

    /* a little error checking      */
    if ((((ulong *)SYSERR) == saddr) || (SYSERR == pid))
    {
        return SYSERR;
    }

    numproc++;
    ppcb = &proctab[pid];

    // TODO: Setup PCB entry for new process. (seting the fileds in the struct in ../include/proc.h  [DONE]
    
    ppcb->stklen = ssize;
    strncpy(ppcb->name, name, PNMLEN);
    ppcb->stkptr = NULL;
    ppcb->state = PRSUSP;  
    ppcb->stkbase = (ulong *)(saddr - ssize);

    // 1.stack size = ssize
    // 2.stack name use the function strncpy(1. pointer to the string, 2.Paramenter being set, 3.Length of the string);
    // 		1. pcbr name field
    // 		2. create() argument for name 
    // 		3.PNMLEN amount
    // 3.stack pointer = NULL
    // 4.state of process = defined in proc.h file
    // 5.stack base (is the bottom of the stack)= stake adder(address in memory)  - size of the stack  

    /* Initialize stack with accounting block. */
    *saddr = STACKMAGIC; //constant to determine where the stack actual ends
    *--saddr = pid; //puts the pid into the stack
    *--saddr = ppcb->stklen; //length of the stack ssize
    *--saddr = (ulong)ppcb->stkbase; //the address of the stack base

    /* Handle variable number of arguments passed to starting function   */
    if (nargs) 
    {
        pads = ((nargs - 1) / 4) * 4; //how many spaces that we need to store any additional arguments
    }
    /* If more than 4 args, pad record size to multiple of native memory */
    /*  transfer size.  Reserve space for extra args                     */
    for (i = 0; i < pads; i++)
    {
        *--saddr = 0;
    }
    
    //void* tempPoint;
    //tempPoint = saddr;


    //setting 16 registers to 0
    //Jack felt funky about this (!)

    for (i = 0; i < 16; i++)
    {
	*--saddr = 0;
    }
    
    //assign stack pointer with stack address

    ppcb->stkptr = saddr;

    va_start(ap, nargs);

    //assign arguments to registers

    for(i = 0; i < nargs; i++)
    { 
	//R0 - R3 
	if(i < 4)
	{
		saddr[CTX_R0 + i] = va_arg(ap, ulong); 
       	}
	//padding registers
	else
	{
		saddr[CTX_PC + (i - 3)] = va_arg(ap, ulong);
		//tempPoint[i - 4] = va_arg(ap, int);
	}	
    }
    
    //end argument list
 
    va_end(ap);

    //assign program counter, link register, and stack pointer
    saddr[CTX_SP] = &saddr[CTX_PC + 1]; // needs to point to the padding in the way we designed our ctswt 
    saddr[CTX_PC] = funcaddr;
    saddr[CTX_LR] = &userret;

    // TODO: Initialize process context.
    //		-make space on the stack for all 16 of your registers and set them to 0
    //		-r15 (instruction pointer) aka program counter -- address of the function you want to run (argument to the create function funcaddr)
    //		-r14 (Link Register) - keep track of the function that calls it - pass the value of &userret
    //		-r13 (stack pointer)
    //		-r12-r0 (content registers)
    //
    // TODO:  Place arguments into activation record.
    //        See K&R 7.3 for example using va_start, va_arg and
    //        va_end macros for variable argument functions.
    //			-set the pcd's stkptr to the last register r0
    //			-add the arguments to their desinated areas 
    //
    // The stack grows down in length so the larger the address the closer to the top (begining) of the stack it     

    return pid;
}

/**
 * Obtain a new (free) process id.
 * @return a free process id, SYSERR if all ids are used
 */
static pid_typ newpid(void)
{
    pid_typ pid;                /* process id to return     */
    static pid_typ nextpid = 0;

    for (pid = 0; pid < NPROC; pid++)
    {                           /* check all NPROC slots    */
        nextpid = (nextpid + 1) % NPROC;
        if (PRFREE == proctab[nextpid].state)
        {
            return nextpid;
        }
    }
    return SYSERR;
}

/**
 * Entered when a process exits by return.
 */
void userret(void)
{
    kprintf("We made it!");
    kill(currpid);
}
