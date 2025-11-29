#include "proxy_server.hpp"

/*
  
  error handling ....
  
*/



 void SERVER::server(void){

         
         try{

            i32 proxy_server_fd= utils.create_socket();
            utils.make_socket_non_blocking(proxy_server_fd);
            if(proxy_server_fd==-1){
                 throw SystemFailureException("Error failed to create server fd"+std::string(strerror(errno)));
            }

            i32 opt=1;

            setsockopt(proxy_server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

            create_epoll_event(proxy_server_fd,epfd);
            

            
          //   FDGUARD server_guard(proxy_server_fd);
   
         prepare_server_socket(proxy_server_fd);

           
          std::vector<epoll_event> events(1024);
          
          while(true){
                
             

               i32 result=epoll_wait(epfd,events.data(),events.size(),-1);

               if(result==-1){
                    throw SystemFailureException("Epoll wait failed "+std::string(strerror(errno)));
               }


               for(i32 i=0;i<result;++i){
                      if(events[i].data.fd==proxy_server_fd){
                        
                           accept_new_connection(proxy_server_fd);
                      }else{

                         i32 fd=events[i].data.fd;

                         epoll_ctl(epfd,EPOLL_CTL_DEL,fd,nullptr);
                         thread_pool.enqueue(
                              [this,fd](){
                                   handle_client(fd);
                              });

                      }
               }
               
            }
         }catch(const ServerException& e){
                std::cerr<<RED<<e.what()<<RESET<<"\n";
         }catch(const NetworkException& e){
                std::cerr<<RED<<e.what()<<RESET<<"\n";
         }catch(const ProxyException& e){
               std::cerr<<RED<<e.what()<<RESET<<"\n";
         }catch(const SystemFailureException& e){
              std::cerr<<RED<<e.what()<<RESET<<"\n";
              exit(EXIT_FAILURE);
         }catch(...){
              std::cerr<<RED<<"Unhandled exception"<<RESET<<"\n";
         }

 }


 void SERVER::accept_new_connection(i32 fd){

         struct sockaddr_storage client_address;
         socklen_t socket_len=sizeof(client_address);

          i32 client_fd=accept(fd,(struct sockaddr*)&client_address,&socket_len);
          
        

          if(client_fd==-1){
               throw ServerException("Failed to accept client connection: "+std::string(strerror(errno)));
          }

          utils.make_socket_non_blocking(client_fd);

          create_epoll_event(client_fd,epfd);
 }


 void SERVER::handle_client(i32 client_fd){

     
             
              std::string request_buffer;
               
              ssize_t received_bytes=utils.recv_(client_fd,request_buffer,UTILS::SERVER_CLIENT::SERVER);
            
              if(received_bytes==-1){
                   throw ServerException("Error receiving request from client: "+std::string(strerror(errno)));
              }

             
   
              if(received_bytes==0){
               
                  epoll_ctl(epfd,EPOLL_CTL_DEL,client_fd,nullptr);
                  close(client_fd);
                  return;
              }

              /*Now the reverse proxy's client will send the request to the intended server
               and get back the response ,then (reverse proxy server ) will send the response to the client
               */

               
               try{

               CLIENT proxy_client;
                 

               std::string response_buffer;
               ssize_t bytes_recved;

               proxy_client.client(request_buffer,received_bytes,response_buffer,bytes_recved);
               
               ssize_t sent_bytes_to_client=utils.send_(client_fd,response_buffer,bytes_recved,UTILS::SERVER_CLIENT::SERVER);
               if(sent_bytes_to_client<0){
                  throw ClientException("Error sending response to client "+std::string(strerror(errno)));
                  }

               }catch(const ClientTimeoutException& e){ 
                   
                    std::cout<<RED<<e.what()<<RESET<<"\n";
                    std::string response=utils.build_http_response(504,"Gateway Timeout");
                    utils.send_(client_fd,response,response.size(),UTILS::SERVER_CLIENT::CLIENT);
                    close(client_fd);

               }catch(const ClientException& e){
                    
                    std::cout<<RED<<e.what()<<RESET<<"\n";
                    std::string response=utils.build_http_response(504,"Bad Gateway");
                    utils.send_(client_fd,response,response.size(),UTILS::SERVER_CLIENT::CLIENT);
                    close(client_fd);

               }catch(...){
                    std::cout<<RED<<"unhandled client exception\n"<<RESET;

               }

 }


 void SERVER::prepare_server_socket(i32 proxy_server_fd){
   
         struct sockaddr_in proxy_server_address;

         utils.create_address(proxy_server_address,AddressType::Server);


         if(bind(proxy_server_fd,(const sockaddr*)&proxy_server_address,sizeof(proxy_server_address))==-1){
                throw SystemFailureException("Proxy Server exception: " + std::string(strerror(errno)));
          }

        i32 backlog=100;


       if(listen(proxy_server_fd,backlog)==-1){
             throw SystemFailureException("Proxy Server exception: " + std::string(strerror(errno)));
       }
       
 }


  i32 SERVER::create_epoll_event(i32 fd,i32 epfd){
            epoll_event ev{};
            ev.data.fd =fd;
            ev.events = EPOLLIN; 
           
            if (epoll_ctl(epfd, EPOLL_CTL_ADD,fd, &ev) == -1) {
               if (errno == EEXIST){

                  epoll_ctl(epfd, EPOLL_CTL_MOD,fd, &ev);
               }else{
                  throw ProxyException("Failed to create epoll event ,"+std::string(strerror(errno)));
               }
            }

   return 0;
}

