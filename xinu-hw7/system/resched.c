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

/*
 * Helper function that preforms the operation of getting all the tickets 
 * in the ready processes and totals them up.  
 *
 * */

uint totalTickets(void)
{
	unsigned int total = 0;
	int i;
	
	//obtaining the total number of tickets for all ready and current processes	
	for(i = NCORES; i < NPROC; i++)
	{
		//Only gets the tickets from the processes that are in the ready state (current process is set to ready before this)
		if((&proctab[i])->state == PRREADY)
		{
			total += (&proctab[i])->tickets;
		}		
	}

	return total;

}


/*
 * Helper function that preforms the operation of choosing the random winning
 * ticket and determines the process that holds this winning ticket
 *
 * */

int pickWinner(uint total)
{

	uint winner;
	int counter = 0;
	int i;

	//Chooses the winning ticket randomly
	winner = random(total);

        //if tickets <= 3 its the null process for one of the cores
        //jusr assign the winner to be the process in index 5?
        //or would we just keep calling random()?
	for(i = NCORES; i < NPROC; i++)
	{
		//Makes sure that only the processes that are in the ready state are considered in the lottery
		if((&proctab[i])->state == PRREADY)
		{	
			
			counter += (&proctab[i])->tickets;
	
			//Checks to see if the counter is greater than or equal to the winner
				//if so then the process index is 'i' from the for loop	
			if(counter >= winner)
			{
				return i;	
			}
		}
	}

	//if the function gets to here, it has not found an eligible winner
	//this would return the
	//kprintf("%d", getcpuid());
	return (getcpuid());	
		
}


syscall resched(void)
{
    pcb *oldproc;               /* pointer to old process entry */
    pcb *newproc;               /* pointer to new process entry */
    uint total = 0;
    static volatile int lock = LOCK_UNLOCKED;

    lock_acquire(&lock); 

    // This now supports multi-core
    //kprintf("%d\n\r", currpid[getcpuid()]);
    oldproc = &proctab[currpid[getcpuid()]];
    /* place current process at end of ready queue */
    if (PRCURR == oldproc->state)
    {
        oldproc->state = PRREADY;
        enqueue(currpid[getcpuid()], readylist);
    }

    /**
     * We recommend you use a helper function for the following:
     * TODO: Get the total number of tickets from all processes that are in current and ready states.
     * Utilize the random() function to pick a random ticket. 
     * Traverse through the process table to identify which proccess has the random ticket value.
     * Remove process from queue.
     * Set currpid to the new process.
     */

    total = totalTickets(); /*Finds the total number of tickets*/
    currpid[getcpuid()] = pickWinner(total); /*Sets the current pid to the process that contains the winning ticket*/
    remove(currpid[getcpuid()]); /*Removes the new currpid from the ready list becuase it is going to be  the current running process*/      
 
    newproc = &proctab[currpid[getcpuid()]];
    newproc->state = PRCURR;    /* mark it currently running*/

#if PREEMPT
    preempt[getcpuid()] = QUANTUM;
#endif

    //release lock
    lock_release(&lock);

    //kprintf("[%d, %d]\r\n", oldproc - proctab, newproc - proctab); 
    ctxsw(&oldproc->stkptr, &newproc->stkptr);

    /* The OLD process returns here when resumed. */
    return OK;
}
