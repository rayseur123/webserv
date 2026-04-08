#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <iostream>

class Location
{
    private: 
        std::string root_;
        std::string error_page_;
        bool        enable_upload_;
        bool        autoindex_;
        int         allow_methods_;

    public:

        bool methodIsAllowed(std::string const& verb);

        std::string const&  getRoot();
        std::string const&  getErrorPage();
        bool const&         getEnableUpload();
        bool const&         getAutoIndex();
        int const&          getAllowMethods();

        Location();
        Location(std::string root, std::string error_page, bool enable_upload, bool autoindex, int allow_methods);
        Location(Location const& to_copy);
        Location const& operator=(Location const& to_copy);
        ~Location();
};

#endif