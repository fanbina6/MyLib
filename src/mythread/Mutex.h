/*
 * Mutex.h
 *
 *  Created on: 2013-8-1
 *      Author: fanbin
 */

#ifndef MUTEX_H_
#define MUTEX_H_

#include <assert.h>
#include <pthread.h>
#include "../base/NonCopyAble.h"
#include "CurrentThread.h"

class MutexLock: NonCopyAble
{
public:
	MutexLock() :
			holder_(0)
	{
		int ret = pthread_mutex_init(&mutex_, NULL);
		assert(ret == 0);
		(void) ret;
	}

	~MutexLock()
	{
		assert(holder_ == 0);
		int ret = pthread_mutex_destroy(&mutex_);
		assert(ret == 0);
		(void) ret;
	}

	bool isLockedByThisThread() const
	{
		return holder_ == CurrentThread::tid();
	}

	void assertLocked() const
	{
		assert(isLockedByThisThread());
	}

	// internal usage

	void lock()
	{
		pthread_mutex_lock(&mutex_);
		assignHolder();
	}

	void unlock()
	{
		unassignHolder();
		pthread_mutex_unlock(&mutex_);
	}

	pthread_mutex_t* getPthreadMutex() /* non-const */
	{
		return &mutex_;
	}

private:
	friend class Condition;

	class UnassignGuard: NonCopyAble
	{
	public:
		UnassignGuard(MutexLock& owner) :
				owner_(owner)
		{
			owner_.unassignHolder();
		}

		~UnassignGuard()
		{
			owner_.assignHolder();
		}

	private:
		MutexLock& owner_;
	};

	void unassignHolder()
	{
		holder_ = 0;
	}

	void assignHolder()
	{
		holder_ = CurrentThread::tid();
	}

	pthread_mutex_t mutex_;
	pid_t holder_;
};

class MutexLockGuard: NonCopyAble
{
public:
	explicit MutexLockGuard(MutexLock& mutex) :
			mutex_(mutex)
	{
		mutex_.lock();
	}

	~MutexLockGuard()
	{
		mutex_.unlock();
	}

private:

	MutexLock& mutex_;
};

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexLockGuard(x) error "Missing guard object name"

#endif /* MUTEX_H_ */
