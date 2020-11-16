/* fileDelete.c - fileDelete */
/* Copyright (C) 2008, Marquette University.  All rights reserved. */
/*                                                                 */
/* Modified by                                                     */
/*                                                                 */
/* and                                                             */
/*                                                                 */
/*                                                                 */

#include <kernel.h>
#include <memory.h>
#include <file.h>

/*------------------------------------------------------------------------
 * fileDelete - Delete a file.
 *------------------------------------------------------------------------
 */
devcall fileDelete(int fd)
{
    // TODO: Unlink this file from the master directory index,
    //  and return its space to the free disk block list.
    //  Use the superblock's locks to guarantee mutually exclusive
    //  access to the directory index.

    struct filenode *file;
    kprintf("We make it into fileDelete()\r\n");
    //locking 
    wait(supertab->sb_freelock);

    kprintf("We lock\r\n");

    //traversing sb_dirlst[]

    //accesing the file from the mastery directory index
    //what index would we use on the directory list? (?)
    //do we use somehting from fd to indicate our position on the
    //directory list? 

    file = &(supertab->sb_dirlst->db_fnodes[fd]);  //there is only one directory list node 

    kprintf("we make it to sbFreeBlock\r\n");
    
    //unlocking
    signal(supertab->sb_freelock);

    sbFreeBlock(supertab, file->fn_blocknum);



    return OK;
}
