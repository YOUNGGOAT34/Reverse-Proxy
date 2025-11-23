#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H


#include "proxy_client.hpp"
#include "threadpool.hpp"



class SERVER{
    //recv
    //send

    private:
          //private members
          UTILS utils;
          
          i32 epfd;
          THREADPOOL thread_pool;
          
           
          //private member functions
          void prepare_server_socket(i32 proxy_server_fd);
           i32 create_epoll_event(i32 fd,i32 epfd);
           void accept_new_connection(i32 client_fd);
           void handle_client(i32 client_fd);
          

    public:
         SERVER():thread_pool(32){
            epfd=epoll_create1(0);

                    if(epfd==-1){
                        throw SystemFailureException("Failed to create epoll file descriptor, "+std::string(strerror(errno)));
                    }
                }
         void server(void);
};


struct CONNECTION{

};





#endif