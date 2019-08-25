
#ifndef DIVISION_BY_ZERO_EXCEPTION_H
#define DIVISION_BY_ZERO_EXCEPTION_H

#include <iostream>

class DivisionByZeroException : public std::exception {
    
    public:
        const char* what() const throw() {
            return "DivisionByZeroException";
        }
};

#endif // DIVISION_BY_ZERO_EXCEPTION_H
