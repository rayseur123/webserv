#include <vector>
#include "Server.hpp"
#include "Block.hpp"
#include "ServerManager.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	try
	{
		std::vector<Connection> client_vec;
		std::string			buff;
		
		std::ifstream		file(av[1]);
		Block				block(file, Block::FILE, buff, "FILE");
		
		std::vector<Server> server_vec = block.makeServerVec();
		
		ServerManager net(server_vec, client_vec);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}


