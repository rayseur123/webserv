#ifndef BLOCK_HPP
#define BLOCK_HPP

#include <vector>
#include <string>
#include <fstream>

class Block
{
	private:
		int							type_;
		std::vector<Block>			blocks_vec_;
		std::vector<std::string>	directivs_vec_;
		std::string					name_;
	public:
		void    		print(int depth = 0) const;
		void 			print2(int depth = 0) const;
		Block const&	operator=(Block const& to_copy);

		Block();
		Block(std::ifstream &file, int type, std::string& buff, std::string const& name);
		Block(Block const& to_copy);
		~Block();

		enum Type
		{
			SERVER,
			LOCATION,
			FILES
		};
};

#endif