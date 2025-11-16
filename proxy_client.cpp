#include "proxy_client.hpp"

void CLIENT::start_client(){
    
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


     std::unique_ptr<i8[]> request_buffer(new i8[BUFFER]);
     std::unique_ptr<i8[]> response_buffer(new i8[BUFFER]);

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
}