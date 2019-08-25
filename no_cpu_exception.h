#ifndef NO_CPU_EXCEPTION_H
#define NO_CPU_EXCEPTION_H

#include <iostream>

class NoCPUException : public std::exception {
    
    public:
        const char* what() const throw() {
            return "NoCPUException";
        }
};

#endif // NO_CPU_EXCEPTION_H
