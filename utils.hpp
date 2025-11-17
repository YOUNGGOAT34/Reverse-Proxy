#ifndef UTILS_H
#define UTILS_H
#include "definitions.hpp"

class UTILS{
   
   public:
         i32 create_socket(void);
         void create_address(struct sockaddr_in& address,std::string client_or_server);
         ssize_t send_(i32 fd,std::unique_ptr<i8[]>& buffer);
         ssize_t recv_(i32 fd,std::unique_ptr<i8[]>& buffer);
};

struct FDGUARD{
       i32& fd;
        
       FDGUARD(i32& fd__):fd(fd__){}

       ~FDGUARD(){
            if(fd>=0){

                  close(fd);
            }
       }
};

#endif