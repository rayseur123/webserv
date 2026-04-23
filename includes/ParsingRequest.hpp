#ifndef PARSING_REQUEST_HPP
#define PARSING_REQUEST_HPP

#include <iostream>
#include "Request.hpp"

enum 
{
    REQUEST,
    HEADER,
    BODY,
    FINISH 
};

class ParsingRequest
{
    private:
        std::string buffer_;
        Request     request_;
        int         step_;

        void    requestLine(std::string line);
        void    headerLine(std::string line);
        void    bodyLine(std::string line);

    public:

        void fillBuffer(std::string tmp);
        
        Request const&  getRequest() const;
        int             getStep() const;

        ParsingRequest const& operator=(ParsingRequest const& to_copy);

        ParsingRequest();
        ParsingRequest(ParsingRequest const& to_copy);
        ~ParsingRequest();
};

#endif