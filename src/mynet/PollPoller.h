/*
 * PollPoller.h
 *
 *  Created on: 2013-8-7
 *      Author: fanbin
 */

#ifndef POLLPOLLER_H_
#define POLLPOLLER_H_

#include "Poller.h"
#include <poll.h>
#include <map>



class PollPoller: public Poller
{
public:
	PollPoller(EventLoop* loop);
	virtual ~PollPoller();
	virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels); //milliseconds, negative value in timeout means an infinite timeout
	virtual void updateChannel(Channel* channel);
	virtual void removeChannel(Channel* channel);

private:
	void fillActiveChannels(int numEvents, ChannelList* activeChannels) const;

	typedef std::vector<struct pollfd> PollFdList;
	typedef std::map<int, Channel*> ChannelMap;
	PollFdList pollfds_;
	ChannelMap channels_;
};

#endif /* POLLPOLLER_H_ */
