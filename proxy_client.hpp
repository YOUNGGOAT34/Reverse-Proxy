#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "utils.hpp"
#include "exceptions.hpp"


class CLIENT{
    private:
      //private members
      UTILS utils;
      ssize_t bytes_recved;
      std::string response;
      
      //private functions

      i32 prepare_socket(void);
    public:
         CLIENT(){}
         ~CLIENT(){}
         void client(std::string& request_buffer,ssize_t& bytes);
         ssize_t get_bytes_received();
         std::string& get_response();
         

};


#endif