/**
 * @file     xsh_fish.c
 * @provides xsh_fish
 *
 */
/* Embedded XINU, Copyright (C) 2013.  All rights reserved. */

#include <xinu.h>

/**
 * Local function for sending simply FiSh packets with empty payloads.
 * @param dst MAC address of destination
 * @param type FiSh protocal type (see fileshare.h)
 * @return OK for success, SYSERR otherwise.
 */
static int fishSend(uchar *dst, char fishtype)
{
	uchar packet[PKTSZ];
	uchar *ppkt = packet;
	int i = 0;

	// Zero out the packet buffer.
	bzero(packet, PKTSZ);

	for (i = 0; i < ETH_ADDR_LEN; i++)
	{
		*ppkt++ = dst[i];
	}

	// Source: Get my MAC address from the Ethernet device
	control(ETH0, ETH_CTRL_GET_MAC, (long)ppkt, 0);
	ppkt += ETH_ADDR_LEN;
		
	// Type: Special "3250" packet protocol.
	*ppkt++ = ETYPE_FISH >> 8;
	*ppkt++ = ETYPE_FISH & 0xFF;
		
	*ppkt++ = fishtype;
		
	for (i = 1; i < ETHER_MINPAYLOAD; i++)
	{
		*ppkt++ = i;
	}
		
	write(ETH0, packet, ppkt - packet);
	
	return OK;
}
		
/**
 * Shell command (fish) is file sharing client.
 * @param args array of arguments
 * @return OK for success, SYSERR for syntax error
 */
command xsh_fish(int nargs, char *args[])
{
	uchar bcast[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
	int i = 0;

	if (nargs == 2 && strncmp(args[1], "ping", 4) == 0)
    {
		fishSend(bcast, FISH_PING);
		
		sleep(1000);

		printf("Known FISH nodes in school:\n");
		for (i = 0; i < SCHOOLMAX; i++)
		{
			if (school[i].used)
			{
				printf("\t%02X:%02X:%02X:%02X:%02X:%02X",
					   school[i].mac[0],
					   school[i].mac[1],
					   school[i].mac[2],
					   school[i].mac[3],
					   school[i].mac[4],
					   school[i].mac[5]);
				printf("\t%s\n", school[i].name);
			}
		}
		printf("\n");
		return OK;
	}
	else if (nargs == 3 && strncmp(args[1], "list", 4) == 0)
	{
		// TODO: Locate named node in school,
		//   and send a FISH_DIRASK packet to it.
		//   Wait one second for reply to come in, and
		//   then print contents of fishlist table.
	
		uchar nameComp[FISH_MAXNAME];
		int schoolIndex = 0;
		nameComp = args[2];
		uchar _str[FMANLEN];
		int notFound = 1;
		

		//find the school that has the same name
		for(int i = 0; i <SCHOOLMAX; i++)
		{
			
			if((school[i].used) && (strncmp(school[i].name, nameComp, FISH_MAXNAME)))
			{
				schoolIndex = i;
				notFound = 0;	
			}
		}	

		if(notFound == 1)
		{
			printf("No FiSh \"%s\" found in school.\n", args[2]);
			return;
		}	
		else
		{
			//Locating the name of the node in the school
			bcast[0] = school[schoolIndex].mac[0];
			bcast[1] = school[schoolIndex].mac[1];
			bcast[2] = school[schoolIndex].mac[2];
			bcast[3] = school[schoolIndex].mac[3];
			bcast[4] = school[schoolIndex].mac[4];
			bcast[5] = school[schoolIndex].mac[5];

			//Sending a FISH_DIRASK to the named node
			fishSend(bcast, FISH_DIRASK);

			//Wait one second
			sleep(1000);
		
			//Print the contents of the fishlist table	
			for(int a = 0; a < DIRENTRIES; a++)
			{
				if(!(fishlist[a][0] == NULL))
				{
					strcpy(_str, fishlist[a], FMANLEN);
					fprintf("%s\n", _str);
				}
			}	

			return OK;
		}
	}
	else if (nargs == 4 && strncmp(args[1], "get", 4) == 0)
	{
		// 	 TODO: Locate named node in school,
		//   and send a FISH_GETFILE packet to it.
		//   FileSharer puts file in system when it arrives.
		
		return OK;
	}
	else
    {
        fprintf(stdout, "Usage: fish [ping | list <node> | get <node> <filename>]\n");
        fprintf(stdout, "Simple file sharing protocol.\n");
        fprintf(stdout, "ping - lists available FISH nodes in school.\n");
        fprintf(stdout, "list <node> - lists directory contents at node.\n");
        fprintf(stdout, "get <node> <file> - requests a remote file.\n");
        fprintf(stdout, "\t--help\t display this help and exit\n");

        return 0;
    }



    return OK;
}
