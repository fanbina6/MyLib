/*
 * Poller.cpp
 *
 *  Created on: 2013-8-7
 *      Author: fanbin
 */

#include "Poller.h"
#include "PollPoller.h"

Poller::Poller(EventLoop* loop) :
		ownerLoop_(loop)
{
}

Poller::~Poller()
{
}

void Poller::assertInLoopThread()
{
	ownerLoop_->assertInLoopThread();
}

Poller* Poller::newDefaultPoller(EventLoop* loop)
{

	return new PollPoller(loop);
//	if (::getenv("MUDUO_USE_POLL"))
//	{
//		return new PollPoller(loop);
//	}
//	else
//	{
//		return new EPollPoller(loop);
//	}
}
