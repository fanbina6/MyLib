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
 һ����ʲô
 stdint.h��c99��������һ����׼C���ͷ�ļ�.
 ������ʲô
 stdint.h�ж�����һЩ�������ͣ���������(����N����Ϊ8��16��32��64)
 intN_t, int_leastN_t, int_fastN_t��ʾ����ΪNλ����������
 uintN_t, uint_leastN_t, uint_fastN_t��ʾ����ΪNλ���޷��������� ��
 stdint.h�еĳ������������ϸ��������������Сֵ(����N����Ϊ8��16��32��64)
 INTN_MIN, UINTN_MIN, INTN_MAX, UINTN_MAX ;
 INT_LEASEN_MIN, INT_LEASEN_MAX ;
 INT_FASTN_MIN, INT_FASTN_MAX ;
 �������͵�C++���Ͷ����
 �����������
 int64_t���������%lld ;
 uint64_t���������%llu ;
 uint64_t��ʮ�����������%llx ;
 uint64_t���˽��������%llo ;
 */
