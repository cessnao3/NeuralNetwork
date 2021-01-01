#ifndef __IO_NEURAL_EXCEPTION__
#define __IO_NEURAL_EXCEPTION__

#include <stdexcept>
#include <string>

class neural_exception : public std::runtime_error
{
public:
    neural_exception(const char* str) :
        std::runtime_error(str)
    {
        // Empty Constructor
    }

    neural_exception(const std::string& str) :
        std::runtime_error(str.c_str())
    {
        // Empty Constructor
    }
};

#endif
