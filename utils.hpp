#ifndef UTILS_H
#define UTILS_H
#include "definitions.hpp"


class UTILS{

  private:
      
   
   public:
         i32 create_socket(void);
         void make_socket_non_blocking(i32 fd);
         void make_client_socket_non_blocking(i32 fd);
         void create_address(struct sockaddr_in& address,AddressType addr_type);
         
         ssize_t send_(i32 fd,std::string& buffer,const ssize_t bytes);
         ssize_t recv_(i32 fd,std::string& buffer);
         std::string read_headers(i32 fd,ssize_t bytes_received);
         std::string read_body(i32 fd,std::string& headers,ssize_t bytes_received);
         std::string build_http_response(i32 code,const std::string& reason,const std::string& body);
};

struct FDGUARD{
       i32 fd;
       FDGUARD(i32 fd__):fd(fd__){}

       ~FDGUARD(){
            if(fd>=0){

                  close(fd);
            }
       }
};

#endif