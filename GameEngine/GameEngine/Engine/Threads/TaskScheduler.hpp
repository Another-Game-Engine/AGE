#pragma once

#include <Threads/ThreadManager.hpp>
#include <TMQ/Queue.hpp>
#include <future>

namespace AGE
{
	template <typename T>
	void PushTask(const T& e)
	{
		GetThreadManager()->getAvailableTaskQueue()->pushTask(e);
	}

	template <typename T, typename ...Args>
	void EmplaceTask(Args ...args)
	{
		GetThreadManager()->getAvailableTaskQueue()->emplaceTask<T>(args...);
	}

	template <typename T, typename F>
	std::future<F> PushFutureTask(const T &e)
	{
		return GetThreadManager()->getAvailableTaskQueue(true)->pushFutureTask<F>(e);
	}

	template <typename T, typename F, typename ...Args>
	std::future<F> EmplaceFutureTask(Args ...args)
	{
		return GetThreadManager()->getAvailableTaskQueue(true)->emplaceFutureTask<T, F>(args...);
	}
}

