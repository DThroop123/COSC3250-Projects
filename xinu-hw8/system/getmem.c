/**
 * @file getmem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Allocate heap memory.
 *
 * @param nbytes
 *      Number of bytes requested.
 *
 * @return
 *      ::SYSERR if @p nbytes was 0 or there is no memory to satisfy the
 *      request; otherwise returns a pointer to the allocated memory region.
 *      The returned pointer is guaranteed to be 8-byte aligned.  Free the block
 *      with memfree() when done with it.
 */
void *getmem(ulong nbytes)
{
    register memblk *prev, *curr, *leftover;

    if (0 == nbytes)
    {
        return (void *)SYSERR;
    }

    /* round to multiple of memblock size   */
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Acquire lock for freelist head -> the lock is already created in the memhead
     *      - Traverse through the freelist
     *        to find a block that's suitable 
     *        (Use First Fit with remainder splitting)
     *        		-base never changes
     *        		-need to create a new memblock after space has been requested
     *        			- head moves to the new start
     *        			- length is what is was previously - the amount allocated
     *        		-if all memory is reserved then the head points to null
     *      - Release memory lock
     *      - return memory address if successful
     */

     lock_acquire(&(freelist.lock));
    
     //set *curr and *prev to head of the freelist
     curr = freelist.head;
     prev = freelist.head;

     //traverse the free list 
     while(curr != NULL)
     {
     	//check if current memblock is best fit
	if((curr->length) >= nbytes)
	{
             //intialize new memblck
	     struct memblock *newblck;
 
             //assign new head to newblck
             newblck = curr;

	     //moves the next pointer to the new allocated end of the block
	     prev->next = nbytes + curr;

	     //move the curr to the end of the remaining block allocated
	     curr = prev->next;

	     //Update curent next and length
	     curr->next = newblck->next;
	     curr->length = newblock->length - nbytes;

	     //Assign nbytes
	     newblck->length = nbytes;

             //update free list length
	     freelist.length = (prev->length) - nbytes;
	      
             //release memory lock
             lock_release(&(freelist.lock));
             
	     //return adress of new memblock
             return (&newblck);
	}

        //set new vars
	prev = curr;
        curr = curr->next;

     } 
    
    return (void *)SYSERR;
}
