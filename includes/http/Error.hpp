#ifndef ERROR_HPP
#define ERROR_HPP

#include <bits/stdc++.h>

class Error
{
    private:

    public:


    class ErrorException: public std::exception
    {
        private:
            int code_;
        
        public:
            int get_code() const;
            ErrorException(int code);
            ErrorException();
    };
};


#endif