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

    kprintf("\n===TEST BEGIN===\r\n");

    c = kgetc();
    switch (c)
    {
	//This test case should unget the character 'X' and then print it to the screen using after getting the character via kgetc
	case '1':
		kprintf("Test Case 1\r\n");
		kungetc('X');
		c = kgetc();
		kprintf("Character = %c\r\n", c);		
		break;
	//This test case should check to see if there is a currently avalaible character to retrieve either in the UART register or in the ungetArray
	case '2':
		kprintf("Test Case 2\r\n");
		if(kcheckc())
		{
			kprintf("Character avaliable\r\n");
		}
		else
		{
			kprintf("No character avalaible to check.\r\n");
		}
		break;
	//This test case should put the character 'h' onto the terminal window
	case '3':
		kprintf("Test Case 3\r\n");
		kputc('h');
		break;
	//
	case '4':
		kprintf("Test Case 4\r\n");
		c = kgetc(); //wait for input via the command line
		kprintf("The character entered was : %c\r\n", c);
	//Testing more than one character input via command line
	case '5':
		kprintf("Test Case 5\r\n");
		kungetc('B');
		if(kcheckc()){
			kprintf("Working\r\n");
		}
		else{
			kprintf("Not working\r\n");
		}
		break;
		
        // TODO: Test your operating system!

    default:
        kprintf("Character = %c\r\n", c);
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
