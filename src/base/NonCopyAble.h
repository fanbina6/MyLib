/*
 * NonCopyAble.h
 *
 *  Created on: 2013-8-1
 *      Author: fanbin
 */

#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

class NonCopyAble
{
protected:
	NonCopyAble()
	{
	}
	~NonCopyAble()
	{
	}
private:
	// emphasize the following members are private
	NonCopyAble(const NonCopyAble&);
	const NonCopyAble& operator=(const NonCopyAble&);
};

#endif /* NONCOPYABLE_H_ */
