#include "proxy_client.hpp"

void CLIENT::client(std::string& request_buffer,ssize_t& bytes,std::string& res,ssize_t& bytes_recvd){
        
       
         
        i32 proxy_client_fd= prepare_socket();
        ssize_t sent_bytes_to_server=utils.send_(proxy_client_fd,request_buffer,bytes);
        
        if(sent_bytes_to_server<0){
           close(proxy_client_fd);
           throw ClientException("Failed to send clien's request to the server "+std::string(strerror(errno)));
        }
  
        
        ssize_t bytes_recved=utils.recv_(proxy_client_fd,res);
        std::cout<<"We received "<<bytes_recvd<<std::endl; 
        std::cout<<res.size()<<std::endl;
        if(bytes_recved<0){
            close(proxy_client_fd);
           if(errno==ETIMEDOUT){
               throw ClientTimeoutException("Upstream server timeout");
           }
           throw ClientException("Failed to receive server's response to the client "+std::string(strerror(errno)));
        }

        bytes_recvd=bytes_recved;


        close(proxy_client_fd);
}


i32 CLIENT::prepare_socket(void){
       i32 proxy_client_fd=utils.create_socket();
       utils.make_client_socket_non_blocking(proxy_client_fd);
       struct sockaddr_in proxy_client_address;
       utils.create_address(proxy_client_address,AddressType::Client);
      i32 connection_status=connect(proxy_client_fd,(struct sockaddr *)&proxy_client_address,sizeof(proxy_client_address));
      if(connection_status==-1 && errno!=EINPROGRESS){
       

            close(proxy_client_fd);
              throw ClientException("Failed to connect to the server "+std::string(strerror(errno)));
       
        }

        if(connection_status==-1 && errno==EINPROGRESS){
             struct pollfd pfd{};
             pfd.fd=proxy_client_fd;
             pfd.events=POLLOUT;

             i32 poll_status=poll(&pfd,1,5000);

             if(poll_status==0){
                 close(proxy_client_fd);
                 throw ClientTimeoutException("Timeout connecting to upstream seever");
             }else if(poll_status<0){
                 close(proxy_client_fd);
                 throw ClientException("Poll error on connect "+std::string(strerror(errno)));
             }else{
                   i32 so_error=0;
                   socklen_t len=sizeof(so_error);

                   if(getsockopt(proxy_client_fd,SOL_SOCKET,SO_ERROR,&so_error,&len)<0 || so_error!=0){
                        close(proxy_client_fd);
                        throw ClientException("connection failed after poll ,"+std::string(strerror(errno)));
                   }
             }
        }

      return proxy_client_fd;
}


