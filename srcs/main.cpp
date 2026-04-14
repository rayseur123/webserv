#include <vector>
#include "Server.hpp"
#include "Block.hpp"

int	main(int ac, char **av)
{
	try
	{
		(void)ac;
		(void)av;
		std::string	buff;

		std::ifstream file("webserv.conf");
		Block	block(file, Block::FILE, buff, "FILE");
		std::cout << block << std::endl;

		std::vector<Server> server_vec = block.makeServerVec();
		for (size_t i = 0; i < server_vec.size(); ++i)
		{
			std::cout << server_vec[i];
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	
	return (0);
}