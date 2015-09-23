#pragma once

#include <mutex>

template <class T>
class DataSwapper
{
private:
	T _data_A;
	T _data_B;
	std::mutex _mutex;
public:
	DataSwapper() = default;
	~DataSwapper()
	{}
	DataSwapper(const DataSwapper&) = delete;
	DataSwapper(const DataSwapper&&) = delete;
	DataSwapper &operator=(const DataSwapper &) = delete;
	DataSwapper &&operator=(const DataSwapper &&) = delete;

	void swap()
	{
		std::lock_guard<std::mutex>(_mutex);
		std::swap(_data_A, _data_B);
	}

	std::mutex &getMutex()
	{
		return _mutex;
	}

	T &getDataA()
	{
		return _data_A;
	}

	T &getDataB()
	{
		return _data_B;
	}
};