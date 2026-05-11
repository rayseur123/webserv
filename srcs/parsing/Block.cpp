#include "parsing/Block.hpp"
#include "socket/Listener.hpp"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

namespace
{
	std::vector<std::string>
	splitDirective(std::string const& directive)
	{
		std::vector<std::string> params_vec;
		std::stringstream		 ss(directive);
		std::string				 word;

		while ((ss >> word) != NULL)
			params_vec.push_back(word);
		return (params_vec);
	}

	bool
	getNextToken(std::ifstream& file, std::string& buff, std::string& content,
				 char& sep_char)
	{
		std::string line;

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

			std::string token = line.substr(start);
			size_t		sep = token.find_first_of("{};");

			if (sep == std::string::npos)
				throw std::runtime_error("[ERROR] : Syntax error: line '" +
										 token +
										 "' is missing a ';' separator.");

			sep_char = token[sep];
			content = token.substr(0, sep);
			size_t end = content.find_last_not_of(" \t\r\n");
			content =
				(end != std::string::npos) ? content.substr(0, end + 1) : "";
			buff = token.substr(sep + 1);
			return (true);
		}
		return (false);
	}

	std::string
	getBlockNameByType(int type)
	{
		if (type == Block::FILE)
			return ("FILE");
		if (type == Block::SERVER)
			return ("SERVER");
		if (type == Block::LOCATION)
			return ("LOCATION");
		return ("ERROR_NAME");
	}
} // namespace

Location
Block::makeLocation() const
{
	Location								 loc;
	std::vector<std::string>				 directives_split;
	std::vector<std::string>::const_iterator it;

	for (it = directives_vec_.begin(); it != directives_vec_.end(); ++it)
	{
		directives_split = splitDirective(*it);
		if (directives_split.size() < 2)
			throw std::invalid_argument("[ERROR] : Invalide directive. : " +
										directives_split[0]);
		if (directives_split[0] == "root")
			loc.setRoot(directives_split[1]);
		else if (directives_split[0] == "allow_methods:")
			loc.setAllowMethods(directives_split);
		else if (directives_split[0] == "autoindex")
			loc.setAutoIndex(directives_split[1]);
		else if (directives_split[0] == "index")
			loc.setIndex(directives_split[1]);
		else if (directives_split[0] == "upload_store")
			loc.setUploadStore(directives_split[1]);
		else if (directives_split[0] == "cgi_pass")
			loc.setCgiPass(directives_split[1]);
		else if (directives_split[0] == "redirect")
			loc.setRedirect(directives_split[1]);
		else
			throw std::invalid_argument("[ERROR] : Invalide directive." +
										directives_split[0]);
	}
	if (name_.find("location ") != std::string::npos)
		loc.setPath(name_.substr(LOC_SIZE));
	else
		loc.setPath(name_);
	return (loc);
}

std::vector<Location>
Block::makeLocationVec() const
{
	std::vector<Location>			   loc_vec;
	std::vector<Block>::const_iterator it;

	loc_vec.reserve(blocks_vec_.size());
	for (it = blocks_vec_.begin(); it != blocks_vec_.end(); ++it)
		loc_vec.push_back(it->makeLocation());
	return (loc_vec);
}

Listener*
Block::makeServer() const
{
	Listener*								 serv = new Listener;
	std::vector<std::string>				 directives_split;
	std::vector<std::string>::const_iterator it;

	serv->setLocations(makeLocationVec());
	for (it = directives_vec_.begin(); it != directives_vec_.end(); ++it)
	{
		directives_split = splitDirective(*it);
		if (directives_split.size() < 2)
		{
			delete serv;
			throw std::invalid_argument(
				"[ERROR] : A directive need parameter(s).");
		}
		if (directives_split[0] == "client_max_body_size")
			serv->setMaxClientRequestBody(directives_split[1]);
		else if (directives_split[0] == "listen")
			serv->setAddrAndPort(directives_split[1]);
		else if (directives_split[0] == "error_page")
		{
			if (!serv->setErrorPage(directives_split))
			{
				delete serv;
				throw std::invalid_argument("[ERROR] : Invalide directive.");
			}
		}
		else
		{
			delete serv;
			throw std::invalid_argument("[ERROR] : Invalide directive.");
		}
	}

	return (serv);
}

std::vector<Listener*>
Block::makeServerVec() const
{
	std::vector<Listener*>			   server_vec;
	std::vector<Block>::const_iterator it;

	server_vec.reserve(blocks_vec_.size());
	for (it = blocks_vec_.begin(); it != blocks_vec_.end(); ++it)
		server_vec.push_back(it->makeServer());
	return (server_vec);
}

int
Block::getType() const
{
	return (type_);
}

std::vector<Block> const&
Block::getBlocks() const
{
	return (blocks_vec_);
}

std::vector<std::string> const&
Block::getDirectives() const
{
	return (directives_vec_);
}

std::string const&
Block::getName() const
{
	return (name_);
}

bool
Block::parseToken(std::ifstream& file, std::string& buff,
				  std::string const& content, char sep_char)
{
	if (sep_char == ';')
	{
		if (!content.empty())
			directives_vec_.push_back(content);
	}
	else if (sep_char == '{')
		blocks_vec_.push_back(Block(file, type_ + 1, buff, content));
	else if (sep_char == '}')
		return (true);
	return (false);
}

Block::Block(std::ifstream& file, int type, std::string& buff,
			 std::string const& name) : type_(type), name_(name)
{
	std::string content;
	char		sep_char = 0;

	while (getNextToken(file, buff, content, sep_char))
		if (parseToken(file, buff, content, sep_char))
			return;

	if (type != FILE)
		throw std::runtime_error("[ERROR] : Syntax error: unclosed block '" +
								 name_ + "' at end of file.");
}

Block&
Block::operator=(Block const& to_copy)
{
	if (&to_copy == this)
		return (*this);
	blocks_vec_ = to_copy.blocks_vec_;
	directives_vec_ = to_copy.directives_vec_;
	type_ = to_copy.type_;
	name_ = to_copy.name_;
	return (*this);
}

std::ostream&
operator<<(std::ostream& os, Block const& to_print)
{
	static int depth = 0;

	std::string indent(depth, '\t');
	std::string name = getBlockNameByType(to_print.getType());
	std::vector<std::string> const& directives = to_print.getDirectives();
	std::vector<Block> const&		blocks = to_print.getBlocks();
	std::vector<std::string>::const_iterator it_directive;
	std::vector<Block>::const_iterator		 it_block;

	os << indent << "\033[1;34m" << name << "\033[0m {" << '\n';

	for (it_directive = directives.begin(); it_directive != directives.end();
		 ++it_directive)
		os << indent << "\t" << *it_directive << '\n';

	depth++;
	for (it_block = blocks.begin(); it_block != blocks.end(); ++it_block)
		os << *it_block;
	depth--;
	os << indent << "}" << '\n';
	return os;
}

Block::Block() : type_(0)
{}

Block::Block(Block const& to_copy) :
	type_(to_copy.type_), blocks_vec_(to_copy.blocks_vec_),
	directives_vec_(to_copy.directives_vec_), name_(to_copy.name_)
{}

Block::~Block()
{}
