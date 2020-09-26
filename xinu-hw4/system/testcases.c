/**
 * @file testcases.c
 * @provides testcases
 *
 * TA-BOT: daniel.throop@marquette.edu brea.brennan@marquette.edu
 *
 */
/* Embedded XINU, Copyright (C) 2007.  All rights reserved. */

#include <xinu.h>
#define NO_PADS 0
#define PADS 1

extern void main(int, char *);

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 10; i++)
    {
        kprintf("This is process %d\r\n", currpid);
	
	resched();
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

void printStack(int pid, int pads)
{
    int i;
    pcb *ppcb = NULL;
    ppcb = &proctab[pid];
 
    //printing stack with PADS   
    if(pads)
    {
    	for(i = 0; i < 20; i++)
    	{
	    kprintf("r%d:0x%X\r\n", i, ((ulong *)ppcb->stkptr)[i]);
    	}	
    }
    //printing stack without PADS
    else
    {
       	for(i = 0; i < 16; i++)
	{
		kprintf("r%d:0x%X\r\n", i, ((ulong *)ppcb->stkptr)[i]);
	}
    }
}
	 

/**
 * testcases - called after initialization completes to test things.
 */
void testcases(void)
{
    int c, pid;

    kprintf("0) Test creation of one process\r\n");
    kprintf("1) Test passing of many args\r\n");
    kprintf("2) Create three processes and run them\r\n");  
    kprintf("3) Create Process with NULL in r0 -> r3\r\n");
    kprintf("4) Create Process with NULL in r0 -> r3 + Padding\r\n"); 
    kprintf("5) Create Process with '2' in r0 and '4' in r1\r\n");
    kprintf("6) Create Process with '2' in r0, '4' in r1, '6' in r2, '8' in r3\r\n");
    kprintf("7) Create Process with '2' in r0, '4' in r1, '6' in r2, '8' in r3, '10' and '12' in pads\r\n");
    kprintf("8) Create Process with '2' in r0, '4' in r1, '6' in r2, '8' in r3, '10' and '12' in pads and call ready()\r\n");
    kprintf("9) Create six processes and run them\r\n");
    kprintf("===TEST BEGIN===\r\n");

    // TODO: Test your operating system!

    c = kgetc();
    switch (c)
    {

    case '0':
	//a few arguments
    	pid = create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	printStack(pid, NO_PADS);

        break;

    case '1':
        // Many arguments testcase
        pid = create((void *)testbigargs, INITSTK, "MAIN1", 8,
                     0x11111111, 0x22222222, 0x33333333, 0x44444444,
                     0x55555555, 0x66666666, 0x77777777, 0x88888888);
        printpcb(pid);
 	
	kprintf("Address of function in R15: 0x%X\r\n", &testbigargs);
		
	printStack(pid, PADS);
	
	ready(pid, RESCHED_YES);
	

        break;	

    case '2':
        // Create three copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL),
              RESCHED_NO);

        ready(create((void *)testmain, INITSTK, "MAIN2", 2, 0, NULL),
              RESCHED_NO);

        ready(create((void *)testmain, INITSTK, "MAIN3", 2, 0, NULL),
              RESCHED_YES);

        break;

    case '3':
	//4 NULL Arguments Test -> R0 - R3  
    	pid = create((void *)testmain, INITSTK, "MAIN1", 4, NULL, NULL, NULL, NULL);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	printStack(pid, NO_PADS);	

        break;

    case '4':
	//8 NULL Arguments Test -> R0 - R3 + Padding   
    	pid = create((void *)testmain, INITSTK, "MAIN1", 8, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	//Getting process control block + access to pointer
	int d;
	pcb *ppcb4 = NULL;
	ppcb4 = &proctab[pid];
	
	//printing out R0 - R15 (reverse order)
	for(d = 0; d < 20; d++)
	{
		kprintf("0x%X\r\n", ((ulong *)ppcb4->stkptr)[d]);
	}

        break;

    case '5':
	//Value of '2' in r0 and value of '4' in r1   
    	pid = create((void *)testmain, INITSTK, "MAIN1", 2, 2, 4);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	//Getting process control block + access to pointer
	int e;
	pcb *ppcb5 = NULL;
	ppcb5 = &proctab[pid];
	
	//printing out R0 - R15 (reverse order)
	for(e = 0; e < 16; e++)
	{
		kprintf("0x%X\r\n", ((ulong *)ppcb5->stkptr)[e]);
	}

        break;

    case '6':
	//Value of '2' in r0, value of '4' in r1, value of '6' in r2, value of '8' in r3   
    	pid = create((void *)testmain, INITSTK, "MAIN1", 4, 2, 4, 6, 8);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	//Getting process control block + access to pointer
	int f;
	pcb *ppcb6 = NULL;
	ppcb6 = &proctab[pid];
	
	//printing out R0 - R15 (reverse order)
	for(f = 0; f < 16; f++)
	{
		kprintf("0x%X\r\n", ((ulong *)ppcb6->stkptr)[f]);
	}

        break;

     case '7':
	//Value of '2' in r0, value of '4' in r1, value of '6' in r2, value of '8' in r3, 10 and 12 in pads   
    	pid = create((void *)testmain, INITSTK, "MAIN1", 6, 2, 4, 6, 8, 10, 12);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	//Getting process control block + access to pointer
	int g;
	pcb *ppcb7 = NULL;
	ppcb7 = &proctab[pid];
	
	//printing out R0 - R15 (reverse order)
	for(g = 0; g < 18; g++)
	{
		kprintf("0x%X\r\n", ((ulong *)ppcb7->stkptr)[g]);
	}

        break;

     case '8':
	//Value of '2' in r0, value of '4' in r1, value of '6' in r2, value of '8' in r3, 10 and 12 in pads and call ready()   
    	pid = create((void *)testmain, INITSTK, "MAIN1", 6, 2, 4, 6, 8, 10, 12);
        printpcb(pid);

	kprintf("Address of function in R15: 0x%X\r\n", &testmain);
	
	//Getting process control block + access to pointer
	int h;
	pcb *ppcb8 = NULL;
	ppcb8 = &proctab[pid];
	
	//printing out R0 - R15 (reverse order)
	for(h = 0; h < 18; h++)
	{
		kprintf("0x%X\r\n", ((ulong *)ppcb8->stkptr)[h]);
	}

	ready(pid, RESCHED_YES);

        break;

     case '9':
        // Create six copies of a process, and let them play.
        ready(create((void *)testmain, INITSTK, "MAIN1", 2, 0, NULL),
              RESCHED_NO);

        ready(create((void *)testmain, INITSTK, "MAIN2", 2, 0, NULL),
              RESCHED_NO);

        ready(create((void *)testmain, INITSTK, "MAIN3", 2, 0, NULL),
              RESCHED_NO);

	ready(create((void *)testmain, INITSTK, "MAIN4", 2, 0, NULL),
              RESCHED_NO);

	ready(create((void *)testmain, INITSTK, "MAIN5", 2, 0, NULL),
              RESCHED_NO);

	ready(create((void *)testmain, INITSTK, "MAIN6", 2, 0, NULL),
              RESCHED_YES);

	
        break;
 
    default:
        break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}        
