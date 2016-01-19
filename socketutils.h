#ifndef __SOCKET_TOOLS_H__
#define __SOCKET_TOOLS_H__

#ifdef _WIN32
#include "wsockutils.h"
#endif

namespace toolbox
{
	void print_bytes(unsigned char* buf, int len);

	int set_reuseaddr(int sockfd);

	//const int PEEK_RESULT_ERROR = 1;
	//const int PEEK_RESULT_TIMEOUT = 1 << 1;
	//const int PEEK_RESULT_READ_AVAILABLE = 1 << 2;
	//const int PEEK_RESULT_WRITE_AVAILABLE = 1 << 3;
	//int peek(int sockfd, int timeout);

	bool select_for_read(int fd, int millisecs = 0);
	bool select_for_write(int fd, int millisecs = 0);
}

#endif // __SOCKET_TOOLS_H__