#include <iostream>
#include "definitions.hpp"
#include<sys/socket.h>
#include <netinet/in.h>
#include<string.h>

const auto PORT=4221;


int main(){
     i32 proxy_fd=socket(AF_INET,SOCK_STREAM,0);
     if(proxy_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
     }
    
     struct sockaddr_in proxy_address;

     proxy_address.sin_addr.s_addr=INADDR_ANY;
     proxy_address.sin_family=AF_INET;
     proxy_address.sin_port=htonl(PORT);
     

     return 0;
}