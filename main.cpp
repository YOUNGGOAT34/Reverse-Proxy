#include <iostream>
#include "definitions.hpp"
#include<sys/socket.h>
#include <netinet/in.h>
#include<string.h>
#include <memory>

const auto SERVER_PORT=4220;
const auto CLIENT_PORT=4221;
const auto BUFFER=4096;


int main(){



      /*
        socket
        bind
        listen
        accept
     */

     i32 proxy_server_fd=socket(AF_INET,SOCK_STREAM,0);
     if(proxy_server_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }
    
     struct sockaddr_in proxy_server_address;
     
     memset(&proxy_server_address,0,sizeof(proxy_server_address));

     proxy_server_address.sin_addr.s_addr=INADDR_ANY;
     proxy_server_address.sin_family=AF_INET;
     proxy_server_address.sin_port=htons(SERVER_PORT);



     if(bind(proxy_server_fd,(const sockaddr*)&proxy_server_address,sizeof(proxy_server_address))==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

     i32 backlog=100;

     if(listen(proxy_server_fd,backlog)==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

     struct sockaddr_storage client_address;
     socklen_t socket_len=sizeof(client_address);



     std::unique_ptr<i8[]> request_buffer(new i8[BUFFER]);
     std::unique_ptr<i8[]> response_buffer(new i8[BUFFER]);

     while(1){
     i32 client_fd=accept(proxy_server_fd,(struct sockaddr*)&client_address,&socket_len);

      if(client_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

        ssize_t received_bytes=recv(client_fd,request_buffer.get(),BUFFER,0);
        if(received_bytes==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }


            /*
        Now we need to simulate something like a client here...that will send the client request to the actual server
        socket
        connect
        send
        recv
     */
     
       i32 proxy_client_fd=socket(AF_INET,SOCK_STREAM,0);

       if(proxy_client_fd==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
       }


       struct sockaddr_in proxy_client_address;
       
       memset(&proxy_client_address,0,sizeof(proxy_client_address));

       proxy_client_address.sin_family=AF_INET;
       proxy_client_address.sin_port=htons(CLIENT_PORT);
       proxy_client_address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);
       
      i32 connection_status= connect(proxy_client_fd,(struct sockaddr *)&proxy_client_address,sizeof(proxy_client_address));
      
      if(connection_status==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
      }

        ssize_t sent_bytes_to_server=send(proxy_client_fd,request_buffer.get(),BUFFER,0);

        if(sent_bytes_to_server<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }

        ssize_t received_bytes_from_server=recv(proxy_client_fd,response_buffer.get(),BUFFER,0);
        if(received_bytes_from_server<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }

        ssize_t sent_bytes_to_client=send(client_fd,response_buffer.get(),BUFFER,0);
        if(sent_bytes_to_client<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }
        
        break;
     }


     //close the server and client fds
     return 0;
}