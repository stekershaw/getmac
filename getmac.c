/* Output is a hex MAC address string with no separators */

#include <stdlib.h>		/* calloc, exit, free */
#include <string.h>		/* memset, memcpy, strlen, strncpy */
#include <stdio.h>		/* printf */

#include <net/if.h>		/* ifreq, AF_INET, SOCK_DGRAM */
#include <linux/sockios.h>	/* SIOCETHTOOL */
#include <linux/ethtool.h>	/* ETHTOOL_GPERMADDR */

#ifndef MAX_ADDR_LEN
#define MAX_ADDR_LEN 32
#endif

int main(int argc, char **argv) 
{
	int retval=0;
	int i;

	/* Take interface name as arg with 'eth0' default */
	/* TODO: make no assumptions, get potential interfaces from ioctl too */
	char *ifname;
	if( argv[1] )
		ifname = argv[1];
	else
		ifname = "eth0";

	/* Set up socket ioctl interface request struct */
	struct ifreq ifr;
	memset(&ifr,0,sizeof(ifr));
	strncpy(ifr.ifr_name, ifname, sizeof(ifr.ifr_name)-1);

	/* Create socket in Internet domain (default UDP), used to send the socket ioctl */
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(fd == -1) {
		perror("Cannot open socket");
		exit(1);
 	}

	/* Now the ethtool specific ioctl part - defining the command & data for the ioctl query */
	/* Using 'ethtool_perm_addr' from linux/ethtool.h
	   We need to allocate the buffer beyond data[0]
	   linux/ethtool.h recommends that MAX_ADDR_LEN should be sufficient for any address type
	*/ 
	struct ethtool_perm_addr *a;
	a = calloc(1, sizeof(*a) + MAX_ADDR_LEN);
	a->cmd = ETHTOOL_GPERMADDR;
	a->size = MAX_ADDR_LEN;

	/* Now the data of the query is in memory, link to the request struct */
	/* Populate the interface request struct with our request and send ioctl with SIOETHTOOL command */
	ifr.ifr_data = (caddr_t)a;
	if(ioctl(fd, SIOCETHTOOL, &ifr) != -1) {
		for (i = 0; i < a->size; i++) printf("%02x", a->data[i]);
		printf("\n");
	} else {
		perror("Cannot perform socket ioctl");
		retval=1;
	}

	/* Cleanup and exit */
	close(fd);
	free(a);
	exit(retval);
}
