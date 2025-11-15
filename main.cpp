#include <iostream>
#include "definitions.hpp"
#include<sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include <memory>

const auto PORT=4221;
const auto BUFFER=4096;


int main(){



      /*
        socket
        bind
        listen
        accept
     */

     i32 proxy_fd=socket(AF_INET,SOCK_STREAM,0);
     if(proxy_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }
    
     struct sockaddr_in proxy_address;

     proxy_address.sin_addr.s_addr=INADDR_ANY;
     proxy_address.sin_family=AF_INET;
     proxy_address.sin_port=htons(PORT);



     if(bind(proxy_fd,(const sockaddr*)&proxy_address,sizeof(proxy_address))==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

     i32 backlog=100;

     if(listen(proxy_fd,backlog)==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

     struct sockaddr_storage client_address;
     socklen_t socket_len=sizeof(client_address);



     std::unique_ptr<i8[]> buffer(new i8[BUFFER]);
     while(1){


     i32 client_fd=accept(proxy_fd,(struct sockaddr*)&client_address,&socket_len);

      if(client_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

        ssize_t received_bytes=recv(client_fd,buffer.get(),BUFFER,0);
        if(received_bytes==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }
   
        std::cout<<buffer.get()<<std::endl;
     }





     return 0;
}