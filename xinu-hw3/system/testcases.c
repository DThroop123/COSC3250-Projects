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
    
    //This test is to ensure that the kgetc() wait until there is a character in the flag register. This character was from the command line and will be used to run the switch statements of the other test cases
    c = kgetc();
    switch (c)
    {
	//This test case should unget the character 'X' and then print it to the screen using after getting the character via kgetc() function
	case '1':
		kprintf("Test Case 1\r\n");
		kungetc('X');
		c = kgetc();
		kprintf("Character = %c\r\n", c);		
		break;
	//This test case should check to see if there is a currently avalaible character to retrieve either in the UART register or in the ungetArray. Since there is no character to retrieve, kcheckc() should evaluate to '0'.
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
	//This test case uses the kgetc() function to receive a character without using the kungetc() function prior
	case '4':
		kprintf("Test Case 4\r\n");
		kprintf("Please enter a character:\r\n");
		c = kgetc(); //wait for input via the command line
		kprintf("The character entered was : %c\r\n", c);
		break;
	//Testing to see if the kcheckc() is able to detect a character that has been ungotten
	case '5':
		kprintf("Test Case 5\r\n");
		kungetc('B');
		kprintf("Ungetting character...\r\n");
		if(kcheckc()){
			kprintf("Character was detected in the ungetArray[] by kcheckc()\r\n");
		}
		else{
			kprintf("Character was NOT detected in the ungetArray[] by kcheckc()\r\n");
		}
		break;
	//Testing to see if the print statment of to many characters appears after the bufp index surpasses the UNGETMAX global variable
	case '6':
		kprintf("Test Case 6\r\n");
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');
		kungetc('T');//This call should cause the print statement in the kungetc() to be called
		break;
	//Testing the conjunction of functions together
	case '7':
		kprintf("Test Case 7\r\n");
		kprintf("Putting a character to the screen\r\n");
		kputc('D');
		kprintf("\r\n");
		kprintf("Getting the character put in\r\n");
		kprintf("Enter a character: \r\n");
		c = kgetc();
		kprintf("If 1, there is a character avaliable: %d\r\n", kcheckc());
		kprintf("Enter a character: \r\n");
		c = kgetc();
		kprintf("The character is: %c\r\n", c);
		kungetc(c);
		kprintf("Have ungotten the character from previous call\r\n");
		c = kgetc();
		kprintf("Re-gotten character is: %c\r\n", c);
		break;
		
        // TODO: Test your operating system!

    default:
        kprintf("Character = %c\r\n", c);
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
