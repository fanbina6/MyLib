/*
 * Thread.h
 *
 *  Created on: 2013-8-6
 *      Author: fanbin
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include <string>
using std::string;

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include "../base/Atomic.h"

class Thread: NonCopyAble
{
public:
	typedef boost::function<void()> ThreadFunc;

	explicit Thread(const ThreadFunc&, const string& name = string());
	~Thread();

	void start();
	int join(); // return pthread_join()

	bool started() const
	{
		return started_;
	}

	pid_t tid() const
	{
		return *tid_;
	}
	const string& name() const
	{
		return name_;
	}

	static int numCreated()
	{
		return numCreated_.get();
	}

private:
	bool started_;
	bool joined_;
	pthread_t pthreadId_;
	boost::shared_ptr<pid_t> tid_;
	ThreadFunc func_;
	string name_;

	static AtomicInt32 numCreated_;
};

#endif /* THREAD_H_ */
