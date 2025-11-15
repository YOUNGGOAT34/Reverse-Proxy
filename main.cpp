#include <iostream>
#include "definitions.hpp"
#include<sys/socket.h>
#include<string.h>

int main(){
     i32 proxy_fd=socket(AF_INET,SOCK_STREAM,0);
     if(proxy_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
     }
     return 0;
}