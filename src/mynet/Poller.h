/*
 * Poller.h
 *
 *  Created on: 2013-8-7
 *      Author: fanbin
 */

#ifndef POLLER_H_
#define POLLER_H_

#include "../base/NonCopyAble.h"
#include "../base/Timestamp.h"
#include "../mynet/Channel.h"
#include "../mythread/EventLoop.h"


#include <vector>

class Channel;
class EventLoop;

class Poller: NonCopyAble
{
public:
	typedef std::vector<Channel*> ChannelList;

	Poller(EventLoop* loop);
	virtual ~Poller();

	/// Polls the I/O events.
	/// Must be called in the loop thread.
	virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

	/// Changes the interested I/O events.
	/// Must be called in the loop thread.
	virtual void updateChannel(Channel* channel) = 0;

	/// Remove the channel, when it destructs.
	/// Must be called in the loop thread.
	virtual void removeChannel(Channel* channel) = 0;

	static Poller* newDefaultPoller(EventLoop* loop);

	void assertInLoopThread();

private:
	EventLoop* ownerLoop_;
};

#endif /* POLLER_H_ */
