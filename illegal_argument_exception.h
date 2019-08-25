
#ifndef ILLEGAL_ARGUMENT_EXCEPTION_H
#define ILLEGAL_ARGUMENT_EXCEPTION_H

#include <iostream>

class IllegalArgumentException : public std::exception {
    
    public:
        const char* what() const throw() {
            return "IllegalArgumentException";
        }
};

#endif // ILLEGAL_ARGUMENT_EXCEPTION_H
