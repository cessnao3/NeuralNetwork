#ifndef __IO_NEURAL_EXCEPTION__
#define __IO_NEURAL_EXCEPTION__

#include <exception>
#include <string>

class neural_exception : public std::exception
{
public:
    neural_exception(const char* str) :
        std::exception(str)
    {
        // Empty Constructor
    }

    neural_exception(const std::string& str) :
        std::exception(str.c_str())
    {
        // Empty Constructo
    }
};

#endif
