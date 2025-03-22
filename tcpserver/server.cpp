#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>


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

static void do_something(int connfd) {
  char rbuf[64] = {};
  ssize_t n = read(connfd, rbuf, sizeof(rbuf) - 1);
  if (n < 0) {
      msg("read() error");
      return;
  }
  fprintf(stderr, "client says: %s\n", rbuf);

  char wbuf[] = "there.";
  write(connfd, wbuf, strlen(wbuf));
}

int main(){

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }

  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
  
  
  //bind
  sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = htons(1234);  // port
  addr.sin_addr.s_addr = htons(0);  // wildcard IP 0.0.0.0
  int rv = bind(fd, (const sockaddr*)&addr, sizeof(addr));
  if(rv){
    // cout << "bind()\n";
    
    die("bind()");

  }


  //listen
  rv = listen(fd, SOMAXCONN);
  if(rv){
    die("Listen()");
    // cout << "Listen()\n";
  }




  while(true){
    //accept connection!

    sockaddr_in client_addr = {};
    socklen_t addrlen = sizeof(client_addr);
    int connfd = accept(fd, (sockaddr*)&client_addr, &addrlen);
    if(connfd < 0){
      continue; //Error, accept next connection!
    }
    do_something(connfd);

    close(connfd);
  }
}
