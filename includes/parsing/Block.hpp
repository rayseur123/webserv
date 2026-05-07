#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <fstream>
#include <string>
#include <vector>

#include "socket/Listener.hpp"

#define LOC_SIZE 9

class Block
{
private:
	int						 type_;
	std::vector<Block>		 blocks_vec_;
	std::vector<std::string> directives_vec_;
	std::string				 name_;

public:
	Location			   makeLocation() const;
	std::vector<Listener*> makeServerVec() const;
	std::vector<Location>  makeLocationVec() const;
	Listener*			   makeServer() const;

	int								getType() const;
	std::vector<Block> const&		getBlocks() const;
	std::vector<std::string> const& getDirectives() const;
	std::string const&				getName() const;

	bool parseToken(std::ifstream& file, std::string& buff,
					std::string const& content, char sep_char);

	Block& operator=(Block const& to_copy);

	Block();
	Block(std::ifstream& file, int type, std::string& buff,
		  std::string const& name);
	Block(Block const& to_copy);
	~Block();

	enum Type
	{
		FILE,
		SERVER,
		LOCATION,
	};
};

std::ostream& operator<<(std::ostream& os, Block const& to_print);

#endif
