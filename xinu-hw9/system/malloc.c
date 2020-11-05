/**
 * @file malloc.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Request heap storage, record accounting information, returning pointer
 * to assigned memory region.
 *
 * @param size
 *      number of bytes requested
 *
 * @return
 *      pointer to region on success, or @c NULL on failure
 */
void *malloc(ulong size)
{
    struct memblock *pmem, *finalAddress, *returnAddress;

    /* we don't allocate 0 bytes. */
    if (0 == size)
    {
        return NULL;
    }

     /** TODO:
      *      1) Make room for accounting info
      *      2) Acquire memory with getmem syscall
      *         - handle possible error (SYSERR) from getmem...
      *      3) Set accounting info in pmem
      *      4) Return proper pointer to base of free memory region
      */


    //Setting pmem next to itself
    pmem->next = pmem;

    //Set the length of the pmem block
    pmem->length = size;

    //obtain address from getmem()
    //kprintf("size + pmem: %d\r\n", size + sizeof(struct memblock));
    pmem = getmem(size + sizeof(struct memblock));
    //kprintf("Return Address: 0x%08x\r\n", pmem);


    //calc final address
    finalAddress =  1 + pmem;
    //kprintf("Final Address: 0x%08x\r\n", finalAddress);
    //kprintf("Size of struct block: %d\r\n", sizeof(struct memblock) );
	

    //return getmem() - 8 bytes
    return finalAddress; 

  
}
