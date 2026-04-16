#ifndef BODY_HPP
#define BODY_HPP

#include <iostream>

class Body
{
    private:
        std::string content_;

    public:

        std::string const &getContent() const;

        void        setContent(std::string const &content);

        Body();
        Body(std::string content);
        Body(Body const& to_copy);
        Body const& operator=(Body const& to_copy);
        ~Body();
};

std::ostream & operator<<(std::ostream& os, Body const& m);

#endif