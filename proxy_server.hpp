#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

// #include "definitions.hpp"
#include "utils.hpp"



class SERVER{
    //recv
    //send

    private:
          //private members
          UTILS utils;
          //private member functions
          void prepare_server_socket(i32 proxy_server_fd);
          

    public:
         void server(void);
};




#endif