/*
 * Acceptor.cpp
 *
 *  Created on: 2013-8-12
 *      Author: fanbin
 */

#include "Acceptor.h"
#include "SocketsFun.h"

#include <boost/bind.hpp>

Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr) :
		loop_(loop), acceptSocket_(SocketsFun::createNonblockingOrDie()), acceptChannel_(
				loop, acceptSocket_.fd()), listenning_(false)
{
	acceptSocket_.setReuseAddr(true);
	acceptSocket_.bindAddress(listenAddr);
	acceptChannel_.setReadCallback(boost::bind(&Acceptor::handleRead, this));
}

Acceptor::~Acceptor()
{
}

void Acceptor::listen()
{
	loop_->assertInLoopThread();
	listenning_ = true;
	acceptSocket_.listen();
	acceptChannel_.enableReading();
}

void Acceptor::handleRead()
{
	loop_->assertInLoopThread();
	InetAddress peerAddr(0);
	int connfd = acceptSocket_.accept(&peerAddr);
	if(connfd >= 0)
	{
		if(newConnectionCallback_)
		{
			newConnectionCallback_(connfd, peerAddr);
		}
		else
		{
			SocketsFun::close(connfd);
		}
	}
}
