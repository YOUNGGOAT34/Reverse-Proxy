#include "proxy_client.hpp"

void CLIENT::client(std::unique_ptr<i8[]>& request_buffer,std::unique_ptr<i8[]>& response_buffer){
     
      i32 proxy_client_fd=utils.create_socket();
      FDGUARD guard(proxy_client_fd);
       if(proxy_client_fd==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
       }

         prepare_socket(proxy_client_fd);
          
        ssize_t sent_bytes_to_server=utils.send_(proxy_client_fd,request_buffer);
        
        if(sent_bytes_to_server<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }

        ssize_t received_bytes_from_server=utils.recv_(proxy_client_fd,response_buffer);
        std::cout<<"Here"<< response_buffer.get()<<std::endl;
        if(received_bytes_from_server<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }
}


void CLIENT::prepare_socket(i32 fd){
       struct sockaddr_in proxy_client_address;
       utils.create_address(proxy_client_address,"client");
      i32 connection_status=connect(fd,(struct sockaddr *)&proxy_client_address,sizeof(proxy_client_address));
      if(connection_status==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
      }
}