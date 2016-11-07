#include "socketutil.h"
#include <stdio.h>

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <fcntl.h>
#endif

namespace toolbox
{

    int set_reuseaddr(int sockfd)
    {
        int opt = 1;
#ifdef _WIN32
        const char* opt_val = (const char*)&opt;
#else
        const void* opt_val = (const void*)&opt;
#endif
        return setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, opt_val, sizeof(opt));
    }

    int bind(int sockfd, unsigned short port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        inet_aton("0.0.0.0", &addr.sin_addr);
        addr.sin_port = htons(port);

        return ::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    }

    int connect(int sockfd, const char* ip, unsigned short port)
    {
        struct sockaddr_in addr;
        addr.sin_family = AF_INET;
        inet_aton(ip, &addr.sin_addr);
        addr.sin_port = htons(port);

        return ::connect(sockfd, (const struct sockaddr*)&addr, sizeof(addr));
    }

    static bool select_rw(int fd, int millisecs, char rw)
    {
        timeval tv;
        tv.tv_sec = millisecs / 1000;
        tv.tv_usec = (millisecs % 1000) * 1000;

        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(fd, &fds);

        int r = -1;
        if (rw == 'r')
        {
            r = select(fd + 1, &fds, NULL, NULL, &tv);
        }
        else if (rw == 'w')
        {
            r = select(fd + 1, NULL, &fds, NULL, &tv);
        }
        if (r == -1 || r == 0)
        {
            return false;
        }
        else
        {
            r = FD_ISSET(fd, &fds);
            if (r)
            {
                return true;
            }
        }
        return false;
    }

    bool select_for_read(int fd, int millisecs)
    {
        return select_rw(fd, millisecs, 'r');
    }

    bool select_for_write(int fd, int millisecs)
    {
        return select_rw(fd, millisecs, 'w');
    }
    
    int set_blocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        flags &= (~(O_NONBLOCK));
        return fcntl(fd, F_SETFL, flags);
    }

    int set_nonblocking(int fd)
    {
        int flags = fcntl(fd, F_GETFL, 0);
        flags |= O_NONBLOCK;
        return fcntl(fd, F_SETFL, flags);
    }

#ifdef _WIN32
    int winsock_init()
    {
        WSADATA wsaData;
        WORD version = MAKEWORD(2, 0);
        return WSAStartup(version, &wsaData);
    }

    int winsock_cleanup()
    {
        return WSACleanup();
    }

    int inet_aton(const char* ip, in_addr* buf)
    {
        return inet_pton(AF_INET, ip, buf); // inet_pton is in Ws2tcpip.h
    }

    int close(int sockfd)
    {
        return closesocket(sockfd);
    }

    int set_blocking(int fd)
    {
        u_long argp = 0;
        ioctlsocket(*ps, FIONBIO, &argp);
    }

    int set_nonblocking(int fd)
    {
        u_long argp = 1;
        ioctlsocket(*ps, FIONBIO, &argp);
    }
#endif

}
