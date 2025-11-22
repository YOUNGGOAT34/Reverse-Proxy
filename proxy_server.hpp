#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


#include "proxy_client.hpp"



class SERVER{
    //recv
    //send

    private:
          //private members
          UTILS utils;
          CLIENT proxy_client;
          i32 epfd;
           
          //private member functions
          void prepare_server_socket(i32 proxy_server_fd);
           i32 create_epoll_event(i32 fd,i32 epfd);
           void accept_new_connection(i32 client_fd);
           void handle_client(i32 client_fd);
          

    public:
         SERVER();
         void server(void);
};




#endif