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
    
     //set *curr to head of the freelist
     curr = freelist.head;

     //traverse the free list 
     while((curr->next) != NULL)
     {
     	//check if current memblock is best fit
	if((curr->length) >= nbytes)
	{
             //intialize new memblck
	     register memblck *newblck;

             //assign new memlist head
             newblck->next = memhead.head       
             memhead.head = newblck;

	     //save prev block and calc length
	     prev = curr->next
	     newblck->length = (prev->length) - nbytes

             
	     //calc and assign left over, honestly not really sure what to do here.

             //release memory lock
             lock_release(&(freelist.lock));
             
	     //return adress of new memblock
             return (&newblck)
	}

     } 
    
    return (void *)SYSERR;
}
