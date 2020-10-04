/**
 * @file resched.c
 * @provides resched
 *
 * COSC 3250 Assignment 4
 */
/* Embedded XINU, Copyright (C) 2008.  All rights reserved. */

#include <xinu.h>

extern void ctxsw(void *, void *);
/**
 * Reschedule processor to next ready process.
 * Upon entry, currpid gives current process id.  Proctab[currpid].pstate 
 * gives correct NEXT state for current process if other than PRREADY.
 * @return OK when the process is context switched back
 */

uint totalTickets(void)
{
	unsigned int total = 0;
	int i;
	
	//obtaining the total number of tickets for all ready and current processes	
	for(i = 0; i < NPROC; i++)
	{
		if(((&proctab[i])->state == PRREADY) || ((&proctab[i])->state == PRCURR))
		{
			total += (&proctab[i])->tickets;
		}		
	}

	kprintf("Total: %d\r\n", total); 	
	return total;

}

int pickWinner(uint total)
{

	kprintf("Total (in pickWinner()): %d\r\n", total); 	
	uint winner;
	winner = random(total);
	int counter = 0;
	int i;

	if(winner == 0)
	{
		winner += 1;
	}

	for(i = 0; i < NPROC; i++)
	{
		counter += (&proctab[i])->tickets;
		
		if(counter >= winner)
		{
			kprintf("Counter: %d\r\n", counter);
			kprintf("Winner: %d\r\n", winner);	
			return i;	
		}
	}	
}


syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */
    uint total = 0;

    oldproc = &proctab[currpid];

    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid, readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
     */

    total = totalTickets();
    currpid = pickWinner(total);
    kprintf("CurrPid: %d\r\n", currpid);
    remove(currpid);      
 
    newproc = &proctab[currpid];
    newproc->state = PRCURR;    /* mark it currently running    */

#if PREEMPT
    preempt = QUANTUM;
#endif

    kprintf("[%d, %d]\r\n", oldproc - proctab, newproc - proctab);
    ctxsw(&oldproc->stkptr, &newproc->stkptr);

    /* The OLD process returns here when resumed. */
    return OK;
}
