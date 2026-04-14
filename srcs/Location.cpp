#include "Location.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>

void    Location::setRoot(std::string const& root)
{
    root_ = root;
}

void    Location::setAllowMethods(std::vector<std::string> const& allow_methods)
{
    for (size_t i = 1; i < allow_methods.size(); ++i)
    {
        if (allow_methods[i] == "GET")
            allow_methods_ += GET;
        else if (allow_methods[i] == "POST")
            allow_methods_ += POST;
        else if (allow_methods[i] == "DELETE")
            allow_methods_ += DELETE;
        else
            throw std::invalid_argument("[ERROR] : Cant find " + allow_methods[i] + " method.");
    }
}

void    Location::setAutoIndex(std::string const& autoindex)
{
    if (autoindex == "on")
        autoindex_ = true;
    else if (autoindex == "off")
        autoindex_ = false;
    else
        throw std::invalid_argument("[ERROR] : " + autoindex + " is not an autoindex mode.");
}

void    Location::setIndex(std::string const& index)
{
    index_ = index;
}

void    Location::setUploadStore(std::string const& upload_store)
{
    upload_store_ = upload_store;
}

void    Location::setCgiPass(std::string const& cgi_pass)
{
    cgi_pass_ = cgi_pass;
}

void    Location::setRedirect(std::string const& redirect)
{
    redirect_ = redirect;
}

Location const& Location::operator=(Location const& to_copy)
{
    if (this == &to_copy)
        return (*this);
    root_ = to_copy.root_;
    autoindex_ = to_copy.autoindex_;
    allow_methods_ = to_copy.allow_methods_;
    index_ = to_copy.index_;
    upload_store_ = to_copy.upload_store_;
    cgi_pass_ = to_copy.cgi_pass_;
    redirect_ = to_copy.redirect_;
    return (*this);
}

std::ostream&   operator<<(std::ostream& os, Location const& to_print)
{
    (void)to_print;
    return (os);
}

Location::Location()
    :root_("/www/data"),
    autoindex_(false),
    allow_methods_(0),
    index_("index.html")
{}

Location::Location(std::string const& root, bool autoindex, int allow_methods, std::string const& index,
                std::string const& upload_store, std::string const& cgi_pass, std::string const& redirect)
    :root_(root),
    autoindex_(autoindex), 
    allow_methods_(allow_methods),
    index_(index),
    upload_store_(upload_store),
    cgi_pass_(cgi_pass),
    redirect_(redirect)
{}

Location::Location(Location const& to_copy)
    :root_(to_copy.root_),
    autoindex_(to_copy.autoindex_),
    allow_methods_(to_copy.allow_methods_),
    index_(to_copy.index_),
    upload_store_(to_copy.upload_store_),
    cgi_pass_(to_copy.cgi_pass_),
    redirect_(to_copy.redirect_)
{}

Location::~Location()
{}