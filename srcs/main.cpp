#include <vector>
#include "Listener.hpp"
#include "Block.hpp"
#include "EpollManager.hpp"
#include "Error.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (1);
	try
	{

		std::string			buff;
		
		std::ifstream		file(av[1]);
		Block				block(file, Block::FILE, buff, "FILE");
		
		std::vector<Listener> server_vec = block.makeServerVec();
		
		EpollManager net(server_vec);
	}
	catch (const Error::ErrorException &e)
	{
		std::cerr << e.get_code();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	return (0);
}


