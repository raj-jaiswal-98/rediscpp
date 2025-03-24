#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include<iostream>

using namespace std;

// struct in_addr{
//   uint32_t    s_addr; // IPv4 in big-endian
// };
// struct sockaddr_in{
//   uint16_t    sin_family; // AF-NET
//   uint16_t    sin_port; // port in big-endian 
//   in_addr     sin_addr; // IPv4
// };

// struct in6_addr {
//   uint8_t       s6_addr[16];   // IPv6
// };

// struct sockaddr_in6 {
//   uint16_t        sin6_family;   // AF_INET6
//   uint16_t        sin6_port;     // port in big-endian
//   uint32_t        sin6_flowinfo; // ignore
//   in6_addr        sin6_addr;     // IPv6
//   uint32_t        sin6_scope_id; // ignore
// };

static void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}


int main(){
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd < 0){
        die("socket()");
    }

    sockaddr_in addr = {};

    addr.sin_family = AF_INET;
    addr.sin_port = ntohs(1234);
    addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK);  // 127.0.0.1

    //connect to above ip and port!

    int rv = connect(fd, (sockaddr*)&addr, sizeof(addr));
    if(rv){
        die("connect()");
    }

    char msg[] = "Hiii!!";
    write(fd, msg, strlen(msg));
    
    
    //read from server
    
    char rbuf[64] = {};

    ssize_t n = read(fd, rbuf, sizeof(rbuf)-1);
    if(n < 0){
        die("read()");
    }
    printf("server says: %s\n", rbuf);
    close(fd);
}