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

		Network net;
		std::vector<Client> clients;
		std::string	buff;

		std::ifstream file("webserv.conf");
		Block	block(file, Block::FILE, buff, "FILE");

		std::vector<Server> server_vec = block.makeServerVec();

		for (size_t i = 0; i < server_vec.size(); i++)
		{
			server_vec[i].createSocket();
		}

		net.setClients(clients);
		net.setServers(server_vec);
		
		net.instanceEpoll();
		net.addingServers();
		net.manageNetwork();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}


