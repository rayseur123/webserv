#include <vector>
#include "Listener.hpp"
#include "Block.hpp"
#include "EpollManager.hpp"
#include "Request.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	try
	{
		std::string buffin = "/test HTTP/1.0\r\nHost: exemple.fr\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: 27\r\n\r\nfield1=value1&field2=value2\r\n";
    	Request test(buffin);

		std::string			buff;
		
		std::ifstream		file(av[1]);
		Block				block(file, Block::FILE, buff, "FILE");
		
		std::vector<Listener> server_vec = block.makeServerVec();
		
		EpollManager net(server_vec);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}


