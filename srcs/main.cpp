#include <vector>
#include "Server.hpp"
#include "Block.hpp"
#include "Network.hpp"
#include "Request.hpp"

int	main(int ac, char **av)
{
	try
	{
		(void)ac;
		(void)av;

		std::string buffin = " /test HTTP/1.0\r\nHost: exemple.fr\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nfield1=value1&field2=value2\r\n";
    	Request test(buffin);

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


