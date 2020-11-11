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
struct memblock *temp, *temp2, *temp3;

//Pound defines from the pThread demo code provided to us
#define MAX 10000
#define THRD_COUNT 4


typedef struct
{
  int *array;
  int length;
  long int *answer;
  pthread_mutex_t *lock;
} myarg_t;


/* pThread struct to pass into testbigarg function */
/**
typedef struct
{
  ulong parm1;
  ulong parm2;
  ulong parm3;
  ulong parm4;
  ulong parm5;
  ulong parm6;
  ulong parm7;
  ulong parm8;
  pthread_mutex_t *lock;
} myarg_t;
**/

/* Function for each thread to sum up its part of the array from the demo code provided */
void *mythread(void *arg)
{
  kprintf("We made it");
  /* Cast the generic args to be our myarg_t struct */
  myarg_t *args = (myarg_t *)arg;
  
  int i = 0;

  //kprintf("Adding %d to %d\n", args->array[0], args->array[args->length - 1]);
  for (i = 0; i < args->length; i++)
  {
  	//pthread_mutex_lock(args->lock);
        *(args->answer) += args->array[i];
        //pthread_mutex_unlock(args->lock);
        kprintf("Adding: %d\r\n", args->array[i]);
  }
  return NULL;
}

int demoTest()
{


  int *array = NULL;               /* Array of numbers to add up        */
  long int answer = 0;             /* Answers from each thread          */
  pthread_t threads[THRD_COUNT];   /* PThread objects                   */
  myarg_t args[THRD_COUNT];        /* Argument struct, one per thread   */
  int i = 0, rc = 0;
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

  array = (int *)malloc(MAX * sizeof(int));
  if (NULL == array) return -1;
  
  kprintf("Made it passed the malloc call\r\n");

  /* Initialize data from 1 to MAX  */
  for (i = 0; i < MAX; i++) { array[i] = i + 1; }

  /* Initialize Per-thread argument structs */
  for (i = 0; i < THRD_COUNT; i++)
    {
      /* Each thread gets its own portion of the array */
      args[i].array = array + i * (MAX / THRD_COUNT);
      args[i].length = MAX / THRD_COUNT;
  
      /* Each thread gets its own answer spot */
      args[i].answer = &answer;
      args[i].lock = &lock;
    }

  /* Create all of the worker threads */
  for (i = 0; i < THRD_COUNT; i++)
    {
      rc = pthread_create(threads + i, NULL, mythread, args + i);
    }

  /* Use join() to wait for each thread to complete its work */
  for (i = 0; i < THRD_COUNT; i++)
    {
      pthread_join(threads[i], NULL);
    }

  kprintf("Final answer is %d\n", answer);
  
  return 0;
}

