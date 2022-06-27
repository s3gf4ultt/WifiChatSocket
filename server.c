#include <stdio.h>
#include <string.h>
#include "server.h"

// internal linkage for this variables
static struct sockaddr_in svaddr;
static int sockfd;
static socklen_t addrlen = sizeof(struct sockaddr_in);

// client address
static struct sockaddr_in claddr;

// setup variables and bind the socket
int svinit()
{
	// ensure all bits are set to zero
	memset(&svaddr, 0, addrlen);
	memset(&claddr, 0, addrlen);
	
	svaddr.sin_family = AF_INET;

	if (!inet_pton(AF_INET, SV_ADDR, &svaddr.sin_addr))
	{
		fprintf(stderr, "Invalid server address\n");
		return -1;
	}
	
	svaddr.sin_port = htons(SV_PORT);
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (bind(sockfd, (struct sockaddr*) &svaddr, addrlen) == -1)
		return -1;
	return 1;
}

int svlisten()
{
	char buff[EXCLEN];
	memset(buff, 0, EXCLEN);
	ssize_t len;

	while (1)
	{
		addrlen = sizeof(struct sockaddr_in);

		len = recvfrom(sockfd, buff, EXCLEN, 0, (struct sockaddr*) &claddr, &addrlen);

		if (len != EXCLEN)
		{
			fprintf(stderr, "error while receiving data from client\n");

			return -1;
		}
		else
		{
			char clientip[INET_ADDRSTRLEN];
			memset(clientip, 0, INET_ADDRSTRLEN);

			if (!inet_ntop(AF_INET, &claddr.sin_addr, clientip, INET_ADDRSTRLEN))
				fprintf(stderr, "couldn't convert client binary IP address to a string\n");

			if (strlen(clientip) < 1)
				strncpy(clientip, "unknown", INET_ADDRSTRLEN);

			printf("%s said: %s\ntype a response: \n", clientip, buff);

			char response[EXCLEN];
			memset(response, 0, EXCLEN);

			char ch;

			while ((ch = (char) fgetc(stdin)) != '\n')
				strncat(response, &ch, 1);

			len = sendto(sockfd, response, EXCLEN, 0, (struct sockaddr*) &claddr, addrlen);

			if (len != EXCLEN)
			{
				fprintf(stderr, "couldn't send response to client\n");
				return -1;
			}
			else
				printf("response sent, waiting reply...\n");
			
			memset(buff, 0, EXCLEN);
		}
	}
	
	return 1;
}