#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>

class Method
{

    #define POST 0
    #define GET 1
    #define DELETE 2
    #define HEAD 3
    #define UNLINK 4
    #define LINK 5
    #define PUT 6

    private:
        int type_;

    public:
    
        int             getMethod() const;
        void            setMethod(int const type);

        bool            operator==(Method const& m) const;
        bool            operator!=(Method const& m) const;
        Method&   operator=(Method const& to_copy);

        Method();
        Method(std::string method);
        Method(Method const& to_copy);
        ~Method();
};

std::ostream & operator<<(std::ostream& os, Method const& m);

#endif