#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include "Version.hpp"
#include "Method.hpp"
#include "Type.hpp"
#include "Body.hpp"
#include "Uri.hpp"

class Request
{
    private:
        Method  method_;
        Uri     uri_;
        Version version_;
        Type    type_;
        Body    body_;

    public:

        void    setMethod(Method const& method);
        void    setUri(Uri const& method);
        void    setVersion(Version const& version);
        void    setType(Type const& type);
        void    setBody(Body const& body);

        Method  const&  getMethod()     const;
        Uri     const&  getUri()        const;
        Version const&  getVersion()    const;
        Type    const&  getType()       const;
        Body    const&  getBody()       const;

        Request();
        Request(std::string request);
        Request(Request const& to_copy);
        Request const& operator=(Request const& to_copy);
        ~Request();
};

std::ostream& operator<<(std::ostream& os, Request const& r);

#endif