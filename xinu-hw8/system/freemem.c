/**
 * @file freemem.c
 *
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Frees a block of heap-allocated memory.
 *
 * @param memptr
 *      Pointer to memory block allocated with memget().
 *
 * @param nbytes
 *      Length of memory block, in bytes.  (Same value passed to memget().)
 *
 * @return
 *      ::OK on success; ::SYSERR on failure.  This function can only fail
 *      because of memory corruption or specifying an invalid memory block.
 */
syscall freemem(void *memptr, ulong nbytes)
{
    //we added a curr pointer to traverse
    register struct memblock *block, *curr, *next, *prev;
    ulong top;

    /* make sure block is in heap */
    if ((0 == nbytes)
        || ((ulong)memptr < freelist.base)
        || ((ulong)memptr > freelist.base + freelist.bound))
    {
        return SYSERR;
    }

    block = (struct memblock *)memptr;
    nbytes = (ulong)roundmb(nbytes);

    /* TODO:
     *      - Acquire freelist lock
     *      - Find where the memory block should
     *        go back onto the freelist (based on address)
     *        		- the first memblock would move the head to the deallocated space, then create a new membloack in the list for the remaining
     *        		  main part of the first list
     *      - Find top of previous memblock
     *      - Make sure block is not overlapping on prev or next blocks
     *      - Coalesce with previous block if adjacent
     *      - Coalesce with next block if adjacent
     */

    lock_aquire(&(freelist.lock));

    //set curr and prev to head of freelist
    curr = freelist.head;
    prev = freelist.head;

    //traverse the freelist
    while((curr->next) != NULL)
    {
         //we find the address
         if((&curr) == (&block))
         {
             //move head of free list
             freelist.head = (&block);

             //add the freed bytes back onto length
             freelist.length += nbytes;   

         }
      
         //update vars
	 prev = curr;
	 curr = curr->next;

    } 
    











    return OK;
}
