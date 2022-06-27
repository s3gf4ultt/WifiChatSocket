#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

extern void startchat();
void usage(char*);

int main(int argc, char **args)
{
	if (argc < 3)
		usage(args[0]);
	else if (strcmp(args[1], "-m") != 0) usage(args[0]);
	
	char mode = args[2][0];
	
	if (mode == 's')
	{
		if (svinit() == 1)
		{
			printf("server listening...\n");
			svlisten();
		}
		else
			fprintf(stderr, "couldn't start server on %s\n", SV_ADDR);
	}
	else if (mode == 'c')
		startchat();

	return 0;
}

void usage(char *name)
{
	fprintf(stderr, "%s options\noptions:\n\t-m\tsets the mode (s to start the server or c to start the client)\n", name);
	exit(EXIT_FAILURE);
}