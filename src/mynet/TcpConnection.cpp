/*
 * TcpConnection.cpp
 *
 *  Created on: 2013-8-13
 *      Author: fanbin
 */

#include "TcpConnection.h"

TcpConnection::TcpConnection(EventLoop* loop, const string& name, int sockfd,
		const InetAddress& localAddr, const InetAddress& peerAddr):
		loop_(loop),
		name_(name),
		state_(kConnecting),
		socket_(new Socket(sockfd)),
		channel_(new Channel(loop, sockfd)),
		localAddr_(localAddr),
		peerAddr_(peerAddr)

{
}

TcpConnection::~TcpConnection()
{
}

void TcpConnection::handleRead()
{
}

void TcpConnection::connectEstablished()
{
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  setState(kConnected);
  channel_->tie(shared_from_this());
  channel_->enableReading();

  connectionCallback_(shared_from_this());
}

