/*
 * TcpServer.cpp
 *
 *  Created on: 2013-8-13
 *      Author: fanbin
 */

#include "TcpServer.h"

#include <boost/bind.hpp>

TcpServer::TcpServer(EventLoop* loop, const InetAddress& listenAddr) :
		loop_(loop),
		acceptor_(new Acceptor(loop, listenAddr)),
		started_(false),
		nextConnId_(1)
{
	acceptor_->setNewConnectionCallback(
			boost::bind(&TcpServer::newConnection, this, _1, _2));
}

TcpServer::~TcpServer()
{
}

void TcpServer::newConnection(int sockfd, const InetAddress & peerAddr)
{
	loop_->assertInLoopThread();
	char buf[32];
	snprintf(buf, sizeof buf, "#%d", nextConnId_);
	++nextConnId_;
	std::string connName = name_ + buf;

	printf("Inof, TcpServer::newConnection[%s]-new connection[%s] form %s \n",
			name_.c_str(), connName.c_str(), peerAddr.toHostPort().c_str());

	InetAddress localAddr(SocketsFun::getLocalAddr(sockfd));

	TcpConnectionPtr conn(
			new TcpConnection(loop_, connName, sockfd, localAddr, peerAddr));

	connections_[connName] = conn;
	conn->setConnectionCallback(connectionCallback_);
	conn->setMessageCallback(messageCallback_);
	conn->connectEstablished();
}

