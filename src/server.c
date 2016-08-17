# include <stdio.h>
# include <stdlib.h>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <time.h>

# define SRV_PORT 5555
# define CLNT_PORT 4444
# define GET_MAX(a, b) ((a) > (b) ? (a) : (b))

int main ()
{
	short port;
	const int on = 1;
	fd_set rset;
	int tcp_sock, udp_sock, from_len, max_sock, ready, contact, contact2, 
	len, len2;
	long int ttime;
	char buf[30];
	struct sockaddr_in s_addr, clnt_addr, new_s_addr;
/*
 * Initiate tcp-udp socket options.
 */
	s_addr.sin_family = AF_INET;
	s_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	s_addr.sin_port = htons(SRV_PORT);
/*
 * Initiate tcp-socket part.
 */
	tcp_sock = socket (AF_INET, SOCK_STREAM, 0);
	setsockopt(tcp_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof (on));
	if (bind (tcp_sock, (struct sockaddr *)&s_addr, sizeof (s_addr)) < 0)
	{
		perror ("TCP bind error!\n");
		exit (1);
	}
	listen (tcp_sock, 1);
/*
 * Initiate udp-socket part.
 */ 
	udp_sock = socket (AF_INET, SOCK_DGRAM, 0);
	if (bind (udp_sock, (struct sockaddr *)&s_addr, sizeof (s_addr)) < 0)
	{
		perror ("UDP bind error!\n");
		exit (1);
	}
/*
 * Waiting event on socket and service them
 */
	FD_ZERO (&rset);
	max_sock = GET_MAX (tcp_sock, udp_sock);
	while (1)
	{
		FD_SET (tcp_sock, &rset);
		FD_SET (udp_sock, &rset);
		if ((ready = select (max_sock + 1, &rset, NULL, NULL, 
		NULL)) < 0)
		{
			perror ("Select error!\n");
			exit (1);
		}
		if (FD_ISSET (tcp_sock, &rset))
		{
			len = sizeof (s_addr);
			contact = accept (tcp_sock, 
				(struct sockaddr *)&s_addr, &len);
			if(contact == (-1))
			{
				perror ("Connect error!\n");
				exit (1);
			}
			from_len = recv (contact, buf, 21, 0);
			write (1, buf, from_len);
			send (contact, "It's for TCP client!\n", 22, 0);
			close (contact);
		}
		if (FD_ISSET (udp_sock, &rset))
		{
			len = sizeof (s_addr);
			while (2)
			{
				from_len = recvfrom (udp_sock, buf, 21, 0,
					(struct sockaddr *)&s_addr, &len);
				if(from_len > 0)
				{
					write (1, buf, from_len);
					break;
				}
			}
			sendto(udp_sock, "It's for UDP client!\n", 22, 0,
					(struct sockaddr *)&s_addr,
					sizeof (s_addr));
		}
	}
	close (tcp_sock);
	close (udp_sock);
	return 1;
}
