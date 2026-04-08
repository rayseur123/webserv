#include "location.hpp"
#include <iostream>

bool Location::methodIsAllowed(std::string verb)
{
    std::cout << verb;
    return 0;
}

Location::Location()
:root_(0), error_page_(0), enable_upload_(0), autoindex_(0), allow_methods_(0)
{}

Location::Location(std::string root, std::string error_page, bool enable_upload, bool autoindex, int allow_methods)
:root_(root), error_page_(error_page), enable_upload_(enable_upload), 
  autoindex_(autoindex), allow_methods_(allow_methods)
{}

Location::Location(Location const& to_copy)
:root_(to_copy.root_), error_page_(to_copy.error_page_), 
 enable_upload_(to_copy.enable_upload_),autoindex_(to_copy.autoindex_),
 allow_methods_(to_copy.allow_methods_)
{}

Location const& Location::operator=(Location const& to_copy)
{
    if (this == &to_copy)
        return *this;
    root_ = to_copy.root_;
    error_page_ = to_copy.error_page_;
    enable_upload_ = to_copy.enable_upload_;
    autoindex_ = to_copy.autoindex_;
    allow_methods_ = to_copy.allow_methods_;
    return *this;
}

Location::~Location()
{}