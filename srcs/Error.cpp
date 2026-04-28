#include "Error.hpp"
#include <cstdarg>


Error::ErrorException::ErrorException():
code_(-1)
{}

Error::ErrorException::ErrorException(int code):
code_(code)
{}

int Error::ErrorException::get_code() const
{
    return code_;
}