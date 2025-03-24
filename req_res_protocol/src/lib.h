#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include<iostream>

static void msg(const char*);
static void die(const char*);
static int32_t read_full(int, char *, size_t);
static int32_t write_all(int , const char *, size_t);
