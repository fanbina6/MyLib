/*
 * EventLoop.cpp
 *
 *  Created on: 2013-8-6
 *      Author: fanbin
 */

#include "EventLoop.h"
#include "../mynet/SocketsFun.h"

#include <assert.h>
#include <stdio.h>
#include <poll.h>
#include <sys/eventfd.h>
#include <boost/bind.hpp>



__thread EventLoop* t_loopInThisThread = 0;
const int kPollTimeMs = 10000;

int createEventfd()
{
	int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
	if (evtfd < 0)
	{
		printf("Failed in createEventfd\n");
		abort();
	}
	return evtfd;
}

EventLoop::EventLoop() :
		looping_(false), threadId_(CurrentThread::tid()), quit_(false), poller_(
				Poller::newDefaultPoller(this)), callingPendingFunctors_(false), wakeupFd_(
				createEventfd()), wakeupChannel_(new Channel(this, wakeupFd_))
{
	printf("EventLoop created in thread %d\n", threadId_);
	if (t_loopInThisThread)
	{
		printf("Another EventLoop %s exists in thread %d", t_loopInThisThread,
				threadId_);
	}
	else
	{
		t_loopInThisThread = this;
	}

	wakeupChannel_->setReadCallback(boost::bind(&EventLoop::handleRead, this));

	wakeupChannel_->enableReading();
}

EventLoop::~EventLoop()
{
	assert(!looping_);
	t_loopInThisThread = NULL;
	::close(wakeupFd_);
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
	return t_loopInThisThread;
}

void EventLoop::updateChannel(Channel* channel)
{
	assert(channel->ownerLoop() == this);
	assertInLoopThread();
	poller_->updateChannel(channel);
}

void EventLoop::loop()
{
	assert(!looping_);
	assertInLoopThread();
	looping_ = true;

	while (!quit_)
	{
		this->activeChannels_.clear();
		this->poller_->poll(kPollTimeMs, &activeChannels_);
		for (ChannelList::iterator it = activeChannels_.begin();
				it != activeChannels_.end(); ++it)
				{
			(*it)->handleEvent();
		}
	}
	doPendingFunctors();
	printf("EventLoop %d stop looping\n", CurrentThread::tid());
	looping_ = false;

}

void EventLoop::runInLoop(const Functor& cb)
{
	if (isInLoopThread())
	{
		cb();
	}
	else
	{
		queueInLoop(cb);
	}
}

void EventLoop::queueInLoop(const Functor& cb)
{
	{
		MutexLockGuard lock(mutex_);
		pendingFunctors_.push_back(cb);
	}

	if (!isInLoopThread() || callingPendingFunctors_)
	{
		wakeup();
	}
}

void EventLoop::wakeup()
{
	uint64_t one = 1;
	ssize_t n = SocketsFun::write(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		printf("EventLoop::wakeup() writes %d bytes instead of 8", n);
	}
}

void EventLoop::handleRead()
{
	uint64_t one = 1;
	ssize_t n = SocketsFun::read(wakeupFd_, &one, sizeof one);
	if (n != sizeof one)
	{
		printf("EventLoop::handleRead() reads %d bytes instead of 8", n);
	}
}
void EventLoop::doPendingFunctors()
{
	std::vector<Functor> functors;
	callingPendingFunctors_ = true;

	{
		MutexLockGuard lock(mutex_);
		functors.swap(pendingFunctors_);
	}

	for (size_t i = 0; i < functors.size(); ++i)
	{
		functors[i]();
	}

	callingPendingFunctors_ = false;
}
