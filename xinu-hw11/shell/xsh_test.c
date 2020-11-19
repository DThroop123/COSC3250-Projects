/**
 * @file     xsh_test.c
 * @provides xsh_test
 *
 */
/* Embedded XINU, Copyright (C) 2009.  All rights reserved. */

#include <xinu.h>

/**
 * Shell command (test) is testing hook.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_test(int nargs, char *args[])
{
    char input = *args[1];
         
    switch(input)
    {
        //create 1 file
        case '1':
            fileCreate("test1");
            //fileCreate("test1");
            break;
        //create 2 files
        case '2':
            fileCreate("test2");
            fileCreate("test3");
            break;
        //create 1 file and delete it
        case '3':
            kprintf("Creating 1 file...\r\n");
            int fd9 = fileCreate("test9");
            kprintf("Deleting 1 file...\r\n");
            fileDelete(fd9);
            kprintf("Done.\r\n");
            break;
        //create 2 files and delete them
        case '4':
            kprintf("Creating 2 files...\r\n");
            int fd10 = fileCreate("test10");
            int fd11 = fileCreate("test11");
            kprintf("Deleting 2 files...\r\n");
            fileDelete(fd10);
            fileDelete(fd11);
            kprintf("Done.\r\n");
            break;
        //create 5 files and delete them     
        case '5':
            kprintf("Creating 5 files...\r\n");
            int fd4 = fileCreate("test4");
            int fd5 = fileCreate("test5");
            int fd6 = fileCreate("test6");
            int fd7 = fileCreate("test7");
            int fd8 = fileCreate("test8");
            kprintf("Deleting 5 files...\r\n");
            fileDelete(fd4);
            fileDelete(fd5);
            fileDelete(fd6);
            fileDelete(fd7);
            fileDelete(fd8);
            kprintf("Done.\r\n");
            break;
        default:
            break;
    }

    return OK;
}
