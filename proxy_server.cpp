#include "proxy_server.hpp"

 void SERVER::start_server(){
     
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


      //********************************************************************************** *//
        /*
           proxy client will send request to the server here and get a response
           then this response will be sent to the client
        */
      //********************************************************************************* *//
        ssize_t sent_bytes_to_client=send(client_fd,response_buffer.get(),BUFFER,0);
        if(sent_bytes_to_client<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }

 }