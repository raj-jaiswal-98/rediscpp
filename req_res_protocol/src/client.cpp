#include <stdint.h>
#include <cstdint>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include<assert.h>

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

static int32_t read_full(int fd, char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = read(fd, buf, n);
        if (rv <= 0) {
            return -1;  // error, or unexpected EOF
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

static int32_t write_all(int fd, const char *buf, size_t n) {
    while (n > 0) {
        ssize_t rv = write(fd, buf, n);
        if (rv <= 0) {
            return -1;  // error
        }
        assert((size_t)rv <= n);
        n -= (size_t)rv;
        buf += rv;
    }
    return 0;
}

const size_t k_max_msg = 4096;

static int32_t query(int fd, const char *text) {
    uint32_t len = (uint32_t)strlen(text);
    if(len > k_max_msg){
        return -1;
    }

    //send request to server

    char wbuf[4+k_max_msg];
    memcpy(wbuf, &len, 4); //copy the header to buffer
    memcpy(&wbuf[4], text, len);
    if(int32_t err = write_all(fd, wbuf, 4+len)){
        return err;
    }

    //read the response!

    char rbuf[4+k_max_msg];
    errno = 0;
    int32_t err = read_full(fd, rbuf, 4);
    if(err){
        msg(errno == 0 ? "EOF": "read() error!");
        return err;
    }
    memcpy(&len, rbuf, 4);
    if(len > k_max_msg){
        msg("too long request");
        return -1;
    }


    //reply body
    err = read_full(fd, &rbuf[4], len);
    if(err){
        msg("read() error");
        return -1;
    }

    printf("Server says: %.*s\n", len, &rbuf[4]);
    return 0;

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
    //connected successfully!

    int err = query(fd, "hello");
    if(err){
        msg("query()");
    }
    err = query(fd, "hello2");
    if (err) {
        msg("query()");
    }
    close(fd);
    return 0;
}