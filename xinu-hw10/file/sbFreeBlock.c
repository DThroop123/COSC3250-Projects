/* sbFreeBlock.c - sbFreeBlock */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <device.h>
#include <memory.h>
#include <disk.h>
#include <file.h>

/*------------------------------------------------------------------------
 * sbFreeBlock - Add a block back into the free list of disk blocks.
 *------------------------------------------------------------------------
 */
devcall sbFreeBlock(struct superblock *psuper, int block)
{
    // TODO: Add the block back into the filesystem's list of
    //  free blocks.  Use the superblock's locks to guarantee
    //  mutually exclusive access to the free list, and write
    //  the changed free list segment(s) back to disk.

    //intialize head
    struct freeblock *head;
    head = psuper->sbfree_lst;

    //traverse the collector nodes
    while(head->fr_next != NULL)
    {
       head = head->fr_next;
    }

    // dealing withn full edge case
    // if(head->fr_count > 60)
    // {
    //     return SYSERR
    // }

    head->fr_count++;

    head->fr_free[head->fr_count] = block;

    return SYSERR;
}
