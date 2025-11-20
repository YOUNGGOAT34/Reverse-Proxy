#include "proxy_server.hpp"


/*
  
  error handling ....
  
*/

 void SERVER::server(void){

         
         try{

            i32 proxy_server_fd= utils.create_socket();
            if(proxy_server_fd==-1){
                 throw SystemFailureException("Error failed to create server fd"+std::string(strerror(errno)));
            }

             i32 opt=1;

            setsockopt(proxy_server_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
            
            FDGUARD server_guard(proxy_server_fd);
   
       

         prepare_server_socket(proxy_server_fd);
         struct sockaddr_storage client_address;
         socklen_t socket_len=sizeof(client_address);
           

   
      
          while(true){
                
               std::string request_buffer;
   
               i32 client_fd=accept(proxy_server_fd,(struct sockaddr*)&client_address,&socket_len);
               FDGUARD client_guard(client_fd);
               if(client_fd==-1){
                    throw ServerException("Failed to accept client connection: "+std::string(strerror(errno)));
               }
              
              ssize_t received_bytes=utils.recv_(client_fd,request_buffer);
              if(received_bytes==-1){
                   throw ServerException("Error receiving request from client: "+std::string(strerror(errno)));
              }
   
              if(received_bytes==0){
                  continue;
              }

              /*Now the reverse proxy's client will send the request to the intended server
               and get back the response ,then (reverse proxy server ) will send the response to the client
               */
               
               try{

               proxy_client.client(request_buffer,received_bytes);
               std::string response_buffer=proxy_client.get_response();
               ssize_t sent_bytes_to_client=utils.send_(client_fd,response_buffer,proxy_client.get_bytes_received());
               if(sent_bytes_to_client<0){
                  throw ClientException("Error sending response to client "+std::string(strerror(errno)));
                  }

               }catch(const ClientTimeoutException& e){
                    std::cout<<RED<<e.what()<<RESET<<"\n";
                    std::string response=utils.build_http_response(504,"Bad Gateway",e.what());
                    utils.send_(client_fd,response,response.size());

               }catch(const ClientException& e){
                    std::cout<<RED<<e.what()<<RESET<<"\n";
                    std::string response=utils.build_http_response(504,"Gateway Timeout",e.what());
                    utils.send_(client_fd,response,response.size());

               }catch(...){
                    std::cout<<RED<<"unhandled client exception\n"<<RESET;

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

