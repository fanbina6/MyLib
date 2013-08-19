/*
 * SocketsFun.cpp
 *
 *  Created on: 2013-8-12
 *      Author: fanbin
 */

#include "SocketsFun.h"

#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>  // snprintf
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <strings.h>  // bzero
typedef struct sockaddr SA;

const SA* sockaddr_cast(const struct sockaddr_in* addr)
{
	return static_cast<const SA*>((const void*) addr);
}

SA* sockaddr_cast(struct sockaddr_in* addr)
{
	return static_cast<SA*>((void*) (addr));
}

int SocketsFun::createNonblockingOrDie()
{
	// socket
#if VALGRIND
	int sockfd = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sockfd < 0)
	{
		LOG_SYSFATAL << "sockets::createNonblockingOrDie";
	}

	setNonBlockAndCloseOnExec(sockfd);
#else
	int sockfd = ::socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC,
			IPPROTO_TCP);
	if (sockfd < 0)
	{
		printf("error sockets::createNonblockingOrDie");
	}
#endif
	return sockfd;
}

void SocketsFun::bindOrDie(int sockfd, const struct sockaddr_in& addr)
{
	int ret = ::bind(sockfd, sockaddr_cast(&addr),
			static_cast<socklen_t>(sizeof addr));
	if (ret < 0)
	{
		printf("sockets::bindOrDie\n");
	}
}

void SocketsFun::listenOrDie(int sockfd)
{
	int ret = ::listen(sockfd, SOMAXCONN);
	if (ret < 0)
	{
		printf("sockets::listenOrDie");
	}
}

int SocketsFun::accept(int sockfd, struct sockaddr_in* addr)
{
	socklen_t addrlen = static_cast<socklen_t>(sizeof *addr);
#if VALGRIND
	int connfd = ::accept(sockfd, sockaddr_cast(addr), &addrlen);
	setNonBlockAndCloseOnExec(connfd);
#else
	int connfd = ::accept4(sockfd, sockaddr_cast(addr), &addrlen,
			SOCK_NONBLOCK | SOCK_CLOEXEC);
#endif
	if (connfd < 0)
	{
		int savedErrno = errno;
		printf("error Socket::accept\n");
		switch (savedErrno)
		{
		case EAGAIN:
		case ECONNABORTED:
		case EINTR:
		case EPROTO: // ???
		case EPERM:
		case EMFILE: // per-process lmit of open file desctiptor ???
			// expected errors
			errno = savedErrno;
			break;
		case EBADF:
		case EFAULT:
		case EINVAL:
		case ENFILE:
		case ENOBUFS:
		case ENOMEM:
		case ENOTSOCK:
		case EOPNOTSUPP:
			// unexpected errors
			printf("unexpected error of ::accept %d\n", savedErrno);
			break;
		default:
			printf("unknown error of ::accept %d\n", savedErrno);
			break;
		}
	}
	return connfd;
}

ssize_t SocketsFun::read(int sockfd, void *buf, size_t count)
{
	return ::read(sockfd, buf, count);
}

ssize_t SocketsFun::readv(int sockfd, const struct iovec *iov, int iovcnt)
{
	return ::readv(sockfd, iov, iovcnt);
}

ssize_t SocketsFun::write(int sockfd, const void *buf, size_t count)
{
	return ::write(sockfd, buf, count);
}

void SocketsFun::close(int sockfd)
{
	if (::close(sockfd) < 0)
	{
		printf("sockets::close\n");
	}
}

void SocketsFun::shutdownWrite(int sockfd)
{
	if (::shutdown(sockfd, SHUT_WR) < 0)
	{
		printf("sockets::shutdownWrite\n");
	}
}

void SocketsFun::fromIpPort(const char* ip, uint16_t port,
		struct sockaddr_in* addr)
{
	addr->sin_family = AF_INET;
	addr->sin_port = hostToNetwork16(port);
	if (::inet_pton(AF_INET, ip, &addr->sin_addr) <= 0)
	{
		printf("sockets::fromIpPort\n");
	}
}

void SocketsFun::toIpPort(char* buf, size_t size,
		const struct sockaddr_in& addr)
{
	char host[INET_ADDRSTRLEN] = "INVALID";
	toIp(host, sizeof host, addr);
	uint16_t port = SocketsFun::networkToHost16(addr.sin_port);
	snprintf(buf, size, "%s:%u", host, port);
}

void SocketsFun::toIp(char* buf, size_t size, const struct sockaddr_in& addr)
{
	assert(size >= INET_ADDRSTRLEN);
	::inet_ntop(AF_INET, &addr.sin_addr, buf, static_cast<socklen_t>(size));
}

struct sockaddr_in SocketsFun::getLocalAddr(int sockfd)
{
	struct sockaddr_in localaddr;
	bzero(&localaddr, sizeof localaddr);
	socklen_t addrlen = static_cast<socklen_t>(sizeof localaddr);
	if (::getsockname(sockfd, sockaddr_cast(&localaddr), &addrlen) < 0)
	{
		printf("error, sockets::getLocalAddr\n");
	}
	return localaddr;
}
