/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT:MAILTO daniel.throop@marquette.edu brea.brennan@marquette.edu 
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>

extern void main(int, char *);

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d\r\n", currpid);

        user_yield();
    }
    return 0;
}

void testbigargs(int a, int b, int c, int d, int e, int f, int g, int h)
{
    kprintf("Testing bigargs...\r\n");
    kprintf("a = 0x%08X\r\n", a);
    kprintf("b = 0x%08X\r\n", b);
    kprintf("c = 0x%08X\r\n", c);
    kprintf("d = 0x%08X\r\n", d);
    kprintf("e = 0x%08X\r\n", e);
    kprintf("f = 0x%08X\r\n", f);
    kprintf("g = 0x%08X\r\n", g);
    kprintf("h = 0x%08X\r\n", h);
}

void printpcb(int pid)
{
    pcb *ppcb = NULL;

    /* Using the process ID, access it in the PCB table. */
    ppcb = &proctab[pid];

    /* Printing PCB */
    kprintf("Process name		  : %s \r\n", ppcb->name);

    switch (ppcb->state)
    {
    case PRFREE:
        kprintf("State of the process	  : FREE \r\n");
        break;
    case PRCURR:
        kprintf("State of the process 	  : CURRENT \r\n");
        break;
    case PRSUSP:
        kprintf("State of the process	  : SUSPENDED \r\n");
        break;
    case PRREADY:
        kprintf("State of the process	  : READY \r\n");
        break;
    default:
        kprintf("ERROR: Process state not correctly set!\r\n");
        break;
    }

    /* Print PCB contents and registers */
    kprintf("Base of run time stack    : 0x%08X \r\n", ppcb->stkbase);
    kprintf("Stack length of process   : %8u \r\n", ppcb->stklen);
}

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c;

    kprintf("0) Test user_none syscall\r\n");
    kprintf("1) Test user_getc syscall\r\n");
    kprintf("2) Test user_putc syscall\r\n");
    kprintf("3) Create three processes that test user_yield syscall\r\n");
    kprintf("P) Testing preemptive scheduling with timing inturrupts\r\n");

    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {
    case '0':
        // Test user_none
        kprintf("This is a test of ...");
        user_none();
        kprintf("user_none() syscall\r\n");
        break;

    case '1':
        kprintf("Enter another character through user_getc() syscall :");
        c = user_getc(0);
        kprintf("\r\nCharacter entered was \'%c\'\r\n", c);
        break;

    case '2':
        kprintf("Echo characters until 'X': ");
        while ((c = user_getc(0)) != 'X')
        {
            user_putc(0, c);
        }
        kprintf("\r\n");
        break;

    case '3':
        // Create three copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 5, 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN2", 5, 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, "MAIN3", 5, 2, 0, NULL),
              RESCHED_YES);
        while (numproc > 1)
            resched();
        break;

    case 'P':
	//demonstrate premptive scheduling
		//This means that it would interrupt what the job is currently doing after the quantum is over to pick a new winner
	ready(create((void *)testbigargs, INITSTK, "BigArgs", 15, 8,0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 
			0x55555555, 0x66666666, 0x77777777, 0x88888888),RESCHED_NO);

	ready(create((void *)testmain, INITSTK, "MAIN4", 5, 2, 0, NULL),  RESCHED_NO);
	
	while (numproc>1)
		resched();
	break;

    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
