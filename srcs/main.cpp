#include <Server.hpp>

int	main(int ac, char **av)
{
	std::vector<Location> local;
	Server test(50, "8080", "0.0.0.0", local);

	test.createSocket();
	(void)ac;
	(void)av;
	return (0);
}