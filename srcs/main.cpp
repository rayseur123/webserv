#include <vector>
#include "Server.hpp"
#include "Block.hpp"

int	main(int ac, char **av)
{
	(void)ac;
	(void)av;
	std::string	buff;

	std::ifstream file("webserv.conf");
	Block	block(file, Block::SERVER, buff, "FILE");
	//block.print();
	//std::cout << "print 2 : " << std::endl << std::endl << std::endl;
	block.print2();
	return (0);
}