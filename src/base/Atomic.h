/*
 * Atomic.h
 *
 *  Created on: 2013-8-1
 *      Author: fanbin
 */

#ifndef ATOMIC_H
#define ATOMIC_H

#include <stdint.h>
#include "NonCopyAble.h"

namespace detail
{
template<typename T>
class AtomicIntegerT: NonCopyAble
{
public:
	AtomicIntegerT() :
			value_(0)
	{
	}

	// uncomment if you need copying and assignment
	//
	// AtomicIntegerT(const AtomicIntegerT& that)
	//   : value_(that.get())
	// {}
	//
	// AtomicIntegerT& operator=(const AtomicIntegerT& that)
	// {
	//   getAndSet(that.get());
	//   return *this;
	// }

	T get()
	{
		return __sync_val_compare_and_swap(&value_, 0, 0);
	}

	T getAndAdd(T x)
	{
		return __sync_fetch_and_add(&value_, x);
	}

	T addAndGet(T x)
	{
		return getAndAdd(x) + x;
	}

	T incrementAndGet()
	{
		return addAndGet(1);
	}

	T decrementAndGet()
	{
		return addAndGet(-1);
	}

	void add(T x)
	{
		getAndAdd(x);
	}

	void increment()
	{
		incrementAndGet();
	}

	void decrement()
	{
		decrementAndGet();
	}

	T getAndSet(T newValue)
	{
		return __sync_lock_test_and_set(&value_, newValue);
	}

private:
	volatile T value_;
};
}

typedef detail::AtomicIntegerT<int32_t> AtomicInt32;
typedef detail::AtomicIntegerT<int64_t> AtomicInt64;

#endif  // ATOMIC_H
/*
 *
 一、是什么
 stdint.h是c99中引进的一个标准C库的头文件.
 二、干什么
 stdint.h中定义了一些整数类型，规则如下(其中N可以为8，16，32，64)
 intN_t, int_leastN_t, int_fastN_t表示长度为N位的整型数；
 uintN_t, uint_leastN_t, uint_fastN_t表示长度为N位的无符号整型数 ；
 stdint.h中的常量，定义以上各类型数的最大最小值(其中N可以为8，16，32，64)
 INTN_MIN, UINTN_MIN, INTN_MAX, UINTN_MAX ;
 INT_LEASEN_MIN, INT_LEASEN_MAX ;
 INT_FASTN_MIN, INT_FASTN_MAX ;
 以上类型的C++类型定义等
 三、大数输出
 int64_t数的输出：%lld ;
 uint64_t数的输出：%llu ;
 uint64_t数十六进制输出：%llx ;
 uint64_t数八进制输出：%llo ;
 */
