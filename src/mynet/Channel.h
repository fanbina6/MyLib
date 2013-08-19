/*
 * Channel.h
 *
 *  Created on: 2013-8-6
 *      Author: fanbin
 */

#ifndef CHANNEL_H_
#define CHANNEL_H_

#include "../base/NonCopyAble.h"
#include "../mythread/EventLoop.h"
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

class EventLoop;

class Channel: NonCopyAble
{
public:
	typedef boost::function<void()> EventCallback;

	Channel(EventLoop *loop, int fd);
	virtual ~Channel();

	void handleEvent();
	void setReadCallback(const EventCallback& cb)
	{
		readCallback_ = cb;
	}

	void setWriteCallback(const EventCallback& cb)
	{
		writeCallback_ = cb;
	}

	void setErrorCallback(const EventCallback& cb)
	{
		errorCallback_ = cb;
	}

	int fd() const
	{
		return fd_;
	}

	int events() const
	{
		return events_;
	}

	void set_revents(int revt)
	{
		revents_ = revt;
	}

	bool isNoneEvent() const
	{
		return events_ == kNoneEvent;
	}

	void enableReading()
	{
		events_ |= kReadEvent;
		update();
	}

	//for poller
	int index() const
	{
		return index_;
	}

	void set_index(int idx)
	{
		index_ = idx;
	}

	EventLoop* ownerLoop()
	{
		return loop_;
	}

	void tie(const boost::shared_ptr<void>& obj);
private:

	void update();

	static const int kNoneEvent;
	static const int kReadEvent;
	static const int kWriteEvent;

	EventLoop* loop_;
	const int fd_;
	int events_;
	int revents_;
	int index_; //used by poller

	EventCallback readCallback_;
	EventCallback writeCallback_;
	EventCallback errorCallback_;


	boost::weak_ptr<void> tie_;
	bool tied_;
};

#endif /* CHANNEL_H_ */
