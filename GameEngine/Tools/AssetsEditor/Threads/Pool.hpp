#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <queue>
#include <vector>
#include <functional>

class Pool
{
public:
	Pool(std::size_t numberOfThread)
		: _isRunning(true)
		, _threadNumber(numberOfThread)
	{
		for (auto i = 0; i < numberOfThread; ++i)
		{
			_threads.emplace_back([this](){
				while (1)
				{
					std::unique_lock<std::mutex> lock(this->_mutex);
					while (this->_isRunning && this->_todo.empty())
					{
						this->_conditionnal.wait(lock);
					}
					if (!this->_isRunning && this->_todo.empty())
						return;
					std::function<void()> todo(this->_todo.front());
					this->_todo.pop();
					lock.unlock();
					try
					{
						todo();
					}
					catch (...)
					{
						//
					}
				}
			});
		}
	}

	~Pool()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_isRunning = false;
		lock.unlock();
		_conditionnal.notify_all();
		for (auto &&e : _threads)
			e.join();
		_threads.clear();
	}

template <class Fun, class... Args>
auto addTask(Fun &&function, Args &&...args)
-> std::future<typename std::result_of<Fun(Args...)>::type>
{ 
	typedef typename std::result_of<Fun(Args...)>::type RT;

	if (!_isRunning)
		throw::std::runtime_error("Thread pool is paused");

	auto todo = std::make_shared<std::packaged_task<RT()> >(
		std::bind(std::forward<Fun>(function), std::forward<Args>(args)...)
	);

	std::future<RT> res = todo->get_future();
	std::unique_lock<std::mutex> lock(_mutex);
	_todo.push([todo](){(*todo)(); });
	lock.unlock();
	_conditionnal.notify_one();
	return res;
}


private:
	bool _isRunning;
	std::size_t _threadNumber;
	std::vector <std::thread> _threads;
	std::mutex _mutex;
	std::queue<std::function<void()>> _todo;
	std::condition_variable _conditionnal;
};