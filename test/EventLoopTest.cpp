/*
 * EventLoopTest.cpp
 *
 *  Created on: 2013-8-6
 *      Author: fanbin
 */
#include <stdio.h>
#include "../src/mythread/EventLoop.h"
#include "../src/mythread/Thread.h"
#include "../src/mythread/CurrentThread.h"

//1
//void threadFunc()
//{
//	printf("threadFunc(): pid =%d, tid = %d\n", getpid(), CurrentThread::tid());
//
//	EventLoop loop;
//	loop.loop();
//}
//
//int main()
//{
//	printf("main(): pid =%d, tid = %d\n", getpid(), CurrentThread::tid());
//
//	EventLoop loop;
//	Thread thread(threadFunc);
//	thread.start();
//
//	loop.loop();
//
//	pthread_exit(NULL);
//	return 0;
//
//}

//2
//#include <sys/timerfd.h>
//EventLoop* g_loop;
//
//void timeout()
//{
//	printf("TimeOut\n");
//	g_loop->quit();
//}
//
//int main()
//{
//	EventLoop loop;
//	g_loop = &loop;
//
//	int timerfd = :: timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
//	Channel channel(&loop, timerfd);
//	channel.setReadCallback(timeout);
//	channel.enableReading();
//
//	struct itimerspec howlong;
//	bzero(&howlong, sizeof howlong);
//	howlong.it_value.tv_sec = 5;
//	::timerfd_settime(timerfd, 0, &howlong, NULL);
//
//	loop.loop();
//
//	::close(timerfd);
//	return 0;
//}

//3

#include "../src/mynet/InetAddress.h"
#include "../src/mynet/SocketsFun.h"
#include "../src/mynet/Acceptor.h"

void newConnection(int sockfd, const InetAddress& peerAddr)
{
	printf("newConnection():accepted a new connection from %s\n",
			peerAddr.toHostPort().c_str());
	::write(sockfd, "How Are you?\n", 13);
	SocketsFun::close(sockfd);
}

int main()
{
	printf("main():pid = %d \n", getpid());

	InetAddress listenAddr(9981);
	EventLoop loop;

	Acceptor acceptor(&loop, listenAddr);
	acceptor.setNewConnectionCallback(newConnection);
	acceptor.listen();

	loop.loop();
}
