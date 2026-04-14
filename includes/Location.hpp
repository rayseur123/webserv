#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <utility>
#include <vector>

#define GET 1
#define POST 2
#define DELETE 4

class Location
{
    private: 
        std::string root_;
        bool        autoindex_;
        int         allow_methods_ : 3;
        std::string index_;
        std::string upload_store_;
        std::string cgi_pass_;
        std::string redirect_;

    public:

        void    setRoot(std::string const& root);
        void    setAllowMethods(std::vector<std::string> const& allow_methods);
        void    setAutoIndex(std::string const& autoindex);
        void    setIndex(std::string const& index);
        void    setUploadStore(std::string const& upload_store);
        void    setCgiPass(std::string const& cgi_pass);
        void    setRedirect(std::string const& redirect);

        void print(int depth = 0) const;

        Location();
        Location(std::string const& root, bool autoindex, int allow_methods, std::string const& index,
                std::string const& upload_store, std::string const& cgi_pass, std::string const& redirect);
        Location(Location const& to_copy);
        Location const& operator=(Location const& to_copy);
        ~Location();
};

#endif