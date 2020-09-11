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
    uchar c;

    kputc('h');

    kprintf("\n===TEST BEGIN===\r\n");

    c = kgetc();
    kputc(c);
    

    switch (c)
    {
	case 'A':
		kputc(c);
		break;

        // TODO: Test your operating system!

    default:
        kprintf("Hello Xinu World!\r\n");
    }


    kprintf("\r\n===TEST END===\r\n");
    return;
}
