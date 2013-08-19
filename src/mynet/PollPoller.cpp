/*
 * PollPoller.cpp
 *
 *  Created on: 2013-8-7
 *      Author: fanbin
 */

#include "PollPoller.h"
#include <assert.h>

PollPoller::PollPoller(EventLoop* loop) :
		Poller(loop)
{
}

PollPoller::~PollPoller()
{
}

Timestamp PollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
	int numEvents = ::poll(&(*(pollfds_.begin())), pollfds_.size(), timeoutMs);
	Timestamp now(Timestamp::now());
	if(numEvents > 0)
	{
		printf("PollPoller::poll: %d events happended!\n", numEvents);
		this->fillActiveChannels(numEvents, activeChannels);
	}
	else if(numEvents == 0)
	{
		printf("PollPoller::poll: notihing happended!\n", numEvents);
	}
	else
	{
		printf("PollPoller::poll: error!\n", numEvents);
	}
	return now;
}
void PollPoller::updateChannel(Channel* channel)
{
	this->assertInLoopThread();
	printf("fd=%d, events=%d \n", channel->fd(), channel->events());
	if (channel->index() < 0)
	{
		assert(channels_.find(channel->fd()) == channels_.end());
		struct pollfd pfd;
		pfd.fd = channel->fd();
		pfd.events = static_cast<short>(channel->events());pfd
		.revents = 0;
		pollfds_.push_back(pfd);
		int idx = static_cast<int>(pollfds_.size()) - 1;
		channel->set_index(idx);
		channels_[pfd.fd] = channel;
	}
	else
	{
		assert(channels_.find(channel->fd()) != channels_.end());
		assert(channels_[channel->fd()] == channel);
		int idx = channel->index();
		assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
		struct pollfd& pfd = pollfds_[idx];
		assert(pfd.fd == channel->fd() || pfd.fd == -1);
		pfd.events = static_cast<short>(channel->events());pfd
		.revents = 0;
		if (channel->isNoneEvent())
		{
			pfd.fd = -1;
		}
	}
}
void PollPoller::removeChannel(Channel* channel)
{

}

void PollPoller::fillActiveChannels(int numEvents,
		ChannelList* activeChannels) const
{
	for (PollFdList::const_iterator pfd = pollfds_.begin();
			pfd != pollfds_.end() && numEvents > 0; ++pfd)
	{
		if (pfd->revents > 0)
		{
			--numEvents;
			ChannelMap::const_iterator ch = channels_.find(pfd->fd);
			assert(ch != channels_.end());
			Channel* channel = ch->second;
			assert(channel->fd() == pfd->fd);
			channel->set_revents(pfd->revents);
			activeChannels->push_back(channel);
		}
	}
}
