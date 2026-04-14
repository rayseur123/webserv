#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>

std::string messageError(std::string const& function_name);

template <typename T>
std::ostream & operator<<(std::ostream& os, std::vector<T> list)
{
    typename std::vector<T>::iterator it;

    os << "----------------------------" << std::endl;
    for (it = list.begin(); it != list.end(); ++it)
    {
        os << *it;
        os << "----------------------------" << std::endl;
    }
    os << std::endl;
    return os;
}

#endif