#include "utils.hpp"

i32 UTILS::create_socket(void){
     return socket(AF_INET,SOCK_STREAM,0);
}

 void UTILS::create_address(struct sockaddr_in& address){
       memset(&address,0,sizeof(address));

         address.sin_addr.s_addr=INADDR_ANY;
         address.sin_family=AF_INET;
         address.sin_port=htons(SERVER_PORT);   
 }


 ssize_t UTILS::recv_(i32 fd,std::unique_ptr<i8[]> buffer){
      return recv(fd,buffer.get(),BUFFER,0);
 }

 ssize_t UTILS::send_(i32 fd,std::unique_ptr<i8[]> buffer){
       return send(fd,buffer.get(),BUFFER,0);
 }