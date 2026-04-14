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
    for (size_t i = 1; i < allow_methods.size(); i++)
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

#include "Location.hpp"
#include <iostream>

void Location::print(int depth) const {
    std::string indent(depth, ' ');
    
    std::cout << indent << "[Location]" << std::endl;
    std::cout << indent << "  Root: " << root_ << std::endl;
    std::cout << indent << "  Autoindex: " << (autoindex_ ? "on" : "off") << std::endl;
    std::cout << indent << "  Methods: " 
              << (allow_methods_ & GET ? "GET " : "")
              << (allow_methods_ & POST ? "POST " : "")
              << (allow_methods_ & DELETE ? "DELETE " : "") << std::endl;
    if (!index_.empty())        std::cout << indent << "  Index: " << index_ << std::endl;
    if (!upload_store_.empty()) std::cout << indent << "  Upload Store: " << upload_store_ << std::endl;
    if (!cgi_pass_.empty())     std::cout << indent << "  CGI Pass: " << cgi_pass_ << std::endl;
    if (!redirect_.empty())     std::cout << indent << "  Redirect: " << redirect_ << std::endl;
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

Location::~Location()
{}