#ifndef DEFINITIONS_H
#define DEFINITIONS_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <sys/socket.h>
#include <errno.h>
#include<sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include<unistd.h>
#include <memory>



const auto SERVER_PORT=4220;
const auto CLIENT_PORT=4221;
const auto BUFFER=4096;

//bytes
using i8=char;
using u8=unsigned char;

//shorts
using i16=short int;
using u16=unsigned short int;

//32-bit integers
using i32=int;
using u32=unsigned int;

//long integers
using i64=long int;
using u64=unsigned long int;

enum class AddressType{
      Server,
      Client
};

#endif