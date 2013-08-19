/*
 * TcpConnection.h
 *
 *  Created on: 2013-8-13
 *      Author: fanbin
 */

#ifndef TCPCONNECTION_H_
#define TCPCONNECTION_H_

#include "../base/NonCopyAble.h"
#include "../mythread/EventLoop.h"
#include "InetAddress.h"
#include "Callbacks.h"
#include "Socket.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>
using std::string;

using namespace Callbacks;

class TcpConnection: NonCopyAble, public boost::enable_shared_from_this<
		TcpConnection>
{
public:
	TcpConnection(EventLoop* loop, const string& name, int sockfd,
			const InetAddress& localAddr, const InetAddress& peerAddr);
	virtual ~TcpConnection();

	void setConnectionCallback(const ConnectionCallback& cb)
	{
		connectionCallback_ = cb;
	}

	void setMessageCallback(const MessageCallback& cb)
	{
		messageCallback_ = cb;
	}

	// called when TcpServer accepts a new connection
	void connectEstablished(); // should be called only once

private:
	enum StateE
	{
		kConnecting, kConnected
	};

	void setState(StateE s)
	{
		state_ = s;
	}

	void handleRead();

	EventLoop* loop_;
	std::string name_;
	StateE state_;
	boost::scoped_ptr<Socket> socket_;
	boost::scoped_ptr<Channel> channel_;
	InetAddress localAddr_;
	InetAddress peerAddr_;
	ConnectionCallback connectionCallback_;
	MessageCallback messageCallback_;
};

#endif /* TCPCONNECTION_H_ */
