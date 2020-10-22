/**
 * @file     main.c
 */
/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <xinu.h>

void testcases(void);

/**
 * Main process.  You can modify this routine to customize what Embedded Xinu
 * does when it starts up.  The default is designed to do something reasonable
 * on all platforms based on the devices and features configured.
 */
process main(void)
{
    int core = getcpuid();
    kprintf("Hello Xinu World from core %d!\r\n", core);

    if (0 == core)
    {
        testcases();
    }

    return 0;
}
