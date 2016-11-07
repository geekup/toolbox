#ifndef __SOCKET_TOOLS_H__
#define __SOCKET_TOOLS_H__

namespace toolbox
{

	int set_reuseaddr(int sockfd);
	int bind(int sockfd, unsigned short port);
	
    int connect(int sockfd, const char* ip, unsigned short port);

    int set_blocking(int fd);
    int set_nonblocking(int fd);

	bool select_for_read(int fd, int millisecs = 0);
	bool select_for_write(int fd, int millisecs = 0);

}

#endif // __SOCKET_TOOLS_H__