#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include "server.h"

void startchat()
{
	int sockfd;
	struct sockaddr_in claddr;
	struct sockaddr_in svaddr;
	
	socklen_t addrlen = sizeof(struct sockaddr);
	
	memset(&claddr, 0, addrlen);
	memset(&svaddr, 0, addrlen);

	claddr.sin_family = AF_INET;
	claddr.sin_addr = (struct in_addr) {.s_addr = INADDR_ANY};
	
	svaddr.sin_family = AF_INET;
	svaddr.sin_port = htons(SV_PORT);
	inet_pton(AF_INET, SV_ADDR, &svaddr.sin_addr);
	
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	
	if (sockfd == -1)
	{
		fprintf(stderr, "could not create socket\n");
		exit(EXIT_FAILURE);
	}
	else
		puts("socket successfully created");
	
	printf("client ready to send data, type something:\n");
	
	ssize_t len;
	char buff[EXCLEN];
	memset(buff, 0, EXCLEN);

	while (1)
	{
		memset(buff, 0, EXCLEN);
		char ch;
		
		while ((ch = (char) fgetc(stdin)) != '\n')
			strncat(buff, &ch, 1);

		addrlen = sizeof(struct sockaddr_in);
		
		len = sendto(sockfd, buff, EXCLEN, 0, (struct sockaddr*) &svaddr, addrlen);
		
		if (len != EXCLEN)
		{
			fprintf(stderr, "error while sending data to server\n");
			exit(EXIT_FAILURE);
		}
		else
			printf("message sent, waiting reply...\n");
		
		memset(buff, 0, EXCLEN);
		
		len = recvfrom(sockfd, buff, EXCLEN, 0, (struct sockaddr*) &svaddr, &addrlen);
		
		if (len != EXCLEN)
		{
			fprintf(stderr, "error while receiving data from server\n");
			exit(EXIT_FAILURE);
		}
		else
		{
			// server ip
			char svip[INET_ADDRSTRLEN];
			memset(svip, 0, INET_ADDRSTRLEN);
			
			addrlen = sizeof(struct sockaddr_in);
			inet_ntop(AF_INET, &svaddr.sin_addr, svip, INET_ADDRSTRLEN);
			
			printf("%s said: %s\n", svip, buff);
		}
	}
}