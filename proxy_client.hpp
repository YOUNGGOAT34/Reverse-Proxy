#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "utils.hpp"


class CLIENT{
    private:
      //private members
      UTILS utils;
      i32 proxy_client_fd;
      ssize_t bytes_recved;
      std::string response;
      
      //private functions

      void prepare_socket();
    public:
         CLIENT();
         ~CLIENT();
         void client(std::string& request_buffer,ssize_t& bytes);
         ssize_t get_bytes_received();
         std::string& get_response();
         

};


#endif