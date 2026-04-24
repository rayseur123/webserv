#ifndef HEADER_HPP
#define HEADER_HPP

#include <iostream>
#include <map>

class Header
{
    private:
        std::map<std::string, std::string> headers_;

        void    typeAccepted(std::string& value);
    public:

        void set(const std::string& key, std::string& value);
        std::string& get(const std::string &key);
        std::map<std::string, std::string> const& getHeaders() const;
        bool has(const std::string &key) const;
        
        int getContentLength(); 

        Header const& operator=(Header const& to_copy);

        Header();
        Header(Header const& to_copy);
        ~Header();
};

std::ostream& operator<<(std::ostream& os, Header const& m);

#endif