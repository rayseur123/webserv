#include <vector>
#include "Server.hpp"
#include "Block.hpp"
#include "Network.hpp"

int	main(int ac, char **av)
{
	try
	{
		(void)ac;
		(void)av;

		std::vector<Client> client_vec;
		std::string	buff;
		
		std::ifstream file("webserv.conf");
		Block	block(file, Block::FILE, buff, "FILE");
		
		std::vector<Server> server_vec = block.makeServerVec();
		
		Network net(server_vec, client_vec);


	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}


