#include <climits>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "http/parsing/Method.hpp"
#include "parsing/Location.hpp"

std::string
Location::buildPath(Request const& request) const
{
	std::string uri(request.getUri().getTarget());
	std::string suffix = uri.substr(path_.length());
	std::string root = root_;

	if (!suffix.empty() && suffix[0] != '/' &&
		(root.empty() || root[root.length() - 1] != '/'))
		return root + "/" + suffix;
	return root + suffix;
}

int
Location::checkAllowMethods(unsigned int actual_methods) const
{
	std::cout << "allow :" << allow_methods_ << std::endl;
	if ((allow_methods_ & actual_methods) != 0)
		return (0);
	return (1);
}

int
Location::getValue(std::string const& uri) const
{
	if (path_.size() == 1 && path_[0] == '/')
		return (1);

	if (uri == path_)
		return (INT_MAX);

	if (uri.compare(0, path_.size(), path_) != 0)
		return (0);

	if (uri.size() > path_.size() && uri[path_.size()] != '/')
		return (0);

	int depth = 0;
	for (size_t i = 0; i < path_.size(); ++i)
		if (path_[i] == '/')
			++depth;
	return (depth);
}

void
Location::setRoot(std::string const& root)
{
	if (root.empty())
	{
		root_ = ".";
		return;
	}

	std::string temp = root;
	if (temp.length() > 1 && temp[temp.length() - 1] == '/')
		temp.erase(temp.length() - 1);

	if (temp.find("./") == 0)
		root_ = temp;
	else if (temp[0] != '/')
		root_ = "/" + temp;
	else
		root_ = "./" + temp;
}

void
Location::setAllowMethods(std::vector<std::string> const& allow_methods)
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
			throw std::invalid_argument("[ERROR] : Cant find " +
										allow_methods[i] + " method.");
	}
}

void
Location::setAutoIndex(std::string const& autoindex)
{
	if (autoindex == "on")
		autoindex_ = true;
	else if (autoindex == "off")
		autoindex_ = false;
	else
		throw std::invalid_argument("[ERROR] : " + autoindex +
									" is not an autoindex mode.");
}

void
Location::setIndex(std::string const& index)
{
	std::string temp = index;

	if (index[0] != '/')
		temp = "/" + temp;

	if (temp.length() > 1 && temp[temp.length() - 1] == '/')
		temp.erase(temp.length() - 1);
	index_ = temp;
}

void
Location::setUploadStore(std::string const& upload_store)
{
	upload_store_ = upload_store;
}

void
Location::setCgiPass(std::string const& cgi_pass)
{
	cgi_pass_ = cgi_pass;
}

void
Location::setRedirect(std::string const& redirect)
{
	redirect_ = redirect;
}

void
Location::setPath(std::string const& name)
{
	path_ = name;
}

std::string const&
Location::getRoot() const
{
	return (root_);
}

bool
Location::getAutoIndex() const
{
	return (autoindex_);
}

int
Location::getAllowMethods() const
{
	return (allow_methods_);
}

std::string const&
Location::getIndex() const
{
	return (index_);
}

std::string const&
Location::getUploadStore() const
{
	return (upload_store_);
}

std::string const&
Location::getCgiPass() const
{
	return (cgi_pass_);
}

std::string const&
Location::getRedirect() const
{
	return (redirect_);
}

std::string const&
Location::getPath() const
{
	return (path_);
}

Location&
Location::operator=(Location const& to_copy)
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

std::ostream&
operator<<(std::ostream& os, Location const& to_print)
{
	os << "\t\tLOCATION " << to_print.getPath() << " {" << '\n';
	os << "\t\t\troot: " << to_print.getRoot() << '\n';
	os << "\t\t\tautoindex: " << (to_print.getAutoIndex() ? "on" : "off")
	   << '\n';

	os << "\t\t\tmethods: ";
	int m = to_print.getAllowMethods();
	if (m & GET)
		os << "GET ";
	if (m & POST)
		os << "POST ";
	if (m & DELETE)
		os << "DELETE ";
	os << '\n';

	if (!to_print.getIndex().empty())
		os << "\t\t\tindex: " << to_print.getIndex() << '\n';
	if (!to_print.getRedirect().empty())
		os << "\t\t\tredirect: " << to_print.getRedirect() << '\n';
	if (!to_print.getUploadStore().empty())
		os << "\t\t\tupload: " << to_print.getUploadStore() << '\n';
	if (!to_print.getCgiPass().empty())
		os << "\t\t\tcgi: " << to_print.getCgiPass() << '\n';

	os << "\t\t}" << '\n';
	return (os);
}

Location::Location() :
	root_("/www/data"), autoindex_(false), allow_methods_(0), path_("/")
{}

Location::Location(std::string const& root, bool autoindex, int allow_methods,
				   std::string const& index, std::string const& upload_store,
				   std::string const& cgi_pass, std::string const& redirect,
				   std::string const& path) :
	root_(root), autoindex_(autoindex), allow_methods_(allow_methods),
	index_(index), upload_store_(upload_store), cgi_pass_(cgi_pass),
	redirect_(redirect), path_(path)
{}

Location::Location(Location const& to_copy) :
	root_(to_copy.root_), autoindex_(to_copy.autoindex_),
	allow_methods_(to_copy.allow_methods_), index_(to_copy.index_),
	upload_store_(to_copy.upload_store_), cgi_pass_(to_copy.cgi_pass_),
	redirect_(to_copy.redirect_), path_(to_copy.path_)
{}

Location::~Location()
{}
