#include "proxy_client.hpp"

CLIENT::CLIENT(){
     proxy_client_fd=utils.create_socket();
      if(proxy_client_fd==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
       }
}

void CLIENT::client(std::unique_ptr<i8[]>& request_buffer,std::unique_ptr<i8[]>& response_buffer,ssize_t& bytes){
     
         prepare_socket();
         
        ssize_t sent_bytes_to_server=utils.send_(proxy_client_fd,request_buffer,bytes);
        
        if(sent_bytes_to_server<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }

        bytes_recved=utils.recv_(proxy_client_fd,response_buffer);
        if(bytes_recved<0){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
        }
}


void CLIENT::prepare_socket(){
       struct sockaddr_in proxy_client_address;
       utils.create_address(proxy_client_address,AddressType::Client);
      i32 connection_status=connect(proxy_client_fd,(struct sockaddr *)&proxy_client_address,sizeof(proxy_client_address));
      if(connection_status==-1){
           std::cout<<"error "<<strerror(errno)<<"\n";
           exit(EXIT_FAILURE);
      }
}

std::string  CLIENT::read_headers(){

     i8 buffer[BUFFER]={0};
     std::string request_data;

     while(true){

         ssize_t received_bytes=recv(proxy_client_fd,buffer,BUFFER,0);

         if(received_bytes>0){
             bytes_recved+=received_bytes;
             request_data.append(buffer,received_bytes);

             if(request_data.find("\r\n\r\n")!=std::string::npos){
                 break;
             }

             continue;
         }else if(received_bytes==0){
             break;
         }else if(received_bytes<0){

                 if(errno==EAGAIN || errno==EWOULDBLOCK){
                     
                      break;
                   }else if (errno == ECONNRESET || errno == EBADF || errno == ENOTCONN) {
          
                         break;
                }
         }
        
     }

     return request_data;
    
}


std::string CLIENT::read_body(std::string& headers){
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
    size_t content_len=std::strtoul(content_len_string.c_str(),NULL,0);


    while(!content_len_string.empty()&&isspace(content_len_string.front())){
           content_len_string.erase(content_len_string.begin());
    }

    size_t headers_end=headers.find("\r\n\r\n");
      if(headers_end!=std::string::npos){
         size_t body_start=headers_end+4;

       if(headers.size()>body_start){
          body=headers.substr(body_start);
       }
    
    }

    


    while(body.size()<content_len){
           ssize_t bytes_received=recv(proxy_client_fd,buffer,BUFFER-1,0);

           if(bytes_received>0){
                body.append(buffer,bytes_received);
                continue;
           }else if(bytes_received==0){
             break;
         }else if(bytes_received<0){

                 if(errno==EAGAIN || errno==EWOULDBLOCK){
                     
                      break;
                   }else if (errno == ECONNRESET || errno == EBADF || errno == ENOTCONN) {
          
                         break;
                }
         }
    }



    return body;
}

ssize_t CLIENT::get_bytes_received(){
     return bytes_recved;
}