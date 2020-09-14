/**
 * @file testcases.c
 * @provides testcases
 *
 * $Id: testcases.c 175 2008-01-30 01:18:27Z brylow $
 *
 * Modified by:
 *
 * and
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

devcall putc(int dev, char c)
{
    return 0;
}


/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    uchar c = '0';

   // kputc('h');

    kprintf("\n===TEST BEGIN===\r\n");
    
    //Test 1: unget a character and check to see if its avalible in the buffer array
    kungetc('X');
    if(kcheckc())
    {
	kprintf("Character available\n");
    }
    else
    {
	kprintf("Didn't work\n");
    }

    //Test 2: Get a character
    c = kgetc();
    switch (c)
    {
	case 'X':
		kprintf("The character was retrieved from the unbuffer array.\n");
		break;

        // TODO: Test your operating system!

    default:
        kprintf("Hello Xinu World!\r\n");
    }

    //Test 3: putting a statement into the UART and/or Buffer array, then getting it and printing the statment to the screen 
    kputc('B');
    c = kgetc(); 
    kprintf("C = %c\n", c);

    kprintf("\r\n===TEST END===\r\n");
    return;
}
