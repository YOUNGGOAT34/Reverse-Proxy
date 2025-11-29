#ifndef UTILS_H
#define UTILS_H
#include "definitions.hpp"


class UTILS{

  private:
      
   
   public:

         //this enum will help us to know where the timeout happened
         enum class SERVER_CLIENT{
               SERVER,
               CLIENT
         };

         i32 create_socket(void);
         void make_socket_non_blocking(i32 fd);
         void make_client_socket_non_blocking(i32 fd);
         void create_address(struct sockaddr_in& address,AddressType addr_type);
         
         ssize_t send_(i32 fd,std::string& buffer,const ssize_t bytes,SERVER_CLIENT type);
         ssize_t recv_(i32 fd,std::string& buffer,SERVER_CLIENT type);
         std::string read_headers(i32 fd,ssize_t& bytes_received,SERVER_CLIENT type);
         std::string read_body(i32 fd,std::string& headers,ssize_t& bytes_received,SERVER_CLIENT type);
         std::string build_http_response(i32 code,const std::string& reason);
         i32 calculate_remaining_time(auto& start);
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