#include "proxy_server.hpp"

 void SERVER::server(void){

         i32 proxy_server_fd= utils.create_socket();
         if(proxy_server_fd==-1){
            std::cout<<"error "<<strerror(errno)<<"\n";
            exit(EXIT_FAILURE);
         }

         /*
            The function will bind,listen and accept connections
         */
        prepare_server_socket(proxy_server_fd);
       
        std::unique_ptr<i8[]> request_buffer(new i8[BUFFER]);
        ssize_t received_bytes=utils.recv_(proxy_server_fd,request_buffer);
        if(received_bytes==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }
   

       
      struct sockaddr_storage client_address;
      socklen_t socket_len=sizeof(client_address);
      std::unique_ptr<i8[]> response_buffer(new i8[BUFFER]);
      i32 client_fd=accept(proxy_server_fd,(struct sockaddr*)&client_address,&socket_len);

      if(client_fd==-1){
        std::cout<<"error "<<strerror(errno)<<"\n";
        exit(EXIT_FAILURE);
     }

         /*
           At this point the proxy now should act as a client ,to send this request to the actual server
        */
         
        ssize_t sent_bytes_to_client=utils.send_(client_fd,response_buffer);
        if(sent_bytes_to_client<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }
        
 }


 void SERVER::prepare_server_socket(i32 proxy_server_fd){
   
         struct sockaddr_in proxy_server_address;

         utils.create_address(proxy_server_address);


         if(bind(proxy_server_fd,(const sockaddr*)&proxy_server_address,sizeof(proxy_server_address))==-1){
               std::cout<<"error "<<strerror(errno)<<"\n";
               exit(EXIT_FAILURE);
          }

        i32 backlog=100;


       if(listen(proxy_server_fd,backlog)==-1){
            std::cout<<"error "<<strerror(errno)<<"\n";
            exit(EXIT_FAILURE);
       }


      
     
      //********************************************************************************** *//
        /*
           proxy client will send request to the server here and get a response
           then this response will be sent to the client
        */
      //********************************************************************************* *//


 }

