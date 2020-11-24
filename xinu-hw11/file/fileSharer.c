/* fileSharer.c - fileSharer */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */

#include <kernel.h>
#include <xc.h>
#include <file.h>
#include <fileshare.h>
#include <ether.h>
#include <network.h>
#include <nvram.h>

struct fishnode school[SCHOOLMAX];
char   fishlist[DIRENTRIES][FNAMLEN];

static uchar bcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static uchar myMAC[ETH_ADDR_LEN];

int fishAnnounce(uchar *packet)
{
	struct ethergram *eg = (struct ethergram *)packet;
	int i = 0;

	for (i = 0; i < SCHOOLMAX; i++)
	{
		/* Check to see if node already known. */
		if (school[i].used && 
			(0 == memcmp(school[i].mac, eg->src, ETH_ADDR_LEN)))
			return OK;
	}
	for (i = 0; i < SCHOOLMAX; i++)
	{
		/* Else find an unused slot and store it. */
		if (!school[i].used)
		{
			school[i].used = 1;
			memcpy(school[i].mac, eg->src, ETH_ADDR_LEN);
			memcpy(school[i].name, eg->data + 1, FISH_MAXNAME);
			//printf("Node %s\n", school[i].name);
			return OK;
		}
	}
	return SYSERR;
}

/*------------------------------------------------------------------------
 * fishPing - Reply to a broadcast FISH request.
 *------------------------------------------------------------------------
 */
void fishPing(uchar *packet)
{
	uchar *ppkt = packet;
	struct ethergram *eg = (struct ethergram *)packet;

	/* Source of request becomes destination of reply. */
	memcpy(eg->dst, eg->src, ETH_ADDR_LEN);
	/* Source of reply becomes me. */
	memcpy(eg->src, myMAC, ETH_ADDR_LEN);
	/* Zero out payload. */
	bzero(eg->data, ETHER_MINPAYLOAD);
	/* FISH type becomes ANNOUNCE. */
	eg->data[0] = FISH_ANNOUNCE;
	strncpy(&eg->data[1], nvramGet("hostname\0"), FISH_MAXNAME-1);
	write(ETH0, packet, ETHER_SIZE + ETHER_MINPAYLOAD);
}

/*------------------------------------------------------------------------
 * fishDirAsk - Reply to a broadcast FISH request.
 *------------------------------------------------------------------------
 */
void fishDirAsk(uchar *packet)
{
	uchar *ppkt = packet;
	struct ethergram *eg = (struct ethergram *)packet;

	/* Source of request becomes destination of reply. */
	memcpy(eg->dst, eg->src, ETH_ADDR_LEN);
	/* Source of reply becomes me. */
	memcpy(eg->src, myMAC, ETH_ADDR_LEN);
	/* Zero out payload. */
	bzero(eg->data, ETHER_MINPAYLOAD);
	/* FISH type becomes DIRLIST. */
	eg->data[0] = FISH_DIRLIST;

	/* Copying file names into packet */
	for(int i = 0; i < DIRENTRIES; i++)
	{
	    //splitting up -> check state, iterate and insert each character, if file state is not in use set to zero
	    if(!(filetab[i].fn_state == FILE_FREE))
	    {
	    	strncpy(&eg->data[i], filetab[i].fn_name, FNAMLEN);	
	    }

	    else
	    {
	    	bzero(&eg->data[i], FNAMLEN);
	    }
	}
 
	write(ETH0, packet, ETHER_SIZE + (DIRENTRIES * FNAMLEN));

}

/*------------------------------------------------------------------------
 * fishDirList - Reply to a broadcast FISH request.
 *------------------------------------------------------------------------
 */
int fishDirList(uchar *packet)
{
	struct ethergram *eg = (struct ethergram *)packet;

	/* copy the conents of the packet into the fish list */ 
	
	for(int i = 0; i < DIRENTRIES; i++)
	{ 
		strncpy(&fishlist[i][0], eg->data[i * FNAMLEN]);
		firstlist[i][FNAMLEN] = '\0'; //add the null character that was striped off the end in dirask
	}

	return OK;
}




/*------------------------------------------------------------------------
 * fileSharer - Process that shares files over the network.
 *------------------------------------------------------------------------
 */
int fileSharer(int dev)
{
	uchar packet[PKTSZ];
	int size;
	struct ethergram *eg = (struct ethergram *)packet;

	enable();
	/* Zero out the packet buffer. */
	bzero(packet, PKTSZ);
	bzero(school, sizeof(school));
	bzero(fishlist, sizeof(fishlist));

	/* Lookup canonical MAC in NVRAM, and store in ether struct */
 	colon2mac(nvramGet("et0macaddr"), myMAC);

	while (SYSERR != (size = read(dev, packet, PKTSZ)))
	{
		/* Check packet to see if fileshare type with
		   destination broadcast or me. */
		if ((ntohs(eg->type) == ETYPE_FISH)
			&& ((0 == memcmp(eg->dst, bcast, ETH_ADDR_LEN))
				|| (0 == memcmp(eg->dst, myMAC, ETH_ADDR_LEN))))
		{
			switch (eg->data[0])
			{
			case FISH_ANNOUNCE:
				fishAnnounce(packet);
				break;
			case FISH_PING:
				fishPing(packet);
				break;
		// TODO: All of the cases below.
			case FISH_DIRASK:
				fishDirAsk(packet);
				break;
			case FISH_DIRLIST:
				fishDirList(packet);
				break;
			case FISH_GETFILE:
			case FISH_HAVEFILE:
			case FISH_NOFILE:
			default:
				printf("ERROR: Got unhandled FISH type %d\n", eg->data[0]);
			}
		}
	}

	return OK;
}
