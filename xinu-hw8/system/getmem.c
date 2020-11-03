/*
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
    memblk *prev, *curr, *leftover;

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
     curr = (struct memblock *)(&(freelist.head));
     prev = (struct memblock *)&freelist;
     memhead * freehead = &freelist;

     kprintf("curr next: 0x%08x\r\n", curr->next);

     //traverse the free list 
     while(curr != NULL)
     {
	kprintf("curr next: 0x%08x\r\n", curr->next);

     	//check if current memblock is best fit
	if((curr->length) > nbytes)
	{ 
	     //moves the next pointer to the new allocated end of the block
	     leftover = (struct memblock *)(nbytes + ((ulong)curr));
	     leftover->length = (curr->length) - nbytes;
             //kprintf("Leftover length: %d\r\n", leftover->length);
             kprintf("Leftover: 0x%08x\r\n", leftover);
             kprintf("curr: 0x%08x\r\n", curr);
 
             //update free list length
	     freehead->length = freehead->length - nbytes;

             //set new links
             leftover->next = curr->next;
             prev->next = leftover;
	     kprintf("curr next: 0x%08x\r\n", curr->next);
	      
             //release memory lock
             lock_release(&(freelist.lock));
             
	     //return adress of new memblock
             return (&curr);
	}

        //set new vars
	prev = curr;
        curr = curr->next;

     } 
    
    return (void *)SYSERR;
}
