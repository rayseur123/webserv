#include <Server.hpp>
#include <Network.hpp>

int	main(int ac, char **av)
{
	
	std::vector <Client> client_vec;	
	std::vector <Server> server_vec;
	
	std::vector<Location> local;
	std::string port = "8080";
	std::string adress = "0.0.0.0";

	Server test(50, port, adress, local);

	Server test1(50, "4242", adress, local);

	

	test.createSocket();
	test1.createSocket();

	server_vec.push_back(test);
	server_vec.push_back(test1);

	Network net;

	net.setClients(client_vec);
	net.setServers(server_vec);
	
	net.instanceEpoll();
	net.addingServers();
	net.manageNetwork();

	(void)ac;
	(void)av;
	return (0);
}


