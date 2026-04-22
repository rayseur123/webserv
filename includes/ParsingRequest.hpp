#ifndef PARSING_REQUEST_HPP
#define PARSING_REQUEST_HPP

#include <iostream>
#include "Request.hpp"

enum 
{
    REQUEST,
    HEADER,
    BODY
};

class ParsingRequest
{
    private:
        std::string buffer_;
        Request     request_;
        int         step_;

        void    requestLine();
        void    headerLine();
        void    bodyLine();

    public:

        void fillBuffer(std::string tmp);
        
        Request& const getRequest() const;

        ParsingRequest const& operator=(ParsingRequest const& to_copy);

        ParsingRequest();
        ParsingRequest(Request const& to_copy);
        ~ParsingRequest();
};

#endif