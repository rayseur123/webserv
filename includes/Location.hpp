#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include "Method.hpp"

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

        std::string const&  getRoot() const;
        bool                getAutoIndex() const;
        int                 getAllowMethods() const;
        std::string const&  getIndex() const;
        std::string const&  getUploadStore() const;
        std::string const&  getCgiPass() const;
        std::string const&  getRedirect() const;

        Location();
        Location(std::string const& root, bool autoindex, int allow_methods, std::string const& index,
                std::string const& upload_store, std::string const& cgi_pass, std::string const& redirect);
        Location(Location const& to_copy);
        Location const& operator=(Location const& to_copy);
        ~Location();
};

std::ostream&   operator<<(std::ostream& os, Location const& to_print);

#endif