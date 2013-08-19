/*
 * CurrentThread.h
 *
 *  Created on: 2013-8-1
 *      Author: fanbin
 */

#ifndef CURRENTTHREAD_H_
#define CURRENTTHREAD_H_

#include <stdint.h>

namespace CurrentThread
{
// internal
extern __thread int t_cachedTid;
extern __thread char t_tidString[32];
extern __thread const char* t_threadName;

void cacheTid();

inline int tid()
{
	if (t_cachedTid == 0)
	{
		cacheTid();
	}
	return t_cachedTid;
}

inline const char* tidString() // for logging
{
	return t_tidString;
}

inline const char* name()
{
	return t_threadName;
}

bool isMainThread();

void sleepUsec(int64_t usec);
}

#endif /* CURRENTTHREAD_H_ */
