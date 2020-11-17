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
    struct freeblock *head, *free2;
  
    struct dentry *phw;
    int result = 1;
    int diskfd;

    kprintf("We make is past intiializing in sbFreeBlock()\r\n");

    //initialzing diskfd (not the disk) 
    if (NULL == psuper)
    {
        return SYSERR;
    }
    phw = psuper->sb_disk;
    if (NULL == phw)
    {
        return SYSERR;
    }
    diskfd = phw - devtab;


    //locking
    wait(psuper->sb_freelock);

    head = psuper->sb_freelst;    

    kprintf("We aren't stuck on locking\r\n");

    //traverse the collector nodes
    while(head->fr_next != NULL)
    {
       head = head->fr_next;
       kprintf("block num: %d\r\n", head->fr_blocknum);
    }

    kprintf("We made it out of traversing the free list!\r\n");
    

    // dealing withn full edge case
    // if(head->fr_count > 60)
    // {
    //     return SYSERR
    // }

    //what happens when full?


    //place block in fr_free at new fr_count
    head->fr_free[head->fr_count] = block;
  
    head->fr_count++;    

    kprintf("Inserting: %d at [%d]\r\n", block, head->fr_count);

     free2 = head->fr_next;
        if (NULL == head->fr_next)
        {
            head->fr_next = 0;
        }
        else
        {
            head->fr_next =
                (struct freeblock *)head->fr_next->fr_blocknum;
        }
        seek(diskfd, head->fr_blocknum);
        if (SYSERR == write(diskfd, head, sizeof(struct freeblock)))
        {
            return SYSERR;
        }
       head->fr_next = free2;
        if (!result)
            result = SYSERR;

    //unlocking
    signal(psuper->sb_freelock);

    return SYSERR;
}