int testmain(int argc, char **argv)
{
    int i = 0;
    kprintf("Hello XINU World!\r\n");

    for (i = 0; i < 3; i++)
    {
        kprintf("This is process %d\r\n", currpid[getcpuid()]);

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

void printFreeList()
{
   struct memblock *curr;

   curr = freelist.head;

   while(curr != NULL)
   {
	kprintf("Space remain on Freelist: %d\r\n", (curr->length));	//Length
	kprintf("Current address where Freelist begins: 0x%08X\r\n", (curr));	//current address

	//reset vars
	curr = curr->next;
   }

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
    kprintf("3) Create five processes that test user_yield syscall\r\n");
    kprintf("P) Testing preemptive scheduling with timing inturrupts\r\n");
    kprintf("4) Testing the Freelist printing function and getmem of 0x100 and 0x200 bytes\r\n");
    kprintf("5) Testing the getmem function with 0x1000, 0x2000 and 0x3000 bytes of requested allocated space\r\n");
    kprintf("6) Testing freemem on a getmem space of 0x100 bytes\r\n");
    kprintf("7) Testing free() and malloc() cases of test 3 in random roder\r\n");
    kprintf("8) Testing pthread create\r\n");
    kprintf("9) Testing pthread for two threads along with pJoin\r\n");
    //kprintf("10) Testing the pthread operation of join, create, lock and unlock using the demo code provided to us\r\n");

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
        // Create five copies copies of a process, and let them play.
        // This will ensure that each core has at least 1 process from the ready list besides their null process
        ready(create((void *)testmain, INITSTK, 5, "MAIN1", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 5, "MAIN2", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 5, "MAIN3", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 5, "MAIN4", 2, 0, NULL),
              RESCHED_NO);
        ready(create((void *)testmain, INITSTK, 5, "MAIN5", 2, 0, NULL),
              RESCHED_NO);
        while (numproc > 5)
		resched();
	break;

    case 'P':
	//demonstrate premptive scheduling
		//This means that it would interrupt what the job is currently doing after the quantum is over to pick a new winner
	ready(create((void *)testbigargs, INITSTK, 15, "BigArgs", 8,0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 
			0x55555555, 0x66666666, 0x77777777, 0x88888888),RESCHED_NO);

	ready(create((void *)testmain, INITSTK, 5, "MAIN6", 2, 0, NULL),  RESCHED_NO);
	
	while (numproc > 5)
		resched();
	break;

    case '4':
	//prints the free list before allocating 100 bytes using getmem
	kprintf("\r\n");
	kprintf("Free list before getmem call:\r\n");
	printFreeList();
	kprintf("\r\n");

        getmem(0x100);
	kprintf("Free list after getmem of 0x100 bytes:\r\n");
	printFreeList();
	kprintf("\r\n");

        getmem(0x200);
        kprintf("Free list after getmem of 0x200 more bytes:\r\n");
       printFreeList();
	break;

    case '5':
	//Allocating major amounts of space
	kprintf("\r\n");
	kprintf("Freelist before getmem call:\r\n");
	printFreeList();
	kprintf("\r\n");

	malloc(0x1000);
	kprintf("Freelist after getmem of 0x1000 bytes:\r\n");
	printFreeList();
	kprintf("\r\n");

	malloc(0x2000);
	kprintf("Freelist after getmem of 0x2000:\r\n");
	printFreeList();
	kprintf("\r\n");

	malloc(0x3000);
	kprintf("Freelist after getmem of 0x3000:\r\n");
	printFreeList();
	break;

    case '6':
	//Testing freemem on getmem of 0x1000 bytes
        kprintf("Free list before getmeming 256 bytes:\r\n");
        printFreeList();
	kprintf("Freelist after getmem of 0x100 bytes:\r\n");
        temp = getmem(0x100);
        printFreeList();
        freemem(temp, 0x100);
        kprintf("Freememing...\r\n");
        printFreeList();
	break;

    case '7':
	//testing free on malloc of test case 3 in random order
	kprintf("Free list before mallocing:\r\n");
        printFreeList();
        temp = malloc(0x100);
        kprintf("Free list after mallocing 256 bytes:\r\n");
        printFreeList();
        kprintf("Freeing...\r\n");
        free(temp);
        printFreeList();

        printFreeList();
        kprintf("Free list after mallocing 768 bytes:\r\n");
        temp2 = malloc(0x300);
        printFreeList();
        kprintf("Freeing...\r\n");
        free(temp2);
        printFreeList();

        printFreeList();
        kprintf("Free list after mallocing 2048 bytes:\r\n");
        temp3 = malloc(0x600);
        kprintf("Freeing...\r\n"); 
        printFreeList();
        free(temp3);
        printFreeList();

	break;
//testing pthread_create() with sum of 100 array elements
    case '8': 	
        ;
        
        int *array = NULL;               /* Array of numbers to add up        */
        long int answer = 0;             /* Answers from each thread          */
        pthread_t thread;                /* PThread objects                   */
        myarg_t args;                    /* Argument struct, one per thread   */
        pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
        int r = 0;
        int i = 0;

        array = (int *)malloc(100 * sizeof(int));

        kprintf("Made it passed the malloc call\r\n");

        /* Initialize data from 1 to MAX  */
        for (i = 0; i < 100; i++) { array[i] = i + 1; }

        /* Each thread gets its own portion of the array */
        args.array = array;
        args.length = 100;

        /* Each thread gets its own answer spot */
        args.answer = &answer;
        args.lock = &lock;

	//create the thread using pthread_create call
	r = pthread_create(&thread, NULL, mythread, &args);

        //pthread_join(thread, NULL);

	kprintf("Finished testcase 8, r = %d\r\n", r);

        kprintf("Answer: %d\r\n", *args.answer);

	break;

    case '9': //Testing pThread create for two threads and pJoin
	break;

    case 't': //Code taken from the demo provided to us
        demoTest();
	break;
    }

    kprintf("\r\n===TEST END===\r\n");
    return;
}
