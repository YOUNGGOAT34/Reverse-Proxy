#include "proxy_client.hpp"

void CLIENT::client(std::string& request_buffer,ssize_t& bytes){

         
     
        i32 proxy_client_fd= prepare_socket();
        ssize_t sent_bytes_to_server=utils.send_(proxy_client_fd,request_buffer,bytes);
        
        if(sent_bytes_to_server<0){
           close(proxy_client_fd);
           throw ClientException("Failed to send clien's request to the server "+std::string(strerror(errno)));
        }
  
        response.clear();

        bytes_recved=utils.recv_(proxy_client_fd,response);
        if(bytes_recved<0){
            close(proxy_client_fd);
           if(errno==ETIMEDOUT){
               throw ClientTimeoutException("Upstream server timeout");
           }
           throw ClientException("Failed to receive server's response to the client "+std::string(strerror(errno)));
        }
}


i32 CLIENT::prepare_socket(void){
       i32 proxy_client_fd=utils.create_socket();
    //    utils.make_client_socket_non_blocking(proxy_client_fd);
       struct sockaddr_in proxy_client_address;
       utils.create_address(proxy_client_address,AddressType::Client);
      i32 connection_status=connect(proxy_client_fd,(struct sockaddr *)&proxy_client_address,sizeof(proxy_client_address));
      if(connection_status==-1){
        // if(errno!=EINPROGRESS){

            close(proxy_client_fd);
              throw ClientException("Failed to connect to the server "+std::string(strerror(errno)));
        //   }
        }

      return proxy_client_fd;
}



ssize_t CLIENT::get_bytes_received(){
     return bytes_recved;
}

std::string& CLIENT::get_response(){
    return response;
}