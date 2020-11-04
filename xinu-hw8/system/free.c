/**
 * @file free.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/**
 * Attempt to free a block of memory based on malloc() accounting information
 * stored in preceding two words.
 *
 * @param ptr
 *      A pointer to the memory block to free.
 */
syscall free(void *ptr)
{
    struct memblock *block;
    ulong nbytes; 

    /* TODO:
     *      1) set block to point to memblock to be free'd (ptr)
     *      2) find accounting information of the memblock
     *      3) call freemem syscall on the block with its length
     */

    block = (struct memblock *)ptr;
    //kprintf("Block length: %d\r\n", block->length);
    //kprintf("Block adress before subtraction: 0x%08x\r\n", block);
    block = -1 + block;
    nbytes = block->length;
    //kprintf("Block adress after subtraction: 0x%08x\r\n", block);
    //kprintf("Block length: %d\r\n", block->length);

    kprintf("nbytes: %d\r\n", nbytes);

    freemem(block, nbytes); 

    return OK;
}
