#ifndef ASOCKET_HPP
#define ASOCKET_HPP

class EpollManager;

class	ASocket
{
	protected:
		int	fd_;

	public:
		int				getFd() const;
		virtual int		handleEvent(EpollManager& manager, int events) = 0;

		ASocket();
		ASocket(int fd);
		virtual ~ASocket();
};

#endif
