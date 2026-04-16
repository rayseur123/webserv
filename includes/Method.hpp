#ifndef METHOD_HPP
#define METHOD_HPP

#include <iostream>

class Method
{
    private:
        int type_ : 3;

    public:
    
        int             getMethod() const;
        void            setMethod(int const type);

        bool            operator==(Method const& m) const;
        bool            operator!=(Method const& m) const;
        Method const&   operator=(Method const& to_copy);

        Method();
        Method(std::string method);
        Method(Method const& to_copy);
        ~Method();
};

std::ostream & operator<<(std::ostream& os, Method const& m);

#endif