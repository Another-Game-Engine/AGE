#pragma once

#include <TMQ/Queue.hpp>

namespace AGE
{
	template <typename T>
	void PushTask(const T& e)
	{
		TMQ::HybridQueue::pushSharedFutureTask(e);
		//GetThreadManager()->getAvailableTaskQueue()->pushTask(e);
	}

	template <typename T, typename ...Args>
	void EmplaceTask(Args ...args)
	{
		TMQ::HybridQueue::emplaceSharedTask<T>(args...);
		//GetThreadManager()->getAvailableTaskQueue()->emplaceTask<T>(args...);
	}

	template <typename T, typename F>
	std::future<F> PushFutureTask(const T &e)
	{
		return TMQ::HybridQueue::pushSharedFutureTask<F>(e);
		//return GetThreadManager()->getAvailableTaskQueue(true)->pushFutureTask<F>(e);
	}

	template <typename T, typename F, typename ...Args>
	std::future<F> EmplaceFutureTask(Args ...args)
	{
		return TMQ::HybridQueue::emplaceSharedFutureTask<T, F>(args...);
		//return GetThreadManager()->getAvailableTaskQueue(true)->emplaceFutureTask<T, F>(args...);
	}
}

