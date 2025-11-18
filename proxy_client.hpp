#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "utils.hpp"


class CLIENT{
    private:
      //private members
      UTILS utils;
      i32 proxy_client_fd;
      ssize_t bytes_recved=0;
      std::string response;
      
      //private functions
      std::string read_headers();
      std::string read_body(std::string& body);
      void prepare_socket();
    public:
         CLIENT();
         void client(std::unique_ptr<i8[]>& request_buffer,std::unique_ptr<i8[]>& response_buffer,ssize_t& bytes);
         ssize_t get_bytes_received();
         

};


#endif