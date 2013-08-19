/*
 * TcpServer.h
 *
 *  Created on: 2013-8-13
 *      Author: fanbin
 */

#ifndef TCPSERVER_H_
#define TCPSERVER_H_

#include "../base/NonCopyAble.h"
#include "TcpConnection.h"
#include "../mythread/EventLoop.h"
#include "Callbacks.h"
#include "Acceptor.h"
#include <string>
#include <map>

using namespace Callbacks;

class TcpServer : NonCopyAble
{
public:
	TcpServer(EventLoop* loop, const InetAddress& listenAddr);
	virtual ~TcpServer();

	void start();

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

private:
	void newConnection(int sockfd, const InetAddress& peerAddr);
	typedef std::map<std::string, TcpConnectionPtr> ConnectionMap;

	EventLoop* loop_;
	const std::string name_;
	boost::scoped_ptr<Acceptor> acceptor_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
	bool started_;
	int nextConnId_;
	ConnectionMap connections_;

};

#endif /* TCPSERVER_H_ */
