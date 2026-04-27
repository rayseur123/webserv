#include "Location.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "Method.hpp"

static std::string getNextWord(std::string const& str)
{
    return (str.substr(0, str.find_first_of('/')));
}

int     Location::getValue(std::string const& uri) const
{
    int value = 1;
    std::string uri_temp = uri;
    std::string path_temp = this->path_;

    if (uri_temp == path_temp)
        return (-1);
    while (1)
    {
        if (getNextWord(uri_temp) == getNextWord(path_temp))
            value++;
        else
        {
            value--;
            break;
        }
        uri_temp = uri_temp.substr(uri_temp.find_first_of('/') + 1);
        path_temp = path_temp.substr(path_temp.find_first_of('/') + 1);
    }
    return (value);
}

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

void    Location::setPath(std::string const& name)
{
    path_ = name;
}

std::string const&  Location::getRoot() const
{
    return (root_);
}

bool    Location::getAutoIndex() const
{
    return (autoindex_);
}

int Location::getAllowMethods() const
{
    return (allow_methods_);
}

std::string const&  Location::getIndex() const
{
    return (index_);
}

std::string const&  Location::getUploadStore() const
{
    return (upload_store_);
}

std::string const& Location::getCgiPass() const
{
    return (cgi_pass_);
}

std::string const&  Location::getRedirect() const
{
    return (redirect_);
}

std::string const& Location::getPath() const
{
    return (path_);
}

Location& Location::operator=(Location const& to_copy)
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
    path_ = to_copy.path_;
    return (*this);
}

std::ostream& operator<<(std::ostream& os, Location const& to_print)
{
    os << "\t\tLOCATION "<< to_print.getPath() << " {" << std::endl;
    os << "\t\t\troot: " << to_print.getRoot() << std::endl;
    os << "\t\t\tautoindex: " << (to_print.getAutoIndex() ? "on" : "off") << std::endl;
    
    os << "\t\t\tmethods: ";
    int m = to_print.getAllowMethods();
    if (m & GET)
        os << "GET ";
    if (m & POST)
        os << "POST ";
    if (m & DELETE)
        os << "DELETE ";
    os << std::endl;

    if (!to_print.getIndex().empty())
        os << "\t\t\tindex: " << to_print.getIndex() << std::endl;
    if (!to_print.getRedirect().empty())
        os << "\t\t\tredirect: " << to_print.getRedirect() << std::endl;
    if (!to_print.getUploadStore().empty())
        os << "\t\t\tupload: " << to_print.getUploadStore() << std::endl;
    if (!to_print.getCgiPass().empty())
        os << "\t\t\tcgi: " << to_print.getCgiPass() << std::endl;
    
    os << "\t\t}" << std::endl;
    return (os);
}

Location::Location()
    :root_("/www/data"),
    autoindex_(false),
    allow_methods_(0),
    index_("index.html"),
    path_("/")
{}

Location::Location(std::string const& root, bool autoindex, int allow_methods, std::string const& index,
                std::string const& upload_store, std::string const& cgi_pass, std::string const& redirect,
                std::string const& path)
    :root_(root),
    autoindex_(autoindex), 
    allow_methods_(allow_methods),
    index_(index),
    upload_store_(upload_store),
    cgi_pass_(cgi_pass),
    redirect_(redirect),
    path_(path)
{}

Location::Location(Location const& to_copy)
    :root_(to_copy.root_),
    autoindex_(to_copy.autoindex_),
    allow_methods_(to_copy.allow_methods_),
    index_(to_copy.index_),
    upload_store_(to_copy.upload_store_),
    cgi_pass_(to_copy.cgi_pass_),
    redirect_(to_copy.redirect_),
    path_(to_copy.path_)
{}

Location::~Location()
{}