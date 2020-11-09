/**
 * @file kill.c
 * Provides: kill
 *
 * COSC 3250/ COEN 4820 Assignment 4
 */

/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

/*
 * kill  --  kill a process and remove it from the system
 */
syscall kill(int pid)
{
    pcb *ppcb;                  /* points to process control block for pid */

    if (isbadpid(pid) || (0 == pid)
        || (PRFREE == (ppcb = &proctab[pid])->state))
    {
        return SYSERR;
    }

    ppcb = &proctab[pid]; //pointer to the process control block

    atomic_decrement(&numproc);
   
    //readying processes in joinqueue
    //dequeueing processes

    for(int i = 0; i < NPROC; i++)
    {
      &proctab[(ppcb->joinqueue[i])]->state = PRREADY;
      dequeue(ppcd->joinqueue[i]);    
    }

    switch (ppcb->state)
    {
    case PRCURR:
        ppcb->state = PRFREE;   /* suicide */
        freemem(ppcb, ppcb->stklen);
        resched(); //looks for the next procces in the que and put it on the cpu

    case PRREADY:
        remove(pid);

    default:
        ppcb->state = PRFREE;
        freemem(ppcb, ppcb->stklen);
    }
    


    return OK;
}
