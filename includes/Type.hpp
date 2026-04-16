#ifndef TYPE_HPP
#define TYPE_HPP

#include <iostream>

class Type
{
    private:
        int         content_length_;
        std::string content_type_;

    public:

        void    setContentLength(int const length);
        void    setContentType(std::string const& type);
        
        int                 getContentLength() const;
        std::string const&  getContentType() const;

        Type const& operator=(Type const& to_copy);

        Type();
        Type(std::string type);
        Type(Type const& to_copy);
        ~Type();
};

std::ostream & operator<<(std::ostream& os, Type const& m);

#endif