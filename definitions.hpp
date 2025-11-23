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
#include <fcntl.h>
#include <mutex>
#include <sys/epoll.h>
#include <condition_variable>
#include <thread>
#include <vector>
#include "exceptions.hpp"
#include <queue>
#include <functional>


// std::mutex mtx;

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


constexpr auto RED=   "\033[1;31m";
constexpr auto GREEN=   "\033[1;32m";
constexpr auto YELLOW= "\033[1;33m";
constexpr auto RESET ="\033[0m";
constexpr auto WHITE= "\033[1;37m";

#endif