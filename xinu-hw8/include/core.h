/*	2017 Embedded Xinu Team
 *	core.h
 *
 *	Contains Core Start addresses, semaphores, and CPUID function.
*/
#ifndef _CORE_H_
#define _CORE_H_

extern unsigned int getmode(void);
extern unsigned int getcpuid(void);
extern unsigned int core_init_sp[];
extern void *corestart[];
extern void unparkcore(int, void *, void *);
#endif                          /* _CORE_H_ */
