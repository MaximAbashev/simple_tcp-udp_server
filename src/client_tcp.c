# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <time.h>

# define SRV_PORT 5555
# define CLNT_PORT 4444

int main ()
{
	short port;
	int sock, new_sock;
	int from_len;
	char buf[30];
	struct sockaddr_in clnt_addr, s_addr, new_s_addr;
/*
 * Fabricate socket and set socket options.
 */
	sock = socket (AF_INET, SOCK_STREAM, 0);
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	s_addr.sin_port = htons(SRV_PORT);
/*
 * Send and receive message
 */
	printf("Connecting to server...\n");
	connect (sock,(struct sockaddr *)&s_addr, sizeof(s_addr));
	send (sock, "Hi! I'm TCP client!\n", 21, 0);
	from_len = recv (sock, buf, 22, 0);
	write(1, buf, from_len);
	close(sock);
	
	return 1;
}
