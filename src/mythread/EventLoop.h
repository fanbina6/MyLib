/*
 * EventLoop.h
 *
 *  Created on: 2013-8-6
 *      Author: fanbin
 */

#ifndef EVENTLOOP_H_
#define EVENTLOOP_H_

#include "../base/NonCopyAble.h"
#include "CurrentThread.h"
#include "../mynet/Channel.h"
#include "../mynet/Poller.h"
#include "Mutex.h"

#include <stdio.h>
#include <sys/types.h>
#include <vector>
#include <boost/function.hpp>

#include <boost/scoped_ptr.hpp>

class Channel;
class Poller;

class EventLoop: NonCopyAble
{
public:
	typedef boost::function<void()> Functor;

	EventLoop();
	virtual ~EventLoop();

	void updateChannel(Channel* channel);

	void loop();
	void assertInLoopThread()
	{
		if (!isInLoopThread())
		{
			abortNotInLoopThread();
		}
	}

	bool isInLoopThread() const
	{
		return threadId_ == CurrentThread::tid();
	}
	;

	static EventLoop* getEventLoopOfCurrentThread();

	void quit()
	{
		quit_ = true;
		if (!isInLoopThread())
		{
			wakeup();
		}
	}

	//在IO线程内执行某个用户任务回调
	void runInLoop(const Functor& cb);

private:
	void abortNotInLoopThread()
	{
		printf(
				"error, EventLoop::abortNotInLoopThread - EventLoop was created in threadId_ = %d, current thread id = %d",
				threadId_, CurrentThread::tid());
	}

	bool looping_;
	const pid_t threadId_;

	typedef std::vector<Channel*> ChannelList;
	bool quit_;
	boost::scoped_ptr<Poller> poller_;
	ChannelList activeChannels_;

	// work for workInLoop
	void queueInLoop(const Functor& cb);
	void wakeup();
	void handleRead(); //waked up 触发
	void doPendingFunctors();
	bool callingPendingFunctors_;
	int wakeupFd_;
	boost::scoped_ptr<Channel> wakeupChannel_;
	MutexLock mutex_;
	std::vector<Functor> pendingFunctors_;
};

#endif /* EVENTLOOP_H_ */
