
#include <iostream>

class Location
{
    private: 
        std::string root_;
        std::string error_page_;
        bool        enable_upload_;
        bool        autoindex_;
        int         allow_methods_ : 3;

    public:

        bool methodIsAllowed(std::string verb);

        Location();
        Location(std::string root, std::string error_page, bool enable_upload, bool autoindex, int allow_methods);
        Location(Location const& to_copy);
        Location const& operator=(Location const& to_copy);
        ~Location();
};