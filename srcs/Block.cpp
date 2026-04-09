#include "Block.hpp"
#include <iostream>

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
    }
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
	
Block::Block(Block const& to_copy):
type_(to_copy.type_), blocks_vec_(to_copy.blocks_vec_), directivs_vec_(to_copy.directivs_vec_), name_(to_copy.name_)
{}

Block::~Block()
{}