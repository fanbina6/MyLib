/*
 * Channel.cpp
 *
 *  Created on: 2013-8-6
 *      Author: fanbin
 */

#include "Channel.h"
#include <poll.h>
#include <stdio.h>

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;

Channel::Channel(EventLoop *loop, int fd) :
		loop_(loop), fd_(fd), events_(0), revents_(0), index_(-1)
{
}

Channel::~Channel()
{
}

void Channel::update()
{
	loop_->updateChannel(this);
}

void Channel::handleEvent()
{
	if (revents_ & POLLNVAL)
	{
		fprintf(stderr, "Channel::handle_event() POLLNVAL");
	}

	if (revents_ & (POLLERR | POLLNVAL))
	{
		if(errorCallback_)
			errorCallback_();
	}

	if(revents_ & (POLLIN | POLLPRI | POLLRDHUP))
	{
		if(readCallback_)
			readCallback_();
	}

	if(revents_ & (POLLOUT))
	{
		if(writeCallback_)
			writeCallback_();
	}
}

void Channel::tie(const boost::shared_ptr<void>& obj)
{
  tie_ = obj;
  tied_ = true;
}

