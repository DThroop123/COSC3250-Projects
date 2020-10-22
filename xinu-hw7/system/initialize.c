/**
 * @file initialize.c
 * The system begins intializing after the C environment has been
 * established.  After intialization, the null process remains always in
 * a ready or running state.
 */
/* Embedded Xinu, Copyright (C) 2009, 2020.  All rights reserved. */

#include <xinu.h>

/* Function prototypes */
static int sysinit(void);       /* initializes system structures         */
static int coreinit(int);       /* initializes per-core structures       */
static void welcome(void);      /* Print inital O/S data                 */
extern process main(void);      /* main is the first process created     */

/* Declarations of major kernel variables */
pcb proctab[NPROC];             /* Process table                         */
qid_typ readylist;              /* List of READY processes               */

/* Active system status */
volatile int numproc;           /* Number of live user processes         */
volatile int currpid[NCORES];   /* Id of currently running process       */

/* Params set by startup.S */
void *memheap;                  /* Bottom of heap (top of O/S stack)     */

struct platform platform;       /* Platform specific configuration       */

/*
 * Intializes the system and becomes the null process.
 * This is where the system begins after the C environment has been
 * established.  Interrupts are initially DISABLED, and must eventually
 * be enabled explicitly.  This routine turns itself into the null process
 * after initialization.  Because the null process must always remain ready
 * to run, it cannot execute code that might cause it to be suspended, wait
 * for a semaphore, or put to sleep, or exit.  In particular, it must not
 * do I/O unless it uses kprintf for synchronous output.
 */
void nulluser(void)
{
    int i, mycore;
    char pname[PNMLEN];

    mycore = getcpuid();
    if (0 == mycore)
    {
        /* Platform-specific initialization */
        platforminit();

        /* General initialization  */
        sysinit();

        /* Standard Embedded Xinu processor and memory info */
        welcome();

        /* Unpark the other cores */
        for (i = 1; i < NCORES; i++)
        {
            unparkcore(i, (void *)nulluser, NULL);
        }
    }
    else
    {
        clkupdate(platform.clkfreq / CLKTICKS_PER_SEC);
    }

    coreinit(mycore);

    /* Call the main program */
    sprintf(pname, "MAIN%d", mycore);
    ready(create((void *)main, INITSTK, INITPRIO, pname, 2, 0, NULL),
		    RESCHED_YES);

    enable();
    /* null process has nothing else to do but cannot exit  */
    while (1)
    {
        resched();
    }

}


static void welcome(void)
{
    kprintf(VERSION);
    kprintf("\r\n\r\n");

    /* Output detected platform. */
    kprintf("Detected platform as: [%s] %s, %s (rev %d)\r\n\r\n",
            platform.manufacturer, platform.family,
            platform.type, platform.revision);

    /* Output Xinu memory layout */
    kprintf("%10d bytes physical memory.\r\n",
            (ulong)platform.maxaddr - (ulong)platform.minaddr);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)platform.minaddr, (ulong)(platform.maxaddr - 1));

    extern void _start(void);
    kprintf("%10d bytes reserved system area.\r\n",
            (ulong)_start - (ulong)platform.minaddr);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)platform.minaddr, (ulong)_start - 1);

    kprintf("%10d bytes Xinu code.\r\n", (ulong)&_end - (ulong)_start);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)_start, (ulong)&_end - 1);

    kprintf("%10d bytes kernel stack space.\r\n",
            (ulong)memheap - (ulong)&_end);
    kprintf("           [0x%08X to 0x%08X]\r\n",
            (ulong)&_end, (ulong)memheap - 1);

    if (PERIPHERALS_BASE < (ulong)platform.maxaddr)
    {
        kprintf("%10d bytes heap space.\r\n",
                (ulong)PERIPHERALS_BASE - (ulong)memheap);
        kprintf("           [0x%08X to 0x%08X]\r\n",
                (ulong)memheap, (ulong)PERIPHERALS_BASE - 1);
        kprintf("%10d bytes memory-mapped peripheral space.\r\n",
                (ulong)platform.maxaddr - (ulong)PERIPHERALS_BASE);
        kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
                (ulong)PERIPHERALS_BASE, (ulong)platform.maxaddr - 1);
    }
    else
    {
        kprintf("%10d bytes heap space.\r\n",
                (ulong)platform.maxaddr - (ulong)memheap);
        kprintf("           [0x%08X to 0x%08X]\r\n\r\n",
                (ulong)memheap, (ulong)platform.maxaddr - 1);
    }
    kprintf("\r\n");
}

/**
 * Intializes all Xinu data structures and devices.
 * @return OK if everything is initialized successfully
 */
static int sysinit(void)
{
    int i = 0;

    /* Initialize system variables */
    /* One NULLPROC per core. */
    numproc = NCORES;

    /* Initialize process table */
    for (i = 0; i < NPROC; i++)
    {
        proctab[i].state = PRFREE;
    }

    for (i = 0; i < NCORES; i++)
    {
	/* Initialize stack pointers for all cores */
        // core_init_sp[i] = (((uint)&_end) + NULLSTK * (i + 1));
	// Why doesn't this work here?  Moved back to start.S.
    }

    /* Initialize ready lists for all cores */
    readylist = newqueue();
    

    disable_irq(IRQ_USB);
    disable_irq(IRQ_PCM);
    disable_irq(IRQ_PL011);
    disable_irq(IRQ_SD);
    clkinit();

    seed_random(SEED);

    dmb();

    return OK;
}

/**
 * Initializes all per-core Xinu data structures and devices.
 * @return OK if everything is initialized successfully
 */
static int coreinit(int core)
{
    pcb *ppcb = NULL;           /* process control block pointer */
    char pname[PNMLEN];

    /* Initialize null process entry */
    ppcb = &proctab[NULLPROC + core];
    ppcb->state = PRCURR;
    sprintf(pname, "NULL%d", core);
    strncpy(ppcb->name, pname, PNMLEN);
    ppcb->stkbase = (void *)core_init_sp[core];
    ppcb->stklen = NULLSTK;
    currpid[core] = NULLPROC + core;
    ppcb->tickets = 1;

    return OK;
}
