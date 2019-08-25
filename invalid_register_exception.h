#ifndef INVALID_REGISTER_EXCEPTION_H
#define INVALID_REGISTER_EXCEPTION_H

#include <iostream>

class InvalidRegisterException : public std::exception {
    
    public:
        const char* what() const throw() {
            return "InvalidRegisterException";
        }
};

#endif // INVALID_REGISTER_EXCEPTION_H
