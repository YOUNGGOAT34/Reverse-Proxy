#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class ServerException:public std::runtime_error{
        explicit ServerException(const std::string& message):std::runtime_error("Server Error: "+message){}
};

class NetworkException:public std::runtime_error{
    explicit NetworkException(const std::string& message):std::runtime_error("Network Error: "+message){}
};


class ProxyException:public std::runtime_error{
    explicit ProxyException(const std::string& message):std::runtime_error("Proxy Error: "+message){}
};

#endif
