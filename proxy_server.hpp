#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

// #include "definitions.hpp"
#include "proxy_client.hpp"



class SERVER{
    //recv
    //send

    private:
          //private members
          UTILS utils;
          CLIENT proxy_client;

          //private member functions
          void prepare_server_socket(i32 proxy_server_fd);
          

    public:
         void server(void);
};




#endif