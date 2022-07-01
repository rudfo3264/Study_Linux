#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char *argv)
{
	int sock;
	pid_t pid;
	sock = socket(AF_INET, SOCK_STREAM, 0); 
	pid = fork();

	if(pid == 0)
		printf("%d\n", sock);
	else
		printf("%d\n",sock);
	return 0;
}
