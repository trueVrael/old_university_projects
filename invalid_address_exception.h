#ifndef INVALID_ADDRESS_EXCEPTION_H
#define INVALID_ADDRESS_EXCEPTION_H

#include <iostream>

class InvalidAddressException : public std::exception {
    
    public:
        const char* what() const throw() {
            return "InvalidAddressException";
        }
};

#endif // INVALID_ADDRESS_EXCEPTION_H
