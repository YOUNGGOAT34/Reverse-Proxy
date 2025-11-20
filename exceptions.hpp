#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class ServerException:public std::runtime_error{
   public:
        explicit ServerException(const std::string& message):std::runtime_error("Server Error: "+message){}
};

class NetworkException:public std::runtime_error{
   public:
       explicit NetworkException(const std::string& message):std::runtime_error("Network Error: "+message){}
};


class ProxyException:public std::runtime_error{
   public:
      explicit ProxyException(const std::string& message):std::runtime_error("Proxy Error: "+message){}
};

class ClientException: public std::runtime_error{
         public:
               explicit ClientException(const std::string& message):std::runtime_error("Client: "+message){}
};


class SystemFailureException: public std::runtime_error{
         public:
               explicit SystemFailureException(const std::string& message):std::runtime_error("System Failure: "+message){}
};

#endif
