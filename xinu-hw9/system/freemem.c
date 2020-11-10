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
syscall freemem_real(void *memptr, ulong nbytes)
{
    //we added a curr pointer to traverse
    register struct memblock *block, *next, *prev;

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

    lock_acquire(&(freelist.lock));

    //set next and prev
    next = (struct memblock *)freelist.head;
    prev = (struct memblock *)&freelist; 

    //traverse the freelist
    while(next != NULL)
    {
       if((prev < block) && (block < next))
       {
            //insert block
            prev->next = block;
            block->next = next;
     
            block->length = nbytes;

            //update freeslist length
            freelist.length = freelist.length + nbytes;
            break;      
       }

     //update vars
     prev = next;
     next = next->next;

    } 

    lock_release(&(freelist.lock));

    return OK;
}
