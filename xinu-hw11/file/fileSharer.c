/* fileSharer.c - fileSharer */
/* Copyright (C) 2007, Marquette University.  All rights reserved. */
/* TA-BOT:MAILTO daniel.throop@marquette.edu brea.brennan@marquette.edu leonardo.jimenez@marquette.edu */

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
	    	strncpy(&eg->data[(i * FNAMLEN) + 1], filetab[i].fn_name, FNAMLEN);	
	    }
	    else
	    {
	    	bzero(&eg->data[(i * FNAMLEN) + 1], FNAMLEN);
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

	//printf("we make it into fishDirList before exception");
	
	for(int i = 0; i < DIRENTRIES; i++)
	{ 
		strncpy(&fishlist[i][0], &(eg->data[(i * FNAMLEN) + 1]), FNAMLEN);
		fishlist[i][FNAMLEN - 1] = '\0'; //add the null character that was striped off the end in dirask
	}


	return OK;
}

/*------------------------------------------------------------------------
 * fishGetFile - Reply to a broadcast FISH request.
 *------------------------------------------------------------------------
 */

 // how do we actually insert the file name into the payload? (?)
 // how do we know if it exists or not? (?) --> search through the filetab?
 // does fishGetFile() decide on FISH_HAVEFILE or FISH_NOFILE? (?)

int fishGetFile(uchar *packet)
{
	char *ppkt = packet;
	struct ethergram *eg = (struct ethergram *)packet;
	char *fileName;
	struct filenode *file;
	int found = 0;

	/* FISH type becomes one of these */
	
	//search the filetab of the receiving machine
	
	//access the fileName
	fileName = &(eg->data[1]);
	printf("File Name: %s\n", fileName);

	//search for file

	for(int i = 0; i < DIRENTRIES; i++)
	{
		printf("%s\n", filetab->fn_name);

		if(strncmp(fileName, filetab->fn_name, FNAMLEN) == 0)
		{
			printf("Yes we are equal.\n");
			file = &filetab;
			found = 1;
			break;		
		}
		else
		{
			filetab++;
		}
		
	}

	/* Source of request becomes destination of reply. */
	memcpy(eg->dst, eg->src, ETH_ADDR_LEN);
	/* Source of reply becomes me. */
	memcpy(eg->src, myMAC, ETH_ADDR_LEN);
	/* Zero out payload. */
	//bzero(eg->data, ETHER_MINPAYLOAD);

	if(found)
	{

		printf("Yes we are found\n");

		eg->data[0] = FISH_HAVEFILE;
		
		//content look for the proper constant to replace the block size of 256 in header file
		for(int i = 0; i < 256; i++)
		{
			eg->data[(1+FNAMLEN)+ i] = file->fn_data[i];
		}

		//size needs to be changed for this to match
		//	EHTER_SIZE + FNAMLEN + CONENT MAX(256) + fishtype
		write(ETH0, packet, ETHER_SIZE + FNAMLEN + FISH_HAVEFILE + 256);
	}
	else
	{
		eg->data[0] = FISH_NOFILE;
		bzero(&(eg->data[1]), ETHER_MINPAYLOAD);
	
		//size needs to change to match
		//	ETHER_SIZE + FNMALEN + fishtype
		write(ETH0, packet, ETHER_SIZE + FNAMLEN + FISH_NOFILE);
	}	

	//write back to the ethernet packet
}

/*------------------------------------------------------------------------
 * fishHaveFile - Reply to a broadcast FISH request.
 *------------------------------------------------------------------------
 */

 //when extracting the payload do we just use index 0? (?)
 //do we use DISKBLOCKLEN for the length of the contents? (?)

int fishHaveFile(uchar *packet)
{
	struct ethergram *eg = (struct ethergram *)packet;
	uchar fileName[FNAMLEN];
	uchar contents[DISKBLOCKLEN];
	int fd = 0;

	//extract payload to get file name
	strncpy(&fileName, &eg->data[1], FNAMLEN);

	//extract payload to get file contents
	strncpy(&contents, &eg->data[FNAMLEN + 1], DISKBLOCKLEN);

	//write file to system

	//obtain file descriptor
	fd = fileCreate(&fileName);

	//write contents
	for(int i = 0; i < DISKBLOCKLEN; i++)
	{
		filePutChar(fd, contents[i]);
	}

	return OK;
}

/*------------------------------------------------------------------------
 * fishNoFile() - Reply to a broadcast FISH request.
 *------------------------------------------------------------------------
 */

int fishNoFile(uchar *packet)
{
	//this is not really right 
	printf("The file was not found.\n");

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
				fishGetFile(packet);
				break;
			case FISH_HAVEFILE:
				fishHaveFile(packet);
				break;
			case FISH_NOFILE:
				fishNoFile(packet);
				break;
			default:
				printf("ERROR: Got unhandled FISH type %d\n", eg->data[0]);
			}
		}
	}

	return OK;
}
