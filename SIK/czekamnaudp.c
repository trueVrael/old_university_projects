#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "err.h"

#define BUFFER_SIZE   1000

int main(int argc, char *argv[]) {
	int sock;
	int flags, sflags;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;
	struct timeval time;
	struct pair package;
    	uint64_t big_endian;
	socklen_t snda_len, rcva_len;
	ssize_t len, snd_len;
	long long time_end;
	sock = socket(AF_INET, SOCK_DGRAM, 0); // creating IPv4 UDP socket
	if (sock < 0)
		syserr("socket");
	// after socket() call; we should close(sock) on any execution path;
	// since all execution paths exit immediately, sock would be closed when program terminates

	server_address.sin_family = AF_INET; // IPv4
	server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
	server_address.sin_port = htons(argv[1]); // default port for receiving is PORT_NUM

	// bind the socket to a concrete address
	if (bind(sock, (struct sockaddr *) &server_address,
			(socklen_t) sizeof(server_address)) < 0)
		syserr("bind");

	snda_len = (socklen_t) sizeof(client_address);
	for (;;) {
		do {
			rcva_len = (socklen_t) sizeof(client_address);
			flags = 0; // we do not request anything special
			len = sizeof(struct pair);
			if (len < 0)
				syserr("error on datagram from client socket");
			else {
			/*	(void) printf("read from socket: %zd bytes: %.*s\n", len,
						(int) len, buffer);*/
				sflags = 0;
				gettimeofday(&time, NULL);
    				time_end = 10000000 * time.tv_sec + time.tv_usec;;
    
    				big_endian = htobe64(time_end);
    				package.end = big_endian;

				snd_len = sendto(sock, &package, (size_t) len, sflags,
						(struct sockaddr *) &client_address, snda_len);
				if (snd_len != len)
					syserr("error on sending datagram to client socket");
			}
		} while (len > 0);
		(void) printf("finished exchange\n");
	}

	return 0;
}

