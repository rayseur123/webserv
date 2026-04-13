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
		Block	block(file, Block::SERVER, buff, "FILE");
		//block.print();
		//std::cout << "print 2 : " << std::endl << std::endl << std::endl;
		//block.print2();

		std::vector<Server> server_vec = block.makeServerVec();
		for (size_t i = 0; i < server_vec.size(); i++)
		{
			server_vec[i].print();
		}
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	
	
	return (0);
}