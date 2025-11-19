#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class ServerException:public std::runtime_error{
        explicit ServerException(const std::string& message):std::runtime_error("Server Error: "+message){}
};

class NetworkException:public std::runtime_error{
    explicit NetworkException(const std::string& message):std::runtime_error("Server Error: "+message){}
};

#endif
