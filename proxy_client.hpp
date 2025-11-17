#ifndef PROXY_CLIENT_H
#define PROXY_CLIENT_H

#include "utils.hpp"


class CLIENT{
    private:
      //private members
      UTILS utils;
      //private functions
      void prepare_socket(i32 fd);
    public:
         ssize_t bytes_recved;
         void client(std::unique_ptr<i8[]>& request_buffer,std::unique_ptr<i8[]>& response_buffer,ssize_t& bytes);

};


#endif