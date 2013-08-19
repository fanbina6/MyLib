/*
 * Condition.h
 *
 *  Created on: 2013-8-1
 *      Author: fanbin
 */

#ifndef CONDITION_H_
#define CONDITION_H_

#include <errno.h>
#include "Mutex.h"

class Condition: NonCopyAble
{
public:
	explicit Condition(MutexLock& mutex) :
			mutex_(mutex)
	{
		pthread_cond_init(&pcond_, NULL);
	}

	~Condition()
	{
		pthread_cond_destroy(&pcond_);
	}

	void wait()
	{
		MutexLock::UnassignGuard ug(mutex_);
		pthread_cond_wait(&pcond_, mutex_.getPthreadMutex());
	}

	// returns true if time out, false otherwise.
	bool waitForSeconds(int seconds)
	{
		struct timespec abstime;
		clock_gettime(CLOCK_REALTIME, &abstime);
		abstime.tv_sec += seconds;
		MutexLock::UnassignGuard ug(mutex_);
		return ETIMEDOUT
				== pthread_cond_timedwait(&pcond_, mutex_.getPthreadMutex(),
						&abstime);
	}

	void notify()
	{
		pthread_cond_signal(&pcond_);
	}

	void notifyAll()
	{
		pthread_cond_broadcast(&pcond_);
	}

private:
	MutexLock& mutex_;
	pthread_cond_t pcond_;
};

#endif /* CONDITION_H_ */
