#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <sys/time.h>
#include <inttypes.h>

#include "err.h"

int main(int argc, char *argv[])
{
  
  int sock;
  
  struct addrinfo addr_hints;
  struct addrinfo *addr_result;
  
  int err;
  char *option;
  char *tcp = "-t";
  char *udp = "-u";
  
  struct timeval time;
  
  int flags, sflags;
  
  size_t len;
  ssize_t snd_len, rcv_len;
  struct sockaddr_in my_address;
  struct sockaddr_in srvr_address;
  socklen_t rcva_len;
  
  long long time_start, time_end;
  
  if (argc != 4){
    fatal("Need 4 parameters.");
  }

  option = argv[1];
  int option_len = strlen(option);
  
  if (option_len != 2){
    fatal("2nd argument might be -t or -u"); 
  }
  
  int use_tcp = strncmp(option, tcp, 2);
  int use_udp = strncmp(option, udp, 2);
  
  if (use_tcp == 0){ //found -t in option
    
    // 'converting' host/port in string to struct addrinfo
    memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_STREAM;
    addr_hints.ai_protocol = IPPROTO_TCP;
    err = getaddrinfo(argv[2], argv[3], &addr_hints, &addr_result);
    if (err != 0)
      syserr("getaddrinfo: %s\n", gai_strerror(err));
    
    // initialize socket according to getaddrinfo results
    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);
    if (sock < 0)
      syserr("socket");

    gettimeofday(&time, NULL);

    time_start = 1000000 * time.tv_sec + time.tv_usec;
    
    // connect socket to the server
    if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0)
      syserr("connect");
    
    gettimeofday(&time, NULL);

    time_end = 1000000 * time.tv_sec + time.tv_usec;
    
    printf("time needed in tcp: %lld microseconds.\n", time_end-time_start);
    
    freeaddrinfo(addr_result);  
(void) close(sock);    
  } 
  else if (use_udp == 0){ //found -u in option
  
    struct pair package;
    uint64_t big_endian;
    
    // 'converting' host/port in string to struct addrinfo
    (void) memset(&addr_hints, 0, sizeof(struct addrinfo));
    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = 0;
    addr_hints.ai_addrlen = 0;
    addr_hints.ai_addr = NULL;
    addr_hints.ai_canonname = NULL;
    addr_hints.ai_next = NULL;
    if (getaddrinfo(argv[2], NULL, &addr_hints, &addr_result) != 0) {
      syserr("getaddrinfo");
    }

    my_address.sin_family = AF_INET; // IPv4
    my_address.sin_addr.s_addr =
	((struct sockaddr_in*) (addr_result->ai_addr))->sin_addr.s_addr; // address IP
    my_address.sin_port = htons((uint16_t) atoi(argv[3])); // port from the command line

    freeaddrinfo(addr_result);

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
      syserr("socket");

    gettimeofday(&time, NULL);

    time_start = 1000000 * time.tv_sec + time.tv_usec;;
    
    big_endian = htobe64(time_start);
    package.begin = big_endian;

    len = sizeof(struct pair);
    
    sflags = 0;
    rcva_len = (socklen_t) sizeof(my_address);
    snd_len = sendto(sock, &package, len, sflags,
	(struct sockaddr *) &my_address, rcva_len);
    if (snd_len != (ssize_t) len) {
      syserr("partial / failed write");
    }

    flags = 0;

    rcva_len = (socklen_t) sizeof(srvr_address);
    rcv_len = recvfrom(sock, &package, len, flags,
	(struct sockaddr *) &srvr_address, &rcva_len);

    if (rcv_len < 0) {
      syserr("read");
    }
    
    (void) printf("response time for udp package: %"PRIu64" microseconds.\n", be64toh(package.end) - be64toh(package.begin));
    fprintf(stderr, "udp send: %"PRIu64"\n udp received: %"PRIu64"\n", be64toh(package.begin),be64toh(package.end)); 
  if (close(sock) == -1){ 
      syserr("close");
  }
    
  } 
  else    fatal("wrong 2nd argument."); 
  return 0;
}