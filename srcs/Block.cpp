#include "Block.hpp"
#include <iostream>
#include "Server.hpp"

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <utility>

static std::vector<std::string> splitDirectiv(std::string const& directiv)
{
    std::vector<std::string> params_vec;
    std::stringstream ss(directiv);
    std::string word;

    while (ss >> word)
        params_vec.push_back(word);
    return (params_vec);
}

Location    Block::makeLocation()
{
    Location                    loc;
    std::vector<std::string>    directivs_split;

    for (size_t i = 0; i < directivs_vec_.size(); i++)
    {
        directivs_split = splitDirectiv(directivs_vec_[i]);
        if (directivs_split.size() < 2)
            throw std::invalid_argument("Invalide directiv. : " + directivs_split[0]);
        else if (directivs_split[0] == "root")
            loc.setRoot(directivs_split[1]);
        else if (directivs_split[0] == "allow_methods:")
            loc.setAllowMethods(directivs_split);
        else if (directivs_split[0] == "autoindex")
            loc.setAutoIndex(directivs_split[1]);
        else if (directivs_split[0] == "index")
            loc.setIndex(directivs_split[1]);
        else if (directivs_split[0] == "upload_store")
            loc.setUploadStore(directivs_split[1]);
        else if (directivs_split[0] == "cgi_pass")
            loc.setCgiPass(directivs_split[1]);
        else if (directivs_split[0] == "redirect")
            loc.setRedirect(directivs_split[1]);
        else
            throw std::invalid_argument("Invalide directiv." + directivs_split[0]);
    }
    return (loc);
}

std::vector<Location>   Block::makeLocationVec()
{
    std::vector<Location> loc_vec;

    for (size_t i = 0; i < blocks_vec_.size(); i++)
        loc_vec.push_back(blocks_vec_[i].makeLocation());
    return (loc_vec);
}

Server  Block::makeServer()
{
    Server  serv;
    std::vector<std::string>    directivs_split;
    serv.setLocations(makeLocationVec());
    for (size_t i = 0; i < directivs_vec_.size(); i++)
    {
        directivs_split = splitDirectiv(directivs_vec_[i]);
        if (directivs_split.size() < 2)
            throw std::invalid_argument("Invalide directiv.");
        else if (directivs_split[0] == "client_max_body_size")
            serv.setMaxClientRequestBody(directivs_split[1]);
        else if (directivs_split[0] == "listen")
            serv.setAddrAndPort(directivs_split[1]);
        else if (directivs_split[0] == "error_page")
            serv.setErrorPage(directivs_split);
        else
            throw std::invalid_argument("Invalide directiv.");
    }
    return (serv);
}

std::vector<Server> Block::makeServerVec()
{
    std::vector<Server> server_vec;
    for (size_t i = 0; i < blocks_vec_.size(); i++)
        server_vec.push_back(blocks_vec_[i].makeServer());
    return (server_vec);
}

std::vector<std::string> const&	Block::getDirectivs() const
{
    return (directivs_vec_);
}

Block::Block(std::ifstream &file, int type, std::string& buff, std::string const& name):
type_(type), name_(name)
{
    std::string line;
    std::string my_buff;
    
    while (file.good() || !buff.empty())
    {
        if (!buff.empty())
		{
			line = buff;
			buff.clear();
		}
        else
			std::getline(file, line);

        size_t start = line.find_first_not_of(" \t\r\n");
        if (start == std::string::npos)
			continue;
        
        my_buff = line.substr(start);
        size_t separator = my_buff.find_first_of("{};");

        if (separator != std::string::npos)
        {
            char sep_char = my_buff[separator];
            std::string content = my_buff.substr(0, separator);
            
            size_t end = content.find_last_not_of(" \t\r\n");
            if (end != std::string::npos)
				content = content.substr(0, end + 1);

            if (sep_char == ';')
			{
                if (!content.empty())
					directivs_vec_.push_back(content);
                buff = my_buff.substr(separator + 1);
            }
            else if (sep_char == '{')
			{
                buff = my_buff.substr(separator + 1);
                blocks_vec_.push_back(Block(file, type_ + 1, buff, content));
            }
            else if (sep_char == '}')
			{
                buff = my_buff.substr(separator + 1);
                return;
            }
        }
        else if (!my_buff.empty())
            throw std::runtime_error("Syntax error: line '" + my_buff + "' is missing a ';' separator.");
    }
    throw std::runtime_error("Syntax error: unclosed block '" + name_ + "' at end of file.");
}

void Block::print(int depth) const
{
    std::string indent(depth * 4, ' ');
    std::string indent2((depth + 1) * 4, ' ');

    if (type_ == SERVER)
        std::cout << indent << "server {" << std::endl;
    else
        std::cout << indent << "location {" << std::endl;

    for (size_t i = 0; i < directivs_vec_.size(); i++)
        std::cout << indent2 << directivs_vec_[i] << ";" << std::endl;

    for (size_t i = 0; i < blocks_vec_.size(); i++)
        blocks_vec_[i].print(depth + 1);

    std::cout << indent << "}" << std::endl;
}
	
void Block::print2(int depth) const
{
	std::string indent(depth * 4, ' ');
    std::string indent2((depth + 1) * 4, ' ');

	std::cout << indent << "Name : " << name_ << std::endl;
    for (size_t i = 0; i < directivs_vec_.size(); i++)
        std::cout << indent2 << directivs_vec_[i] << std::endl;

    for (size_t i = 0; i < blocks_vec_.size(); i++)
        blocks_vec_[i].print2(depth + 1);
}
	
Block const&	Block::operator=(Block const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	blocks_vec_ = to_copy.blocks_vec_;
	directivs_vec_ = to_copy.directivs_vec_;
	type_ = to_copy.type_;
	name_ = to_copy.name_;
	return (*this);
}

Block::Block()
{}

Block::Block(Block const& to_copy)
    :type_(to_copy.type_),
    blocks_vec_(to_copy.blocks_vec_),
    directivs_vec_(to_copy.directivs_vec_),
    name_(to_copy.name_)
{}

Block::~Block()
{}