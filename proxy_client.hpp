#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "utils.hpp"
#include "exceptions.hpp"


class CLIENT{
    private:
      //private members
      UTILS utils;
      
      //private functions

      i32 prepare_socket(void);
    public:
         CLIENT(){}
         ~CLIENT(){}
         void client(std::string& request_buffer,ssize_t& bytes,std::string& response,ssize_t& bytes_recvd);
         
         

};


#endif