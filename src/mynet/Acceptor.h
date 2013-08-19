/*
 * Acceptor.h
 *
 *  Created on: 2013-8-12
 *      Author: fanbin
 */

#ifndef ACCEPTOR_H_
#define ACCEPTOR_H_

#include "../base/NonCopyAble.h"
#include "InetAddress.h"
#include "Socket.h"
#include "../mythread/EventLoop.h"

#include <boost/function.hpp>



class Acceptor : NonCopyAble
{
public:
	typedef boost::function<void(int sockfd, const InetAddress&)> NewConnectionCallback;

	Acceptor(EventLoop* loop, const InetAddress& listenAddr);
	virtual ~Acceptor();

	void setNewConnectionCallback(const NewConnectionCallback& cb)
	{
		newConnectionCallback_ = cb;
	}

	bool listending() const
	{
		return listenning_;
	}

	void listen();

private:

	void handleRead();

	EventLoop* loop_;
	Socket acceptSocket_;
	Channel acceptChannel_;
	NewConnectionCallback newConnectionCallback_;
	bool listenning_;
};

#endif /* ACCEPTOR_H_ */
