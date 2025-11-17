#include "proxy_server.hpp"

 void SERVER::server(void){

         i32 proxy_server_fd= utils.create_socket();
         if(proxy_server_fd==-1){
            std::cout<<"error "<<strerror(errno)<<"\n";
            exit(EXIT_FAILURE);
         }

         FDGUARD guard(proxy_server_fd);

         /*
            The function will bind,listen and accept connections
         */
        prepare_server_socket(proxy_server_fd);
       
 
   

       
      struct sockaddr_storage client_address;
      socklen_t socket_len=sizeof(client_address);
      
         /*
           At this point the proxy now should act as a client ,to send this request to the actual server
        */

        
       while(true){
             
            std::unique_ptr<i8[]> response_buffer(new i8[BUFFER]);
            std::unique_ptr<i8[]> request_buffer(new i8[BUFFER]);
            
            i32 client_fd=accept(proxy_server_fd,(struct sockaddr*)&client_address,&socket_len);
            FDGUARD guard(client_fd);
            if(client_fd==-1){
                  std::cout<<"error "<<strerror(errno)<<"\n";
                  exit(EXIT_FAILURE);
            }
           
           ssize_t received_bytes=utils.recv_(client_fd,request_buffer);
           if(received_bytes==-1){
              std::cout<<"error"<<strerror(errno)<<"\n";
              exit(EXIT_FAILURE);
           }

           if(received_bytes==0){
               // continue;
           }
              
           proxy_client.client(request_buffer,response_buffer);
          


            ssize_t sent_bytes_to_client=utils.send_(client_fd,response_buffer);
            if(sent_bytes_to_client<0){
              std::cout<<"error "<<strerror(errno)<<"\n";
              exit(EXIT_FAILURE);
           }

           break;
            
         }


         

    
 }


 void SERVER::prepare_server_socket(i32 proxy_server_fd){
   
         struct sockaddr_in proxy_server_address;

         utils.create_address(proxy_server_address,"server");


         if(bind(proxy_server_fd,(const sockaddr*)&proxy_server_address,sizeof(proxy_server_address))==-1){
               std::cout<<"error "<<strerror(errno)<<"\n";
               exit(EXIT_FAILURE);
          }

        i32 backlog=100;


       if(listen(proxy_server_fd,backlog)==-1){
            std::cout<<"error "<<strerror(errno)<<"\n";
            exit(EXIT_FAILURE);
       }


 }

