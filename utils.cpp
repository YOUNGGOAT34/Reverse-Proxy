#include "utils.hpp"

i32 UTILS::create_socket(void){
     return socket(AF_INET,SOCK_STREAM,0);
}

 void UTILS::create_address(struct sockaddr_in& address,AddressType addr_type){
       memset(&address,0,sizeof(address));

        //  address.sin_addr.s_addr=htonl(addr_type==AddressType::Server?INADDR_ANY:192.168.100.5);
         if(addr_type==AddressType::Server){
              address.sin_port=htons(SERVER_PORT);
              address.sin_addr.s_addr=htonl(INADDR_ANY);
         }else{
               address.sin_port=htons(4221);
               //  if(inet_pton(AF_INET,"192.168.100.5",&address.sin_addr.s_addr)!=1){
               //        throw NetworkException("Invalid upstream IP address "+std::string(strerror(errno)));
               //  }

               address.sin_addr.s_addr=htonl(INADDR_LOOPBACK);


         }
         address.sin_family=AF_INET;
           
 }


void UTILS::make_socket_non_blocking(i32 fd){
          i32 flags=fcntl(fd,F_GETFL,0);

          if(flags==-1){
              throw ServerException("Error getting the socket flags "+std::string(strerror(errno)));
          }

          i32 status= fcntl(fd,F_SETFL,flags | O_NONBLOCK);   
          if(status==-1){
               throw ServerException("Error setting the socket flags "+std::string(strerror(errno)));
          }

   }


void UTILS::make_client_socket_non_blocking(i32 fd){
          i32 flags=fcntl(fd,F_GETFL,0);

          if(flags==-1){
              throw ServerException("Error getting the client socket flags "+std::string(strerror(errno)));
          }

          i32 status= fcntl(fd,F_SETFL,flags | O_NONBLOCK);   
          if(status==-1){
               throw ServerException("Error setting client the socket flags "+std::string(strerror(errno)));
          }

   }


 ssize_t UTILS::recv_(i32 fd,std::string& buffer){

     
     ssize_t bytes_received=0;

      std::string headers=read_headers(fd,bytes_received);
    
      std::string body=read_body(fd,headers,bytes_received);
      std::string res=headers+body;
      buffer=std::move(res);
    
      return bytes_received;

 }



std::string  UTILS::read_headers(i32 fd,ssize_t& total_bytes_received){
     i8 buffer[BUFFER]={0};
     std::string request_data;
     while(true){
         ssize_t received_bytes=recv(fd,buffer,BUFFER,0);
         if(received_bytes>0){
              
             total_bytes_received+=received_bytes;
             request_data.append(buffer,received_bytes);
             if(request_data.find("\r\n\r\n")!=std::string::npos){
                 break;
             }

             continue;
         }else if(received_bytes==0){
             
             break;
         }else if(received_bytes<0){

               

                 if(errno==EAGAIN || errno==EWOULDBLOCK){
                    /*
                       Figure out on how to handle this 
                    */
                    // break;
                   }else if (errno == ECONNRESET || errno == EBADF || errno == ENOTCONN) {
          
                         break;
                  }else if(errno==EINTR){
                        continue;
                  }
         }
        
     }

     

     return request_data;
    
}


std::string UTILS::read_body(i32 fd,std::string& headers,ssize_t& total_bytes_received){
    i8 buffer[BUFFER]={0};
    std::string body;
    //find content length
    size_t content_len_start=headers.find("Content-Length:");
    if(content_len_start==std::string::npos){
       return "";
    }
    size_t content_len_end=headers.find("\r\n",content_len_start);

    if(content_len_end==std::string::npos){
       return "";
    }


    std::string content_len_string=headers.substr(content_len_start+strlen("Content-Length:"),content_len_end-(content_len_start+strlen("Content-Length:")));
   
    while(!content_len_string.empty()&&isspace(content_len_string.front())){
           content_len_string.erase(content_len_string.begin());
    }


     size_t content_len=std::strtoul(content_len_string.c_str(),NULL,10);

    size_t headers_end=headers.find("\r\n\r\n");
      if(headers_end!=std::string::npos){
         size_t body_start=headers_end+4;

       if(headers.size()>body_start){
          body=headers.substr(body_start);
       }
    
    }

    

    auto start_clock=std::chrono::steady_clock::now();

    while(body.size()<content_len){
           ssize_t bytes_received_=recv(fd,buffer,BUFFER,0);

           if(bytes_received_>0){
                total_bytes_received+=bytes_received_;
                body.append(buffer,bytes_received_);
                continue;
           }else if(bytes_received_==0){
            //  break;
         }else if(bytes_received_<0){

                 if(errno==EAGAIN || errno==EWOULDBLOCK){

                      i32 timeout=calculate_remaining_time(start_clock);
                      if(timeout==0){
                          throw ClientTimeoutException("Server timed out sending response ");
                      }

                      struct pollfd pfd;
                      pfd.fd=fd;
                      pfd.events=POLLIN;
                      i32 poll_result=poll(&pfd,1,timeout);


                      if(poll_result==0){
                          throw ClientTimeoutException("Upstream server timed out ");
                      }else if(poll_result<0){
                          throw ClientException("Error while polling , "+std::string(strerror(errno)));
                      }
                      continue;

             
                   }else if (errno == ECONNRESET || errno == EBADF || errno == ENOTCONN) {
          
                         break;
                }
         }
    }



    return body;
}


 ssize_t UTILS::send_(i32 fd,std::string& buffer,const ssize_t bytes){
       ssize_t sent_bytes=0;
       
       errno=0;
       while(sent_bytes<bytes){
             ssize_t sent=send(fd,buffer.data()+sent_bytes,bytes-sent_bytes,0);
            
          

             if(sent>0){
                
                sent_bytes+=sent;
                
             }
             else if(sent==-1){
                  if(errno==EAGAIN || errno==EWOULDBLOCK){
                    /*
                      Figure out on how to handle this
                    */
                    //   break;
                  }else if(errno==EINTR){
                       continue;
                  }else{
                     return -1;
                  }

             }

    
   
       }

      
       

       return sent_bytes;
       
 }





 std::string UTILS::build_http_response(i32 code,const std::string& reason){

       std::string body=std::to_string(code)+" "+reason;
    
       std::string headers=
         "HTTP/1.1 " + std::to_string(code) + " " + reason + "\r\n"
        "Content-Type: text/plain\r\n"
        "Content-Length: " + std::to_string(body.size()) + "\r\n"
        "Connection: close\r\n"
        "\r\n";

        return headers+body;

 }


 i32 UTILS::calculate_remaining_time(auto& start){
        auto now=std::chrono::steady_clock::now();
        auto elapsed_time=std::chrono::duration_cast<std::chrono::milliseconds>(now-start).count();
        i32 left=TIMEOUT_MS-static_cast<i32>(elapsed_time);

        return left>0 ?left:0;
 }